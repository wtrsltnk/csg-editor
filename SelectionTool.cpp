/* 
 * File:   SelectionTool.cpp
 * Author: wouter
 * 
 * Created on May 25, 2011, 12:12 PM
 */

#include "SelectionTool.h"
#include "MapViewer.h"
#include <GL/freeglut.h>

SelectionTool::SelectionTool()
	: Tool("Selection Tool", Key::N)
{
}

SelectionTool::~SelectionTool()
{
}

void SelectionTool::renderMinitature(bool selected)
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
	glutWireOctahedron();
	glPopMatrix();
}

void SelectionTool::renderHitTestMinitature()
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

bool SelectionTool::onMouseButtonDown(Mouse::Button button)
{
	if (button == Mouse::Left)
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
	return true;
}

bool SelectionTool::onMouseButtonUp(Mouse::Button button)
{
	return true;
}
