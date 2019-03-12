/******* COPYRIGHT ************************************************
*                                                                 *
*                         FlowVR VRPN                             *
*                    FlowVR VRPN Coupling Modules                 *
*                                                                 *
*-----------------------------------------------------------------*
 * COPYRIGHT (C) 2003-2011                by                       *
* Laboratoire d'Informatique Fondamentale d'Orleans               *
* (EA 4022) ALL RIGHTS RESERVED.                                  *
*                                                                 *
* This source is covered by the GNU LGPL, please refer to the     *
* COPYING-LIB file for further information.                       *
*                                                                 *
*-----------------------------------------------------------------*
*                                                                 *
*  Original Contributors:                                         *
*    Sebastien Limet,                                             *
*    Sophie Robert.                                               *
*                                                                 * 
*******************************************************************
*                                                                 *
* File: ./include/flowvr-vrpn/components/moduleflowvr_analog.comp.h*
*                                                                 *
* Contacts:                                                       *
*  01/06/2008 Sophie Robert <Sophie.Robert@univ-orleans.fr>       *
*                                                                 *
******************************************************************/
#include <flowvr/app/components/module.comp.h>

#ifndef _MODULEFLOWVRANALOG_H_
#define _MODULEFLOWVRANALOG_H_

using namespace flowvr::app;

namespace flowvrvrpn{

class Moduleflowvr_analog : public Module 
{
public :
  Moduleflowvr_analog(const std::string& id_) : Module(id_)
  {

    setInfo("Transforms VRPN analog messages to ftl ones");

    // Interface declaration
    addPort("vrpn_analog", INPUT);
    addPort("ftl_analog", OUTPUT);
    // Class Module has created beginIt and endIt ports 
  };

  // Mandatory create method
  virtual Component* create() const { return new Moduleflowvr_analog(getId());};
};

};
#endif 
