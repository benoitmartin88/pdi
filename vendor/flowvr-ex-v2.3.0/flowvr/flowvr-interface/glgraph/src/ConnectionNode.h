/******* COPYRIGHT ************************************************
 *                                                                 *
 *                             FlowVR                              *
 *                    Graph Visualization Tool                     *
 *                                                                 *
 *-----------------------------------------------------------------*
 * COPYRIGHT (C) 2003-2011                by                       *
 * INRIA                                                           *
 *  ALL RIGHTS RESERVED.                                           *
 *                                                                 *
 * This source is covered by the GNU GPL, please refer to the      *
 * COPYING file for further information.                           *
 *                                                                 *
 *-----------------------------------------------------------------*
 *                                                                 *
 *  Original Contributors:                                         *
 *    Jeremie Allard,                                              *
 *    Thomas Arcila,                                               *
 *    Antoine Meler                                                *
 *    Clement Meniers,                                             *
 *    Bruno Raffin,                                                *
 *                                                                 *
 *******************************************************************
 *                                                                 *
 * File: ./src/utils.cpp                                           *
 *                                                                 *
 * Contacts: Xavier Martin <xavier.martin@imag.fr>                 *
 *                                                                 *
 ******************************************************************/

#ifndef CONNECTIONNODE_H_
#define CONNECTIONNODE_H_

#include "GNode.h"

class ConnectionNode : public GNode {
public:
	ConnectionNode(float x, float y, float z, float width, float height, std::string label, int r=132, int g=197, int b=250, int o_r=0, int o_g=0, int o_b=0, Component* comp=NULL) :
		GNode(x,y,z,width,height,label,r,g,b,o_r,o_g,o_b,comp) {

	}
	//  int r=240, int g=249, int b=255 previous color

	void generateDisplayList() {
		directRenderMode = false;
		nodeDisplayList = glGenLists(1);
		if (nodeDisplayList == 0) {
			directRenderMode = true;
		}
		glNewList(nodeDisplayList, GL_COMPILE);
			// Inside
			glColor3f(fillColor[0], fillColor[1], fillColor[2]);

			glBegin( GL_QUADS);
				glVertex3f(x - width / 2.0, y - height / 2.0, z);
				glVertex3f(x - width / 2.0, y + height / 2.0, z);
				glVertex3f(x + width / 2.0, y + height / 2.0, z);
				glVertex3f(x + width / 2.0, y - height / 2.0, z);
			glEnd();

			// Border
			glColor3f(outlineColor[0], outlineColor[1], outlineColor[2]);
			glLineWidth(1.0f);

			glBegin( GL_LINE_LOOP);
				glVertex3f(x - width / 2.0, y - height / 2.0, z+1);
				glVertex3f(x - width / 2.0, y + height / 2.0, z+1);
				glVertex3f(x + width / 2.0, y + height / 2.0, z+1);
				glVertex3f(x + width / 2.0, y - height / 2.0, z+1);
			glEnd();

			renderStrokeLineFit(x, y, z+1, toChar(label), width, height);
		glEndList();
	}

	void draw();
};

#endif /* CONNECTIONNODE_H_ */
