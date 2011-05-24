/* 
 * File:   CameraTool.cpp
 * Author: wouter
 * 
 * Created on May 23, 2011, 12:15 PM
 */

#include "CameraTool.h"
#include "MapViewer.h"
#include <GL/freeglut.h>

#define PI 3.14159265
#define Deg2Rad(Ang) ((float)( Ang * PI / 180.0 ))

CameraTool::CameraTool()
	: Tool("Camera tool (shift+Z)", Key::Z), mDragging1(false), mDragging2(false)
{
}

CameraTool::~CameraTool()
{
}

void CameraTool::select()
{
	this->mViewer->mStatus.setStatus("Camera tool selected");
}

void CameraTool::deselect()
{
}

void CameraTool::prerender(int time)
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

void CameraTool::render(int time)
{
	if (this->mViewer->mSelectedBrush != 0)
	{
		glPushMatrix();
		glTranslatef(
				this->mViewer->mSelectionOrigin.x(), 
				this->mViewer->mSelectionOrigin.y(), 
				this->mViewer->mSelectionOrigin.z()
				);
		Tool::renderGrid(32, 20, this->mViewer->mCamera);
		glPopMatrix();
	}
}

void CameraTool::renderMinitature(bool selected)
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
	glutWireCone(0.8f, 1.0f, 8, 8);
	glPopMatrix();
}

void CameraTool::renderHitTestMinitature()
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

bool CameraTool::onMouseButtonDown(Mouse::Button button)
{
	if (button == Mouse::Left)
		this->mDragging1 = true;
	if (button == Mouse::Right)
		this->mDragging2 = true;
	this->startx = MouseState::currentState().getMousePositionX();
	this->starty = MouseState::currentState().getMousePositionY();
	
	return true;
}

bool CameraTool::onMouseButtonUp(Mouse::Button button)
{
	this->mDragging1 = false;
	this->mDragging2 = false;
	return true;
}

bool CameraTool::onMouseMove(int x, int y)
{
	if (this->mDragging1)
		this->mViewer->mCamera.rotate(Deg2Rad((this->starty-y)/10.0f), 0, Deg2Rad((x-this->startx)/10.0f));
	if (this->mDragging2)
	{
		// ToDo : dit werkt nog niet zoals het bedoeld is!
		Vector3 pos = this->mViewer->mCamera.position();
		Vector3 diff = this->mViewer->mSelectionOrigin - pos;
		this->mViewer->mCamera.moveForward(diff.length());
		this->mViewer->mCamera.rotate(Deg2Rad((this->starty-y)/10.0f), 0, Deg2Rad((x-this->startx)/10.0f));
		this->mViewer->mCamera.moveForward(-diff.length());
	}
	
	this->startx = x;
	this->starty = y;
	
	return true;
}
	
