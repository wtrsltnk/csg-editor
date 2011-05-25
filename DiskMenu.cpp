/* 
 * File:   DiskMenu.cpp
 * Author: wouter
 * 
 * Created on May 24, 2011, 7:18 PM
 */

#include "DiskMenu.h"
#include <common/TextureLoader.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

DiskMenu::DiskMenu()
	: mSize(70), mHoverType(HoverType::None), mColors(0), mMove(0), mScale(0), mRotate(0), mOpacity(0.6f)
{
}

DiskMenu::~DiskMenu()
{
}

bool DiskMenu::initialize()
{
	TextureLoader loader;
	
	this->mColors = loader.loadFromTga("colors.tga");
	this->mColors->upload();
	
	this->mScale = loader.loadFromTga("scale.tga");
	this->mScale->upload();
	
	this->mMove = loader.loadFromTga("move.tga");
	this->mMove->upload();
	
	this->mRotate = loader.loadFromTga("rotate.tga");
	this->mRotate->upload();
	
	return true;
}

void DiskMenu::render(int time)
{
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glTranslatef(this->mPosition.x(), this->mPosition.y(), 5);
	
	glColor4f(1.0f, 1.0f, 1.0f, this->mOpacity);
	
	this->mColors->use();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-this->mSize, -this->mSize, 0.0f);
	glTexCoord2f(1, 0); glVertex3f( this->mSize, -this->mSize, 0.0f);
	glTexCoord2f(1, 1); glVertex3f( this->mSize,  this->mSize, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(-this->mSize,  mSize, 0.0f);
	glEnd();
	
	glColor4f(1.0f, 1.0f, 1.0f, this->mOpacity);
	if (this->mHoverType == HoverType::Scale)
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	this->mScale->use();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-this->mSize, -this->mSize, 0.0f);
	glTexCoord2f(1, 0); glVertex3f( this->mSize, -this->mSize, 0.0f);
	glTexCoord2f(1, 1); glVertex3f( this->mSize,  this->mSize, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(-this->mSize,  this->mSize, 0.0f);
	glEnd();
	
	glColor4f(1.0f, 1.0f, 1.0f, this->mOpacity);
	if (this->mHoverType == HoverType::Move)
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	this->mMove->use();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-this->mSize, -this->mSize, 0.0f);
	glTexCoord2f(1, 0); glVertex3f( this->mSize, -this->mSize, 0.0f);
	glTexCoord2f(1, 1); glVertex3f( this->mSize,  this->mSize, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(-this->mSize,  this->mSize, 0.0f);
	glEnd();
	
	glColor4f(1.0f, 1.0f, 1.0f, this->mOpacity);
	if (this->mHoverType == HoverType::Rotate)
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	this->mRotate->use();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-this->mSize, -this->mSize, 0.0f);
	glTexCoord2f(1, 0); glVertex3f( this->mSize, -this->mSize, 0.0f);
	glTexCoord2f(1, 1); glVertex3f( this->mSize,  this->mSize, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(-this->mSize,  this->mSize, 0.0f);
	glEnd();
	
	glPopMatrix();
}

void DiskMenu::testHover(int mousex, int mousey)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glLoadIdentity();
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

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
	glPushMatrix();
	glTranslatef(this->mPosition.x(), this->mPosition.y(), 5);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	
	this->mColors->use();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-this->mSize, -this->mSize, 0.0f);
	glTexCoord2f(1, 0); glVertex3f( this->mSize, -this->mSize, 0.0f);
	glTexCoord2f(1, 1); glVertex3f( this->mSize,  this->mSize, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(-this->mSize,  this->mSize, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	GLubyte pixel[3];
	glReadBuffer(GL_BACK);
	glReadPixels(mousex, mousey, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *)pixel);

	if (pixel[0] >= 200 && pixel[1] == 0 && pixel[2] == 0)
		this->mHoverType = HoverType::Move;
	else if (pixel[1] >= 200 && pixel[0] == 0 && pixel[2] == 0)
		this->mHoverType = HoverType::Scale;
	else if (pixel[2] >= 200 && pixel[1] == 0 && pixel[0] == 0)
		this->mHoverType = HoverType::Rotate;
	else 
		this->mHoverType = HoverType::None;
	
	// Determine opacity
	Vector3 v(this->mPosition.x()-mousex, this->mPosition.y()-mousey, 0);
	int x = viewport[2] - viewport[0];
	int y = viewport[3] - viewport[1];
	int maxviewport = sqrt(x*x+y*y);
	this->mOpacity = 0.6f -  (1.0f / float(maxviewport)) * v.length();
	if (this->mOpacity < 0.1f) this->mOpacity = 0.1f;
}
