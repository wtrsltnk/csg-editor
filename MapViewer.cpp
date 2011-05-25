/* 
 * File:   MapViewer.cpp
 * Author: wouter
 * 
 * Created on April 12, 2011, 7:23 PM
 */

#include "MapViewer.h"
#include "MoveTool.h"
#include "ScaleTool.h"
#include "RotateTool.h"
#include "CameraTool.h"
#include "Status.h"
#include "AllInOneTool.h"
#include "SelectionTool.h"
#include <GLee.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <geo/MapLoader.h>
#include <stdio.h>
#include <GL/freeglut_std.h>
#include <c++/4.4/bits/stl_bvector.h>
#include <GL/freeglut_ext.h>

GlutApplication* gApplication = new MapViewer();

MapViewer::MapViewer()
	: GlutApplication("Map Viewer"), mSelectedBrush(0), mSelectedPlane(0), mMode(0), mDragging(false), mSelectedTool(0)
{
}

MapViewer::~MapViewer()
{
}

bool MapViewer::initialize(int argc, char* argv[])
{
	geo::MapLoader loader;

	if (argc > 1)
	{
		loader.load(argv[1], &this->mScene);
	}
	else
	{
		loader.load("dust_001.map", &this->mScene);
	}
	
	this->mTools.push_back(new CameraTool());
	this->mTools.push_back(new MoveTool());
	this->mTools.push_back(new ScaleTool());
	this->mTools.push_back(new RotateTool());
	this->mTools.push_back(new AllInOneTool());
	this->mTools.push_back(new SelectionTool());

	for (std::vector<Tool*>::iterator itr = this->mTools.begin(); itr != this->mTools.end(); ++itr)
		(*itr)->initialize(this);
	
	this->mSelectedTool = this->mTools[0];
	
	this->mCamera.rotateX(-90 * 3.14159265f / 180.0f);
	
	this->mStatus.mFont = new ui::Font();
	this->mStatus.mFont->initializeFont("Ubuntu-R.ttf");
	
	this->mDiskMenu.initialize();
	
	return true;
}

void MapViewer::resize(int w, int h)
{
	if (h == 0) h = 1;
	float aspect = 1.0f * (float(w)/float(h));

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, aspect, 0.1f, 8000.0f);

	glMatrixMode(GL_MODELVIEW);
}

