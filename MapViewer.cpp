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
#include <GL/freeglut_std.h>
#include <c++/4.4/bits/stl_bvector.h>

#define PI 3.14159265
#define Deg2Rad(Ang) ((float)( Ang * PI / 180.0 ))

int startx = 0, starty = 0;
GLUquadricObj* quad = 0;

GlutApplication* gApplication = new MapViewer();

MapViewer::MapViewer()
	: GlutApplication("Map Viewer"), mSelectedBrush(0), mSelectedPlane(0), mMode(0), mDragging(false)
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
	quad = gluNewQuadric();
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

	if (KeyboardState::currentState().isKeyPressed(Key::Up))
		this->mCamera.moveForward(speed);
	if (KeyboardState::currentState().isKeyPressed(Key::Down))
		this->mCamera.moveForward(-speed);
	if (KeyboardState::currentState().isKeyPressed(Key::Left))
		this->mCamera.moveLeft(speed);
	if (KeyboardState::currentState().isKeyPressed(Key::Right))
		this->mCamera.moveLeft(-speed);

	this->mCamera.update();

	float lineColor[] = { -1, 1, 1, 1.0f };
	float red[] = { 1, 0, 0, 1.0f };
	float white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
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
		glColor3f(1, 1, 1);
		glLineWidth(2);
		this->renderBoundingBox(this->mSelectedBrush->mMins, this->mSelectedBrush->mMaxs, white);
		glPushMatrix();
		glTranslatef(this->mSelectionOrigin.x(), this->mSelectionOrigin.y(), this->mSelectionOrigin.z());
		glScalef(10.0f, 10.0f, 10.0f);
		if (this->mMode == EditMode::Scale)
			glutWireSphere(1, 8, 8);
		else if (this->mMode == EditMode::Move)
			glutWireCube(1);
		else if (this->mMode == EditMode::Rotate)
			glutWireTetrahedron();
		glPopMatrix();
	}
}

void MapViewer::renderBrush(geo::Brush* brush, float lineColor[])
{
	glEnable(GL_DEPTH_TEST);
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
	glPushMatrix();
	glTranslatef(this->mSelectionOrigin.x(), this->mSelectionOrigin.y(), this->mSelectionOrigin.z());
	glScalef(10.0f, 10.0f, 10.0f);
	if (this->mMode == EditMode::Scale)
		glutSolidSphere(1, 8, 8);
	else if (this->mMode == EditMode::Move)
		glutSolidCube(1);
	else if (this->mMode == EditMode::Rotate)
		glutSolidTetrahedron();
	glPopMatrix();
	
	GLubyte pixel[3];
	glReadBuffer(GL_BACK);
	glReadPixels(mousex, mousey, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *)pixel);

	if (pixel[0] == 255)
		return true;
	return false;
}

void MapViewer::onKeyDown(Key::Code key)
{
	if (key == Key::Escape)
	{
		this->quit();
	}
	else if (key == Key::g || key == Key::G)
	{
		this->mMode = EditMode::Move;
	}
	else if (key == Key::s || key == Key::S)
	{
		this->mMode = EditMode::Scale;
	}
	else if (key == Key::r || key == Key::R)
	{
		this->mMode = EditMode::Rotate;
	}
	else if (key == Key::x)
	{
		if (this->mSelectedBrush != 0)
			this->mSelectedBrush->move(10, 0, 0);
	}
	else if (key == Key::c)
	{
		if (this->mSelectedBrush != 0)
			this->mSelectedBrush->move(-10, 0, 0);
	}
	else if (key == Key::v)
	{
		if (this->mSelectedBrush != 0)
			this->mSelectedBrush->scale(1.5f, 1, 1, this->mSelectionOrigin);
	}
	else if (key == Key::b)
	{
		if (this->mSelectedBrush != 0)
			this->mSelectedBrush->scale(0.75f, 1, 1, this->mSelectionOrigin);
	}
	else if (key == Key::n)
	{
		if (this->mSelectedBrush != 0)
			this->mSelectedBrush->rotate(0.75f, 0, 0, this->mSelectionOrigin);
	}
	else if (key == Key::m)
	{
		if (this->mSelectedBrush != 0)
			this->mSelectedBrush->rotate(-0.75f, 0, 0, this->mSelectionOrigin);
	}
}

void MapViewer::onMouseButtonDown(Mouse::Button button)
{
	if (button == Mouse::Left)
	{
		if (this->mSelectedBrush != 0)
		{
			if (this->mSelectedPlane == 0)
			{
				if (this->selectHandle(MouseState::currentState().getMousePositionX(), MouseState::currentState().getMousePositionY()))
				{
					startx = MouseState::currentState().getMousePositionX();
					starty = MouseState::currentState().getMousePositionY();
					this->mDragging = true;
					return;
				}
			}
			geo::Plane* plane = this->selectPlane(this->mSelectedBrush, MouseState::currentState().getMousePositionX(), MouseState::currentState().getMousePositionY());
			if (plane == 0)
			{
				// Select Brush here
				geo::Brush* b = this->selectBrush(MouseState::currentState().getMousePositionX(), MouseState::currentState().getMousePositionY());
				if (b != 0)
				{
					this->mSelectedBrush = b;
					this->mSelectionOrigin = this->mSelectedBrush->origin();
				}
			}
			else
			{
				if (this->mSelectedPlane == plane)
				{
					this->mSelectedPlane = 0;
					this->mSelectionOrigin = this->mSelectedBrush->origin();
				}
				else
				{
				this->mSelectedPlane = plane;
				this->mSelectionOrigin = plane->average;
				}
			}
		}
		else
		{
			// Select Brush here
			geo::Brush* b = this->selectBrush(MouseState::currentState().getMousePositionX(), MouseState::currentState().getMousePositionY());
			if (b != 0)
			{
				this->mSelectedBrush = b;
				this->mSelectionOrigin = this->mSelectedBrush->origin();
			}
		}
	}
}

void MapViewer::onMouseButtonUp(Mouse::Button button)
{
	this->mDragging = false;
}

void MapViewer::onMouseMove(int x, int y)
{
	if (MouseState::currentState().isButtonPressed(Mouse::Middle))
		this->mCamera.rotate(Deg2Rad((starty-y)/10.0f), 0, Deg2Rad((x-startx)/10.0f));
	else if (MouseState::currentState().isButtonPressed(Mouse::Left))
	{
		if (this->mDragging && this->mSelectedBrush != 0)
		{
			if (this->mSelectedPlane != 0)
			{
				// dragg plane
			}
			else
			{
				// dragg brush
				Vector3 left = this->mCamera.left().unit() * (x-startx);
				Vector3 up = this->mCamera.up().unit() * (y-starty);
				this->mSelectedBrush->move(left.x(), left.y(), left.z());
				this->mSelectedBrush->move(up.x(), up.y(), up.z());
				this->mSelectionOrigin = this->mSelectedBrush->origin();
			}
		}
	}
	startx = x;
	starty = y;
}
