/* 
 * File:   MapViewer.cpp
 * Author: wouter
 * 
 * Created on April 12, 2011, 7:23 PM
 */

#include "MapViewer.h"
#include <GLee.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <geo/MapLoader.h>
#include <stdio.h>

#define PI 3.14159265
#define Deg2Rad(Ang) ((float)( Ang * PI / 180.0 ))

int startx = 0, starty = 0;

GlutApplication* gApplication = new MapViewer();

MapViewer::MapViewer()
	: GlutApplication("Map Viewer"), mSelectedBrush(0)
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

	this->mCamera.rotateX(Deg2Rad(-90));
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glLoadIdentity();
	
	static int lastTime = 0;
	float speed = 5.0f * ((time - lastTime) / 10.0f);
	lastTime = time;

	if (KeyboardState::currentState().isKeyPressed(Key::w))
		this->mCamera.moveForward(speed);
	if (KeyboardState::currentState().isKeyPressed(Key::s))
		this->mCamera.moveForward(-speed);
	if (KeyboardState::currentState().isKeyPressed(Key::a))
		this->mCamera.moveLeft(speed);
	if (KeyboardState::currentState().isKeyPressed(Key::d))
		this->mCamera.moveLeft(-speed);

	this->mCamera.update();

	float lineColor[] = { 1, 1, 1 };
	float red[] = { 1, 0, 0 };
	for(std::vector<geo::Entity*>::iterator e = this->mScene.mEntities.begin(); e != this->mScene.mEntities.end(); ++e)
	{
		for (std::vector<geo::Brush*>::iterator b = (*e)->mBrushes.begin(); b != (*e)->mBrushes.end(); ++b)
		{
			glLineWidth(1);
			if (this->mSelectedBrush == (*b))
			{
			glLineWidth(2);
				this->renderBrush(*b, red);
			}
			else
			{
				this->renderBrush(*b, lineColor);
			}
		}
	}
//	selectBrush(0, 0);
}

void MapViewer::renderBrush(geo::Brush* brush, float lineColor[3])
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (lineColor[0] > 0)
		glColor3f(lineColor[0], lineColor[1], lineColor[2]);
	else
		glColor3fv(brush->mColor);
	for(std::vector<geo::Plane>::iterator p = brush->mPlanes.begin(); p != brush->mPlanes.end(); ++p)
	{
		glBegin(GL_POLYGON);
		for(std::vector<int>::iterator itr = (*p).mIndices.begin(); itr != (*p).mIndices.end(); ++itr)
			glVertex3fv(brush->mVertices[(*itr)]);
		glEnd();
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

void MapViewer::renderBoundingBox(const float mins[], const float maxs[], float color[])
{
	glActiveTextureARB(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);
	glColor3f(color[0], color[1], color[2]);
	glLineWidth(1.0f);

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

geo::Brush* MapViewer::selectBrush(int mousex, int mousey)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glLoadIdentity();

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

	printf("%d %d %d\n", pixel[0], pixel[1], pixel[2]);
	for(std::vector<geo::Entity*>::iterator e = this->mScene.mEntities.begin(); e != this->mScene.mEntities.end(); ++e)
	{
		for (std::vector<geo::Brush*>::iterator b = (*e)->mBrushes.begin(); b != (*e)->mBrushes.end(); ++b)
		{
			if ((*b)->mColor[0] == pixel[0]/255.0f && (*b)->mColor[1] == pixel[1]/255.0f && (*b)->mColor[2] == pixel[2]/255.0f)
			{
				printf("Foundit!\n");
				return (*b);
			}
		}
	}
}

void MapViewer::onKeyDown(Key::Code key)
{
	if (key == Key::Escape)
	{
		this->quit();
	}
}

void MapViewer::onMouseButtonDown(Mouse::Button button)
{
	if (button == Mouse::Left)
	{
		// Select Brush here
		geo::Brush* b = this->selectBrush(MouseState::currentState().getMousePositionX(), MouseState::currentState().getMousePositionY());
		if (b != 0)
			this->mSelectedBrush = b;
		
	}
}

void MapViewer::onMouseMove(int x, int y)
{
	if (MouseState::currentState().isButtonPressed(Mouse::Middle))
		this->mCamera.rotate(Deg2Rad((starty-y)/10.0f), 0, Deg2Rad((x-startx)/10.0f));

	startx = x;
	starty = y;
}