void MapViewer::render(int time)
{
	Vector3 project2D;
	if (this->mSelectedTool != 0)
		this->mSelectedTool->prerender(time);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
	glPushMatrix();
	
	this->mCamera.update();

	float lineColor[] = { -1, 1, 1, 1.0f };
	float red[] = { 1, 0, 0, 1.0f };
	float white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float bb[] = { 0.0f, 1.0f, 0.0f, 0.5f };
	glEnable(GL_DEPTH_TEST);
	for(std::vector<geo::Entity*>::iterator e = this->mScene.mEntities.begin(); e != this->mScene.mEntities.end(); ++e)
	{
		for (std::vector<geo::Brush*>::iterator b = (*e)->mBrushes.begin(); b != (*e)->mBrushes.end(); ++b)
			this->renderBrush(*b, lineColor);
	}
	if (this->mSelectedBrush != 0)
	{
		glDisable(GL_DEPTH_TEST);
		if (this->mSelectedPlane != 0)
		{
//			glEnable(GL_BLEND);
			glColor4f(0, 0, 1, 0.5f);
			glBegin(GL_POLYGON);
			for(std::vector<int>::iterator itr = this->mSelectedPlane->mIndices.begin(); itr != this->mSelectedPlane->mIndices.end(); ++itr)
				glVertex3fv(this->mSelectedBrush->mVertices[(*itr)]);
			glEnd();
//			glDisable(GL_BLEND);
		}
		glLineWidth(1);
		this->renderBoundingBox(this->mSelectedBrush->mMins, this->mSelectedBrush->mMaxs, bb);
		glDisable(GL_DEPTH_TEST);
		glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
		glLineWidth(1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		for(std::vector<geo::Plane>::iterator p = this->mSelectedBrush->mPlanes.begin(); p != this->mSelectedBrush->mPlanes.end(); ++p)
		{
			glBegin(GL_POLYGON);
			for(std::vector<int>::iterator itr = (*p).mIndices.begin(); itr != (*p).mIndices.end(); ++itr)
				glVertex3fv(this->mSelectedBrush->mVertices[(*itr)]);
			glEnd();
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (this->mSelectedTool != 0)
			this->mSelectedTool->render(time);
		this->getScreenPosition(this->mSelectionOrigin, project2D);

	}
	glPopMatrix();
	
	glDisable(GL_DEPTH_TEST);
	int x = 0;
	for (std::vector<Tool*>::iterator itr = this->mTools.begin(); itr != this->mTools.end(); ++itr)
	{
		glViewport(0, height-70-x*50, width/10, height/10);
		(*itr)->renderMinitature((*itr) == this->mSelectedTool);
		x++;
	}
	
	glViewport(0, 0, width, height);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0, viewport[2], 0, viewport[3], -10.0f, 10.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	this->mStatus.render(time);
	
	if (this->mSelectedBrush != 0)
	{
		glRasterPos2d(project2D.x(), project2D.y());
		glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)"Wouter");

//		glEnable(GL_TEXTURE_2D);
//		this->mDiskMenu.mPosition = project2D;
//		this->mDiskMenu.render(time);
//		glDisable(GL_TEXTURE_2D);
	}
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void MapViewer::renderBrush(geo::Brush* brush, float lineColor[])
{
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (lineColor[0] > 0)
	{
		glColor3f(lineColor[0], lineColor[1], lineColor[2]);
		for(std::vector<geo::Plane>::iterator p = brush->mPlanes.begin(); p != brush->mPlanes.end(); ++p)
		{
			glBegin(GL_POLYGON);
			for(std::vector<int>::iterator itr = (*p).mIndices.begin(); itr != (*p).mIndices.end(); ++itr)
				glVertex3fv(brush->mVertices[(*itr)]);
			glEnd();
		}
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3fv(brush->mColor);
	for(std::vector<geo::Plane>::iterator p = brush->mPlanes.begin(); p != brush->mPlanes.end(); ++p)
	{
		glBegin(GL_POLYGON);
		for(std::vector<int>::iterator itr = (*p).mIndices.begin(); itr != (*p).mIndices.end(); ++itr)
			glVertex3fv(brush->mVertices[(*itr)]);
		glEnd();
	}

	glColor3f(1,0,0);
	glBegin(GL_POINTS);
	for(std::vector<Vector3>::iterator itr = brush->mVertices.begin(); itr != brush->mVertices.end(); ++itr)
		glVertex3fv((*itr));
	glEnd();
}

geo::Brush* MapViewer::selectBrush(int mousex, int mousey)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);

	this->mCamera.update();

	float lineColor[] = { -1, 1, 1 };
	float red[] = { -1, 0, 0 };
	for(std::vector<geo::Entity*>::iterator e = this->mScene.mEntities.begin(); e != this->mScene.mEntities.end(); ++e)
	{
		for (std::vector<geo::Brush*>::iterator b = (*e)->mBrushes.begin(); b != (*e)->mBrushes.end(); ++b)
		{
			this->renderBrush(*b, lineColor);
		}
	}

	GLubyte pixel[3];
	glReadBuffer(GL_BACK);
	glReadPixels(mousex, mousey, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *)pixel);

	for(std::vector<geo::Entity*>::iterator e = this->mScene.mEntities.begin(); e != this->mScene.mEntities.end(); ++e)
	{
		for (std::vector<geo::Brush*>::iterator b = (*e)->mBrushes.begin(); b != (*e)->mBrushes.end(); ++b)
		{
			if ((*b)->mColor[0] == pixel[0]/255.0f && (*b)->mColor[1] == pixel[1]/255.0f && (*b)->mColor[2] == pixel[2]/255.0f)
			{
				return (*b);
			}
		}
	}
	return 0;
}

geo::Plane* MapViewer::selectPlane(geo::Brush* brush, int mousex, int mousey)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_BUFFER);

	glLoadIdentity();

	this->mCamera.update();

	int index = 1;
	for (std::vector<geo::Plane>::iterator p = brush->mPlanes.begin(); p != brush->mPlanes.end(); ++p)
	{
		glColor3f(index/255.0f, 0, 0);
		glBegin(GL_POLYGON);
		for(std::vector<int>::iterator itr = (*p).mIndices.begin(); itr != (*p).mIndices.end(); ++itr)
			glVertex3fv(brush->mVertices[(*itr)]);
		glEnd();
		index++;
	}
	
	GLubyte pixel[3];
	glReadBuffer(GL_BACK);
	glReadPixels(mousex, mousey, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *)pixel);

	if (pixel[0] > 0)
		return &brush->mPlanes[pixel[0]-1];
	
	return 0;
}

bool MapViewer::selectHandle(int mousex, int mousey)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_BUFFER);

	glLoadIdentity();

	this->mCamera.update();

	glColor3f(1.0f, 1.0f, 1.0f);
	if (this->mSelectedTool != 0)
		this->mSelectedTool->renderHitTest();
	
	GLubyte pixel[3];
	glReadBuffer(GL_BACK);
	glReadPixels(mousex, mousey, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *)pixel);

	if (pixel[0] == 255 && pixel[1] == 255 && pixel[2] == 255)
		return true;
	
	return false;
}

