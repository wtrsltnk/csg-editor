/* 
 * File:   Tool.cpp
 * Author: wouter
 * 
 * Created on May 23, 2011, 11:11 AM
 */

#include "Tool.h"
#include "MapViewer.h"
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

    if (fabs(camera.forward().x) > fabs(camera.forward().z) &&
            fabs(camera.forward().x) > fabs(camera.forward().y))
	{
		X = 2;
		Y = 1;
		Z = 0;
	}
    if (fabs(camera.forward().y) > fabs(camera.forward().z) &&
            fabs(camera.forward().y) > fabs(camera.forward().x))
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

geo::Brush* Tool::selectBrush(int mousex, int mousey)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);

	this->mViewer->mCamera.update();

	for(std::vector<geo::Entity*>::iterator e = this->mViewer->mScene.mEntities.begin(); e != this->mViewer->mScene.mEntities.end(); ++e)
	{
		for (std::vector<geo::Brush*>::iterator b = (*e)->mBrushes.begin(); b != (*e)->mBrushes.end(); ++b)
		{
			glColor3ubv((*b)->mColor);
			for(std::vector<geo::Plane>::iterator p = (*b)->mPlanes.begin(); p != (*b)->mPlanes.end(); ++p)
			{
				glBegin(GL_POLYGON);
                for (int index : (*p).mIndices)
                    glVertex3fv(&((*b)->mVertices[index][0]));
				glEnd();
			}
		}
	}

	GLubyte pixel[3];
	glReadBuffer(GL_BACK);
	glReadPixels(mousex, mousey, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *)pixel);

	for(std::vector<geo::Entity*>::iterator e = this->mViewer->mScene.mEntities.begin(); e != this->mViewer->mScene.mEntities.end(); ++e)
	{
		for (std::vector<geo::Brush*>::iterator b = (*e)->mBrushes.begin(); b != (*e)->mBrushes.end(); ++b)
		{
			if ((*b)->mColor[0] == pixel[0] && (*b)->mColor[1] == pixel[1] && (*b)->mColor[2] == pixel[2])
			{
				return (*b);
			}
		}
	}
	return 0;
}

bool Tool::selectHandle(int mousex, int mousey)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_BUFFER);

	glLoadIdentity();

	this->mViewer->mCamera.update();

	glColor3f(1.0f, 1.0f, 1.0f);
	if (this->mViewer->mSelectedTool != 0)
		this->mViewer->mSelectedTool->renderHitTest();
	
	GLubyte pixel[3];
	glReadBuffer(GL_BACK);
	glReadPixels(mousex, mousey, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *)pixel);

	if (pixel[0] == 255 && pixel[1] == 255 && pixel[2] == 255)
		return true;
	
	return false;
}
