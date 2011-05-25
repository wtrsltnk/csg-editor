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
	: Tool("All-in-One tool", Key::B)
{
}

AllInOneTool::~AllInOneTool()
{
}

void AllInOneTool::select()
{
}

void AllInOneTool::deselect()
{
}

void AllInOneTool::prerender(int time)
{
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
	
	this->mViewer->getScreenPosition(this->mViewer->mSelectionOrigin, this->mViewer->mDiskMenu.mPosition);
	this->mViewer->mDiskMenu.render(time);
	
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
	if (button == Mouse::Left)
	{
		if (this->mViewer->selectHandle(MouseState::currentState().getMousePositionX(), MouseState::currentState().getMousePositionY()) && button == Mouse::Left)
		{
		}
	}
	return false;
}

bool AllInOneTool::onMouseButtonUp(Mouse::Button button)
{
	if (button == Mouse::Left)
	{
		if (this->mViewer->selectHandle(MouseState::currentState().getMousePositionX(), MouseState::currentState().getMousePositionY()) && button == Mouse::Left)
		{
		}
	}
	return false;
}

bool AllInOneTool::onMouseMove(int x, int y)
{
	return true;
}
