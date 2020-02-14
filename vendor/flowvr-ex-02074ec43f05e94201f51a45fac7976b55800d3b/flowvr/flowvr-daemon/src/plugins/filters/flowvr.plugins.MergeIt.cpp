/******* COPYRIGHT ************************************************
*                                                                 *
*                             FlowVR                              *
*                     Daemon and Base Plugins                     *
*                                                                 *
*-----------------------------------------------------------------*
* COPYRIGHT (C) 2003-2011                by                       *
* INRIA and                                                       *
* Laboratoire d'Informatique Fondamentale d'Orleans               *
* (FRE 2490) ALL RIGHTS RESERVED.                                 *
*                                                                 *
* This source is covered by the GNU GPL, please refer to the      *
* COPYING file for further information.                           *
*                                                                 *
*-----------------------------------------------------------------*
*                                                                 *
*  Original Contributors:                                         *
*    Jeremie Allard,                                              *
*    Ronan Gaugne,                                                *
*    Valerie Gouranton,                                           *
*    Loick Lecointre,                                             *
*    Sebastien Limet,                                             *
*    Bruno Raffin,                                                *
*    Sophie Robert,                                               *
*    Emmanuel Melin.                                              *
*                                                                 * 
*******************************************************************
*                                                                 *
* File: src/plugins/flowvr.plugins.MergeIt.cpp                    *
*                                                                 *
* Contacts:                                                       *
*  12/10/2003 Jeremie Allard <Jeremie.Allard@imag.fr>             *
*                                                                 *
******************************************************************/
#include "flowvr/daemon.h"
#include "flowvr/plugins/filter.h"
#include "flowvr/plugd/dispatcher.h"
#include "flowvr/plugd/messagequeue.h"
#include "flowvr/mem/sharedmemorymanager.h"
#include <iostream>
#include <sstream>
#include <unistd.h>

namespace flowvr
{

namespace plugins
{

using namespace flowvr::plugd;

/// \brief A filter which chooses  the message corresponding to the it
/// stamp of received orders, and merge the message not yet sent up to
/// this it value.
///
/// <b>Init parameters:</b>
/// -  \<stamp\>name of the stamp to combine\</stamp\> <i>(optional)</i>
/// -  \<scratch\>stampname\</scratch\>  <i>(optional)</i>:  name of a
/// stamp with non-zero  value for messages  which can be discarded if
/// not the most recent one.
///
/// <b>Input ports:</b>
/// -  <b>in</b>: Messages to be filtered.
/// -  <b>order</b>:  Filtering orders  (from a  synchronizer  such as
/// flowvr::plugins::GreedySynchronizor).
///
/// <b>Output Ports:</b>
/// - <b>out</b>: Filtered messages.

class MergeIt : public Filter
{
 public:

  MergeIt(const std::string objID);

  virtual ~MergeIt();

  virtual Class* getClass() const;

  virtual flowvr::plugd::Result init(flowvr::xml::DOMElement* xmlRoot, flowvr::plugd::Dispatcher* dispatcher);

  virtual void newMessageNotification(int mqid, int msgnum, const Message& msg, Dispatcher* dispatcher);
  virtual void newStampListSpecification(int mqid, const Message& msg, Dispatcher* dispatcher);

  enum {
    IDPORT_IN=0,
    IDPORT_ORDER,
    NBPORTS
  };

  int numout;

  int lastnum;
  //StampsWrite laststamps;

protected:

  std::string stampname;
  StampInfo* stamp;

  std::string scratchname;
  StampInfo* scratch;

  std::vector<Message> msgs;

