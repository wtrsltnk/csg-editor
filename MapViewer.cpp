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

#define PI 3.14159265
#define Deg2Rad(Ang) ((float)( Ang * PI / 180.0 ))

int startx = 0, starty = 0;

GlutApplication* gApplication = new MapViewer();

MapViewer::MapViewer()
	: GlutApplication("Map Viewer")
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

	for(std::vector<geo::Entity*>::iterator e = this->mScene.mEntities.begin(); e != this->mScene.mEntities.end(); ++e)
	{
		for (std::vector<geo::Brush*>::iterator b = (*e)->mBrushes.begin(); b != (*e)->mBrushes.end(); ++b)
		{
			this->renderBrush(*b);
		}
	}

}

void MapViewer::renderBrush(geo::Brush* brush)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3fv(brush->mColor);
	for(std::vector<geo::Plane>::iterator p = brush->mPlanes.begin(); p != brush->mPlanes.end(); ++p)
	{
		glBegin(GL_POLYGON);
		for(std::vector<int>::iterator itr = (*p).mIndices.begin(); itr != (*p).mIndices.end(); ++itr)
		{
			glVertex3f(brush->mVertices[(*itr)][0], brush->mVertices[(*itr)][1], brush->mVertices[(*itr)][2]);
		}
		glEnd();
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(1,1,1);
	glColor3fv(brush->mColor);
	for(std::vector<geo::Plane>::iterator p = brush->mPlanes.begin(); p != brush->mPlanes.end(); ++p)
	{
		glBegin(GL_POLYGON);
		for(std::vector<int>::iterator itr = (*p).mIndices.begin(); itr != (*p).mIndices.end(); ++itr)
		{
			glVertex3f(brush->mVertices[(*itr)][0], brush->mVertices[(*itr)][1], brush->mVertices[(*itr)][2]);
		}
		glEnd();
	}
	glColor3f(1,0,0);
	glBegin(GL_POINTS);
	for(std::vector<Vector3>::iterator itr = brush->mVertices.begin(); itr != brush->mVertices.end(); ++itr)
	{
		glVertex3fv((*itr));
	}
	glEnd();
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
	}
}

void MapViewer::onMouseMove(int x, int y)
{
	if (MouseState::currentState().isButtonPressed(Mouse::Middle))
		this->mCamera.rotate(Deg2Rad((starty-y)/10.0f), 0, Deg2Rad((x-startx)/10.0f));

	startx = x;
	starty = y;
}
