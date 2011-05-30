/* 
 * File:   Tool.cpp
 * Author: wouter
 * 
 * Created on May 23, 2011, 11:11 AM
 */

#include "Tool.h"
#include <GLee.h>
#include <math.h>

Tool::Tool(const char* title, Key::Code activatorKey)
	: mActivatorKey(activatorKey)
{
	int i = 0;
	while (title[i] != '\0')
		this->mTitle[i] = title[i++];
	this->mTitle[i] = title[i];
}

Tool::~Tool()
{
}

bool Tool::initialize(MapViewer* viewer)
{
	this->mViewer = viewer;
	
	return true;
}

const char* Tool::title() const
{
	return this->mTitle;
}

Key::Code Tool::activatorKey() const
{
	return this->mActivatorKey;
}

void Tool::renderGrid(int cellsize, int cellcount, Camera& camera)
{
	int X = 0, Y = 1, Z = 2;

	if (fabs(camera.forward().x()) > fabs(camera.forward().z()) && 
			fabs(camera.forward().x()) > fabs(camera.forward().y()))
	{
		X = 2;
		Y = 1;
		Z = 0;
	}
	if (fabs(camera.forward().y()) > fabs(camera.forward().z()) && 
			fabs(camera.forward().y()) > fabs(camera.forward().x()))
	{
		X = 1;
		Y = 2;
		Z = 0;
	}
	
	glColor4f(1, 1, 1, 0.2f);
	glBegin(GL_LINES);
	for (int i = 0; i <= cellcount; i++)
	{
		float l1[] = { -cellsize * (cellcount/2.0f) + i * cellsize,  cellsize * (cellcount/2.0f), 0.0f };
		float l2[] = { -cellsize * (cellcount/2.0f) + i * cellsize, -cellsize * (cellcount/2.0f), 0.0f };
		float l3[] = {  cellsize * (cellcount/2.0f), -cellsize * (cellcount/2.0f) + i * cellsize, 0.0f };
		float l4[] = { -cellsize * (cellcount/2.0f), -cellsize * (cellcount/2.0f) + i * cellsize, 0.0f };
		
		glVertex3f(l1[X], l1[Y], l1[Z]);
		glVertex3f(l2[X], l2[Y], l2[Z]);
		glVertex3f(l3[X], l3[Y], l3[Z]);
		glVertex3f(l4[X], l4[Y], l4[Z]);
	}
	glEnd();
}