  virtual void sendPendingOrders(plugd::Dispatcher* dispatcher);
};

using namespace flowvr::xml;

/// Constructor.
MergeIt::MergeIt(const std::string objID)
: Filter(objID), numout(0), lastnum(-1), stamp(NULL), scratch(NULL)
{
}

MergeIt::~MergeIt()
{
}

flowvr::plugd::Result MergeIt::init(flowvr::xml::DOMElement* xmlRoot, flowvr::plugd::Dispatcher* dispatcher)
{
  flowvr::plugd::Result result = Filter::init(xmlRoot, dispatcher);
  if (result.error()) return result;

  xml::DOMNodeList* lstamp = xmlRoot->getElementsByTagName("stamp");
  if (lstamp->getLength()>=1)
  {
    stampname = lstamp->item(0)->getTextContent();
  }
  delete lstamp;

  xml::DOMNodeList* lscratch = xmlRoot->getElementsByTagName("scratch");
  if (lscratch->getLength()>=1)
  {
    scratchname = lscratch->item(0)->getTextContent();
  }
  delete lscratch;

  initInputs(NBPORTS);
  inputs[IDPORT_IN]->setName("in");
  //inputs[IDPORT_IN]->storeSpecification();
  inputs[IDPORT_ORDER]->setName("order");

  //only one outputmessagequeue for this filter
  initOutputs(1);
  outputs[0]->setName("out");
  outputs[0]->msgtype=Message::FULL;

  return result;
}

void MergeIt::newMessageNotification(int mqid, int msgnum, const Message& msg, Dispatcher* dispatcher)
{
#ifdef DEBUG
  if (mqid == IDPORT_IN)
    std::cout << objectID()<<": new input "<<msgnum<<" queue size "<<inputs[mqid]->size()<<std::endl;
  else
    std::cout << objectID()<<": new order "<<msgnum<<" queue size "<<inputs[mqid]->size()<<std::endl;
#endif
  sendPendingOrders(dispatcher);
}

void MergeIt::newStampListSpecification(int mqid, const Message& msg, Dispatcher* dispatcher)
{
  if (mqid == IDPORT_IN)
  {
    if (!stampname.empty())
    {
      stamp = inputs[mqid]->getStampList()[stampname];
      if (stamp == NULL)
	std::cerr << objectID() << ":in : ERROR stamp "<<stampname<<" not found."<<std::endl;
#ifdef DEBUG
      else
	std::cout << objectID() << ":in : stamp "<<stampname<<" @ "<<stamp->getOffset()<<std::endl;
#endif
    }
    if (!scratchname.empty())
    {
      scratch = inputs[mqid]->getStampList()[scratchname];
      if (scratch == NULL)
	std::cerr << objectID() << ":in : ERROR stamp "<<scratchname<<" not found."<<std::endl;
#ifdef DEBUG
      else
	std::cout << objectID() << ":in : stamp "<<scratchname<<" @ "<<scratch->getOffset()<<std::endl;
#endif
    }
    // forward specification to out port
#ifdef DEBUG
    std::cout << objectID()<<": forwarding stamps specification"<<std::endl;
#endif
    
    //give the Stamplist to the outputmessage queue    
    outputs[0]->stamps  = inputs[IDPORT_IN]->getStampList();
    outputs[0]->newStampSpecification(dispatcher);
    
    sendPendingOrders(dispatcher);
  }
}

void MergeIt::sendPendingOrders(plugd::Dispatcher* dispatcher)
{ // MAIN FILTER FUNCTION

  if (!inputs[IDPORT_IN]->stampsReceived()) return; // still waiting for stamps specification

  for (;;)
  {
    Message msg;
    int num;
    MessagePut newmsg;
    {
      //ipc::ScopedMTLock locker(globalLock,"sendPendingOrders");
      if (!inputs[IDPORT_ORDER]->frontMsg().valid())
      {
#ifdef DEBUG
	std::cout<<objectID()<<": waiting orders"<<std::endl;
#endif
	return;
      }

      msg = inputs[IDPORT_ORDER]->frontMsg();

      msg.stamps.read(inputs[IDPORT_ORDER]->getStampList().num,num);

      int it,itin;
      msg.stamps.read(inputs[IDPORT_ORDER]->getStampList().it,it);
      bool is_scratch = (it<0);
      it = (it<0?-it:it)-10;
      itin = -10;
      if (is_scratch)
      {
	inputs[IDPORT_ORDER]->eraseFront();
	int nbscratch = 0;
	int num_scratch0 = 0;
	int num_scratch1 = 0;
	if (!inputs[IDPORT_IN]->empty() && scratch != NULL)
	{ // remove any discardable messages
	  for (int num = inputs[IDPORT_IN]->frontNum();
	       num <= inputs[IDPORT_IN]->backNum();
	       num++)
	  {
	    flowvr::Message m = inputs[IDPORT_IN]->get(num);
	    if (!m.valid()) continue;
	    if (!m.stamps.read(inputs[IDPORT_IN]->getStampList().it,itin)) continue;
	    if (itin>=it) break; // reached new front message
	    int val = 0;
	    m.stamps.read(*scratch,val);
	    if (val != 0)
	    { // discard this message
	      if (num == inputs[IDPORT_IN]->frontNum())
		inputs[IDPORT_IN]->eraseFront();
	      else
		inputs[IDPORT_IN]->erase(num);
	      if (nbscratch==0) num_scratch0=num_scratch1=num;
	      else		num_scratch1=num;
	      ++nbscratch;
	    }
	  }
	}
#ifdef DEBUG
	std::cout << objectID()<<": scratched "<<nbscratch<<" messages "<<num_scratch0<<" - "<<num_scratch1<<", "<<inputs[IDPORT_IN]->size()<<" remaining elems in queue."<<std::endl;
#endif
	continue; // do not send any message
      }

      while(!inputs[IDPORT_IN]->empty())
      {
	if (inputs[IDPORT_IN]->frontMsg().valid())
	{
	  if (!inputs[IDPORT_IN]->frontMsg().stamps.read(inputs[IDPORT_IN]->getStampList().it,itin)
	      || itin>=it)
	    break;

	  int val = 0;
	  if (scratch != NULL)
	    inputs[IDPORT_IN]->frontMsg().stamps.read(*scratch,val);
	  if (val == 0)
	  {
#ifdef DEBUG
	    std::cout << objectID() << ": using message "<<inputs[IDPORT_IN]->frontNum()<<std::endl;
#endif
	    msgs.push_back(inputs[IDPORT_IN]->frontMsg());
	  }
	  // else discard this message
#ifdef DEBUG
	  else
	    std::cout << objectID() << ": discarding message "<<inputs[IDPORT_IN]->frontNum()<<std::endl;
#endif
	}
	inputs[IDPORT_IN]->eraseFront();
      }

      if (itin>=it) // && (numout==0 || msgs.size()>0))
      {
	msgs.push_back(inputs[IDPORT_IN]->frontMsg());

	newmsg.stamps.clone(inputs[IDPORT_IN]->frontMsg().stamps,&inputs[IDPORT_IN]->getStampList());
	int num = 0;
	msgs[0].stamps.read(inputs[IDPORT_IN]->getStampList().num,num);
	unsigned int first = (num==lastnum)?1:0;
	//	lastnum=num;
	msgs[msgs.size()-1].stamps.read(inputs[IDPORT_IN]->getStampList().num,lastnum);
	/*
	for(unsigned i=first;i<msgs.size();i++)
	{
	  int num=-2;
	  int it=-2;
	  msgs[i].stamps.read(inputs[IDPORT_IN]->getStampList().num,num);
	  msgs[i].stamps.read(inputs[IDPORT_IN]->getStampList().it,it);
	  std::string source;
	  msgs[i].stamps.read(inputs[IDPORT_IN]->getStampList().source,source);
	  std::cout << "MERGEIT"<<source<<": msgs["<<i<<"] num="<<num<<" it="<<it<<std::endl;
	}
	*/
	if (msgs.size() <= first)
	{ // No new data
#ifdef DEBUG
	  std::cout << objectID() << "("<<numout<<"): No new message"<<std::endl;
#endif
	  newmsg.data = alloc(0);
	}
	else if (msgs.size() == first+1)
	{ // One message
#ifdef DEBUG
	  std::cout << objectID() << "("<<numout<<"): 1 new message"<<std::endl;
#endif
	  newmsg.data = msgs[first].data;
	}
	else
	{
#ifdef DEBUG
	  std::cout << objectID() << "("<<numout<<"): "<<msgs.size()-first<<" new messages"<<std::endl;
#endif
	  size_t size = 0;
	  for (unsigned int i=first;i<msgs.size();i++)
	    size+=msgs[i].data.getSize();

	  BufferWrite newdata = alloc(size);
	  size_t pos = 0;
	  for (unsigned int i=first;i<msgs.size();i++)
	  {
	    memcpy(newdata.getWrite<char>(pos),msgs[i].data.getRead<char>(0),msgs[i].data.getSize());
	    pos += msgs[i].data.getSize();
	  }
	  if (stamp != NULL)
	  {
	    // add all stamp values
	    int val = 0;
	    for (unsigned int i=first;i<msgs.size();i++)
	    {
	      int v = 0;
	      msgs[i].stamps.read(*stamp,v);
	      val+=v;
	    }
	    newmsg.stamps.write(*stamp,val);
	  }
	  if (scratch != NULL)
	  {
	    // non discardable messages merged: force stamp scratch to 0
	    newmsg.stamps.write(*scratch,0);
	  }
	  newmsg.data = newdata;
	}
      }
      else
      {
#ifdef DEBUG
	std::cout<<objectID()<<": waiting message "<<it<<std::endl;
#endif
        return; // missing message;
      }
      inputs[IDPORT_ORDER]->eraseFront();
      num = numout++;
    }
#ifdef DEBUG
    std::cout<<objectID()<<": sending message "<<num<<std::endl;
#endif
    outputs[0]->put(newmsg,dispatcher,num);
    msgs.clear();
  }
}

flowvr::plugd::GenClass<MergeIt> MergeItClass("flowvr.plugins.MergeIt", // name
						"", // description
						&flowvr::plugins::FilterClass
						);

Class* MergeIt::getClass() const
{
  return &MergeItClass;
}

} // namespace plugins

} // namespace flowvr
