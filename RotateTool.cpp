/* 
 * File:   RotateTool.cpp
 * Author: wouter
 * 
 * Created on May 23, 2011, 11:25 AM
 */

#include "RotateTool.h"
#include "MapViewer.h"
#include <GL/freeglut.h>

RotateTool::RotateTool()
	: Tool("Rotate tool (shift+V)", Key::V)
{
}

RotateTool::~RotateTool()
{
}

void RotateTool::select()
{
	this->mViewer->mStatus.setStatus("Rotate tool selected");
}

void RotateTool::deselect()
{
}

void RotateTool::render(int time)
{
	glLineWidth(1.0f);
	glPushMatrix();
	glTranslatef(this->mViewer->mSelectionOrigin.x(), this->mViewer->mSelectionOrigin.y(), this->mViewer->mSelectionOrigin.z());
	glScalef(10.0f, 10.0f, 10.0f);
//	glutWireTeapot(0.6f);
	glPopMatrix();
}

void RotateTool::renderHitTest()
{
	glPushMatrix();
	glTranslatef(this->mViewer->mSelectionOrigin.x(), this->mViewer->mSelectionOrigin.y(), this->mViewer->mSelectionOrigin.z());
	glScalef(10.0f, 10.0f, 10.0f);
//	glutSolidTeapot(0.6f);
	glPopMatrix();
}

void RotateTool::renderMinitature(bool selected)
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
	glColor3f(0, 0, 1.0f);
//	glutWireTeapot(0.6f);
	glPopMatrix();
}

void RotateTool::renderHitTestMinitature()
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