Tool* MapViewer::testMenu(int mousex, int mousey)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_BUFFER);

	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	int x = 0;
	for (std::vector<Tool*>::iterator itr = this->mTools.begin(); itr != this->mTools.end(); ++itr)
	{
		glViewport(0, height-70-x*50, width/10, height/10);
		glColor3f(x/255.0f, 0, 0);
		(*itr)->renderHitTestMinitature();
		x++;
	}
	
	GLubyte pixel[3];
	glReadBuffer(GL_BACK);
	glReadPixels(mousex, mousey, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *)pixel);

	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	if (pixel[0] == 255 && pixel[1] == 255 && pixel[2] == 255)
		return 0;
	
	return this->mTools[pixel[0]];
}

bool MapViewer::getScreenPosition(const Vector3& worldPosition, Vector3& screenPosition)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLdouble winX, winY, winZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	gluProject(
			worldPosition.x(), worldPosition.y(), worldPosition.z(),
			modelview, projection, viewport, 
			(GLdouble*)&winX, (GLdouble*)&winY, (GLdouble*)&winZ
		);
	
	screenPosition.x(winX);
	screenPosition.y(winY);
	screenPosition.z(winZ);
	
	return true;
}

void MapViewer::onKeyDown(Key::Code key)
{
	if (this->mSelectedTool == 0 || this->mSelectedTool->onKeyDown(key) == false)
	{
		if (key == Key::Escape)
		{
			this->quit();
		}
		else
		{
			for (std::vector<Tool*>::iterator itr = this->mTools.begin(); itr != this->mTools.end(); ++itr)
			{
				if ((*itr)->activatorKey() == key)
				{
					if (this->mSelectedTool != 0)
						this->mSelectedTool->deselect();
					this->mSelectedTool = (*itr);
					this->mSelectedTool->select();
					break;
				}
			}
		}
	}
}

void MapViewer::onKeyUp(Key::Code key)
{
	if (this->mSelectedTool != 0)
		this->mSelectedTool->onKeyUp(key);
}

void MapViewer::onMouseButtonDown(Mouse::Button button)
{
	if (button == Mouse::Left)
	{
		Tool* tool = this->testMenu(MouseState::currentState().getMousePositionX(), MouseState::currentState().getMousePositionY());
		if (tool != 0)
		{
			if (this->mSelectedTool != 0)
				this->mSelectedTool->deselect();
			this->mSelectedTool = tool;
			this->mSelectedTool->select();
			return;
		}
	}
		
	if (this->mSelectedTool != 0)
		if (this->mSelectedTool->onMouseButtonDown(button) == true)
			return;
}

void MapViewer::onMouseButtonUp(Mouse::Button button)
{
	if (this->mSelectedTool != 0)
		this->mSelectedTool->onMouseButtonUp(button);
}

void MapViewer::onMouseMove(int x, int y)
{
	this->mDiskMenu.testHover(x, y);
	if (this->mStatus.isStatusVisible() == false)
	{
		Tool* tool = this->testMenu(x, y);
		if (tool != 0)
				this->mStatus.setStatus(tool->title(), -1);
		else
			this->mStatus.setStatus("", 0);
	}
	
	if (this->mSelectedTool != 0)
		this->mSelectedTool->onMouseMove(x, y);
}

void MapViewer::renderBoundingBox(const float mins[], const float maxs[], float color[])
{
	glActiveTextureARB(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(color[0], color[1], color[2], color[3]);

	glBegin(GL_LINES);
	// bottom
	glVertex3f(mins[0], mins[1], mins[2]);
	glVertex3f(maxs[0], mins[1], mins[2]);

	glVertex3f(maxs[0], mins[1], mins[2]);
	glVertex3f(maxs[0], maxs[1], mins[2]);

	glVertex3f(maxs[0], maxs[1], mins[2]);
	glVertex3f(mins[0], maxs[1], mins[2]);

	glVertex3f(mins[0], maxs[1], mins[2]);
	glVertex3f(mins[0], mins[1], mins[2]);

	// top
	glVertex3f(mins[0], maxs[1], maxs[2]);
	glVertex3f(maxs[0], maxs[1], maxs[2]);

	glVertex3f(maxs[0], maxs[1], maxs[2]);
	glVertex3f(maxs[0], mins[1], maxs[2]);

	glVertex3f(maxs[0], mins[1], maxs[2]);
	glVertex3f(mins[0], mins[1], maxs[2]);

	glVertex3f(mins[0], mins[1], maxs[2]);
	glVertex3f(mins[0], maxs[1], maxs[2]);

	//sides
	glVertex3f(mins[0], maxs[1], mins[2]);
	glVertex3f(mins[0], maxs[1], maxs[2]);

	glVertex3f(maxs[0], maxs[1], mins[2]);
	glVertex3f(maxs[0], maxs[1], maxs[2]);

	glVertex3f(maxs[0], mins[1], mins[2]);
	glVertex3f(maxs[0], mins[1], maxs[2]);

	glVertex3f(mins[0], mins[1], mins[2]);
	glVertex3f(mins[0], mins[1], maxs[2]);
	glEnd();
}
