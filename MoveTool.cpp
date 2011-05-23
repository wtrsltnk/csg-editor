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
	: Tool("Move tool (shift+X)", Key::X), mDragging(false)
{
}

MoveTool::~MoveTool()
{
}

void MoveTool::select()
{
	this->mViewer->mStatus.setStatus("Move tool selected");
}

void MoveTool::deselect()
{
}

void MoveTool::render(int time)
{
	glLineWidth(1.0f);
	
	if (this->mViewer->mSelectedBrush != 0)
	{
		glPushMatrix();
		glTranslatef(this->mInitialPosition.x(), this->mInitialPosition.y(), this->mInitialPosition.z());
		Tool::renderGrid(32, 20, this->mViewer->mCamera);
		glPopMatrix();
	}
	
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

bool MoveTool::onKeyDown(Key::Code key)
{
	if (key == Key::Escape && this->mDragging == true)
	{
		this->mDragging = false;
		Vector3 up = this->mInitialPosition - this->mViewer->mSelectedBrush->origin();
		this->mViewer->mSelectedBrush->move(up.x(), up.y(), up.z());
		this->mViewer->mSelectionOrigin = this->mViewer->mSelectedBrush->origin();
		return true;
	}
	return false;
}

bool MoveTool::onMouseButtonDown(Mouse::Button button)
{
	if (button == Mouse::Left)
	{
		if(this->mDragging == true)
		{
			this->mDragging = false;
			this->mInitialPosition = this->mViewer->mSelectedBrush->origin();
			return true;
		}
		else
		{
			if (this->mViewer->selectHandle(MouseState::currentState().getMousePositionX(), MouseState::currentState().getMousePositionY()) && button == Mouse::Left)
			{
				this->mInitialPosition = this->mViewer->mSelectionOrigin;
				this->mDragging = true;
				this->startx = MouseState::currentState().getMousePositionX();
				this->starty = MouseState::currentState().getMousePositionY();

				return true;
			}
		}
	}
	else if (button == Mouse::Right)
	{
		this->mDragging = false;
		Vector3 up = this->mInitialPosition - this->mViewer->mSelectedBrush->origin();
		this->mViewer->mSelectedBrush->move(up.x(), up.y(), up.z());
		this->mViewer->mSelectionOrigin = this->mViewer->mSelectionOrigin;
	}
	
	return false;
}

bool MoveTool::onMouseButtonUp(Mouse::Button button)
{
	return true;
}

bool MoveTool::onMouseMove(int x, int y)
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
	this->mInitialPosition = this->mViewer->mSelectionOrigin;
	
	return false;
}
