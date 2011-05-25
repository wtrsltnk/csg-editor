/* 
 * File:   AllInOneTool.cpp
 * Author: wouter
 * 
 * Created on May 25, 2011, 12:03 PM
 */

#include "AllInOneTool.h"
#include "MapViewer.h"
#include <GL/freeglut.h>

AllInOneTool::AllInOneTool()
	: Tool("All-in-One tool", Key::B), mHasMoved(false)
{
}

AllInOneTool::~AllInOneTool()
{
}

bool AllInOneTool::initialize(MapViewer* viewer)
{
	this->mDiskMenu.initialize();
	return Tool::initialize(viewer);
}

void AllInOneTool::select()
{
}

void AllInOneTool::deselect()
{
}

void AllInOneTool::prerender(int time)
{
	static int lastTime = 0;
	float speed = 5.0f * ((time - lastTime) / 10.0f);
	lastTime = time;

	if (KeyboardState::currentState().isKeyPressed(Key::w))
		this->mViewer->mCamera.moveForward(speed);
	if (KeyboardState::currentState().isKeyPressed(Key::s))
		this->mViewer->mCamera.moveForward(-speed);
	if (KeyboardState::currentState().isKeyPressed(Key::a))
		this->mViewer->mCamera.moveLeft(speed);
	if (KeyboardState::currentState().isKeyPressed(Key::d))
		this->mViewer->mCamera.moveLeft(-speed);
}

void AllInOneTool::render(int time)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0, viewport[2], 0, viewport[3], -10.0f, 10.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glEnable(GL_TEXTURE_2D);
	this->mDiskMenu.mPosition = this->mViewer->mSelectionProjectedOrigin;
	this->mDiskMenu.render(time);
	glDisable(GL_TEXTURE_2D);
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void AllInOneTool::renderMinitature(bool selected)
{
	glPushMatrix();
	glTranslatef(0, 0, -3);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (selected)
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	else
		glColor4f(1, 1, 1, 0.2f);
	glBegin(GL_QUADS);
	glVertex3f(-1.0f, -1.0f,  0.0f);
	glVertex3f( 1.0f, -1.0f,  0.0f);
	glVertex3f( 1.0f,  1.0f,  0.0f);
	glVertex3f(-1.0f,  1.0f,  0.0f);
	glEnd();
	glDisable(GL_BLEND);
	glColor3f(0.9f, 0.6f, 0);
	glTranslatef(-0.2f, 0, 0);
	glRotatef(75, 0, 1, 0);
	glutWireRhombicDodecahedron();
	glPopMatrix();
}

void AllInOneTool::renderHitTestMinitature()
{
	glPushMatrix();
	glTranslatef(0, 0, -3);
	glBegin(GL_QUADS);
	glVertex3f(-1.0f, -1.0f,  0.0f);
	glVertex3f( 1.0f, -1.0f,  0.0f);
	glVertex3f( 1.0f,  1.0f,  0.0f);
	glVertex3f(-1.0f,  1.0f,  0.0f);
	glEnd();
	glPopMatrix();
}

bool AllInOneTool::onMouseButtonDown(Mouse::Button button)
{
	this->mHasMoved = false;
	this->startx = MouseState::currentState().getMousePositionX();
	this->starty = MouseState::currentState().getMousePositionY();
	
	if (this->mDiskMenu.mHoverType != HoverType::None)
	{
		this->mHasMoved = true;
		this->mHoverType1 = this->mDiskMenu.mHoverType;
	}
	else
	{
		this->mHoverType1 = HoverType::None;
		if (button == Mouse::Left)
			this->mDragging1 = true;
	}
	return false;
}

bool AllInOneTool::onMouseButtonUp(Mouse::Button button)
{
	this->mDragging1 = false;
	this->mHoverType1 = HoverType::None;
	
	if (button == Mouse::Left && this->mHasMoved == false)
	{
		if (this->mViewer->mSelectedBrush != 0)
		{
			geo::Plane* plane = this->mViewer->selectPlane(this->mViewer->mSelectedBrush, MouseState::currentState().getMousePositionX(), MouseState::currentState().getMousePositionY());
			if (plane == 0)
			{
				// Select Brush here
				geo::Brush* b = this->mViewer->selectBrush(MouseState::currentState().getMousePositionX(), MouseState::currentState().getMousePositionY());
				if (b != 0)
				{
					this->mViewer->mSelectedBrush = b;
					this->mViewer->mSelectedPlane = 0;
					this->mViewer->mSelectionOrigin = this->mViewer->mSelectedBrush->origin();
				}
			}
			else
			{
				if (this->mViewer->mSelectedPlane == plane)
				{
					this->mViewer->mSelectedPlane = 0;
					this->mViewer->mSelectionOrigin = this->mViewer->mSelectedBrush->origin();
				}
				else
				{
					this->mViewer->mSelectedPlane = plane;
					this->mViewer->mSelectionOrigin = plane->average;
				}
			}
		}
		else
		{
			// Select Brush here
			geo::Brush* b = this->mViewer->selectBrush(MouseState::currentState().getMousePositionX(), MouseState::currentState().getMousePositionY());
			if (b != 0)
			{
				this->mViewer->mSelectedBrush = b;
				this->mViewer->mSelectedPlane = 0;
				this->mViewer->mSelectionOrigin = this->mViewer->mSelectedBrush->origin();
			}
		}
	}
	return false;
}

#define PI 3.14159265
#define Deg2Rad(Ang) ((float)( Ang * PI / 180.0 ))

bool AllInOneTool::onMouseMove(int x, int y)
{
	this->mHasMoved = true;
	this->mDiskMenu.testHover(x, y);
	
	if (this->mHoverType1 == HoverType::None && MouseState::currentState().isButtonPressed(Mouse::Left))
	{
		this->mViewer->mCamera.rotate(Deg2Rad((this->starty-y)/10.0f), 0, Deg2Rad((x-this->startx)/10.0f));
	}
	else if (this->mHoverType1 == HoverType::Move)
	{
		Vector3 left = this->mViewer->mCamera.left().unit() * (x-startx);
		Vector3 up = this->mViewer->mCamera.up().unit() * (y-starty);
		this->mViewer->mSelectedBrush->move(left.x(), left.y(), left.z());
		this->mViewer->mSelectedBrush->move(up.x(), up.y(), up.z());
		this->mViewer->mSelectionOrigin = this->mViewer->mSelectedBrush->origin();
	}
	else if (this->mHoverType1 == HoverType::Scale)
	{
		float scale = (x - this->startx) / 100.0f;
		this->mViewer->mSelectedBrush->scale(1.0f+scale, 1.0f+scale, 1.0f+scale, this->mViewer->mSelectionOrigin);
	}
	else if (this->mHoverType1 == HoverType::Rotate)
	{
	}
	
	this->startx = x;
	this->starty = y;
	
	return true;
}
