/* 
 * File:   ScaleTool.cpp
 * Author: wouter
 * 
 * Created on May 23, 2011, 11:25 AM
 */

#include "ScaleTool.h"
#include "MapViewer.h"
#include <GL/freeglut.h>

ScaleTool::ScaleTool()
	: Tool("Scale tool (shift+C)", Key::C)
{
}

ScaleTool::~ScaleTool()
{
}

void ScaleTool::select()
{
	this->mViewer->mStatus.setStatus("Scale tool selected");
}

void ScaleTool::deselect()
{
}

void ScaleTool::render(int time)
{
	glLineWidth(1.0f);
	glPushMatrix();
	glTranslatef(this->mViewer->mSelectionOrigin.x(), this->mViewer->mSelectionOrigin.y(), this->mViewer->mSelectionOrigin.z());
	glScalef(10.0f, 10.0f, 10.0f);
	glutWireSphere(1, 8, 8);
	glPopMatrix();
}

void ScaleTool::renderHitTest()
{
	glPushMatrix();
	glTranslatef(this->mViewer->mSelectionOrigin.x(), this->mViewer->mSelectionOrigin.y(), this->mViewer->mSelectionOrigin.z());
	glScalef(10.0f, 10.0f, 10.0f);
	glutSolidSphere(1, 8, 8);
	glPopMatrix();
}

void ScaleTool::renderMinitature(bool selected)
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
	glRotatef(90, 1, 0, 0);
	glColor3f(1, 0, 0);
	glutWireSphere(0.8f, 8, 8);
	glPopMatrix();
}

void ScaleTool::renderHitTestMinitature()
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
