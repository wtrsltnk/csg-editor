/* 
 * File:   MoveTool.cpp
 * Author: wouter
 * 
 * Created on May 23, 2011, 11:14 AM
 */

#include "MoveTool.h"
#include "MapViewer.h"
#include <GL/freeglut.h>

MoveTool::MoveTool()
	: mDragging(false)
{
}

MoveTool::~MoveTool()
{
}

void MoveTool::render(int time)
{
	glLineWidth(1.0f);
	glPushMatrix();
	glTranslatef(this->mViewer->mSelectionOrigin.x(), this->mViewer->mSelectionOrigin.y(), this->mViewer->mSelectionOrigin.z());
	glScalef(10.0f, 10.0f, 10.0f);
	glutWireCube(1);
	glPopMatrix();
}

void MoveTool::renderHitTest()
{
	glPushMatrix();
	glTranslatef(this->mViewer->mSelectionOrigin.x(), this->mViewer->mSelectionOrigin.y(), this->mViewer->mSelectionOrigin.z());
	glScalef(10.0f, 10.0f, 10.0f);
	glutSolidCube(1);
	glPopMatrix();
}

void MoveTool::renderMinitature(bool selected)
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
	glColor3f(0, 1, 0);
	glutWireCube(1.2);
	glPopMatrix();
}

void MoveTool::renderHitTestMinitature()
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


bool MoveTool::onMouseButtonDown(Mouse::Button button)
{
	if (button == Mouse::Left)
	{
		if (this->mViewer->selectHandle(MouseState::currentState().getMousePositionX(), MouseState::currentState().getMousePositionY()))
		{
			this->mDragging = true;
			this->startx = MouseState::currentState().getMousePositionX();
			this->starty = MouseState::currentState().getMousePositionY();

			return true;
		}
	}
	
	return false;
}

bool MoveTool::onMouseButtonUp(Mouse::Button button)
{
	this->mDragging = false;
	
	return true;
}

bool MoveTool::onMouseMove(int x, int y)
{
	if (MouseState::currentState().isButtonPressed(Mouse::Left))
	{
		if (this->mDragging && this->mViewer->mSelectedBrush != 0)
		{
			if (this->mViewer->mSelectedPlane != 0)
			{
				// dragg plane
			}
			else
			{
				// dragg brush
				Vector3 left = this->mViewer->mCamera.left().unit() * (x-startx);
				Vector3 up = this->mViewer->mCamera.up().unit() * (y-starty);
				this->mViewer->mSelectedBrush->move(left.x(), left.y(), left.z());
				this->mViewer->mSelectedBrush->move(up.x(), up.y(), up.z());
				this->mViewer->mSelectionOrigin = this->mViewer->mSelectedBrush->origin();

				this->startx = x;
				this->starty = y;
				return true;
			}
		}
	}
	
	return false;
}
