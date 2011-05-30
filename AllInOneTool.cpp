/* 
 * File:   AllInOneTool.cpp
 * Author: wouter
 * 
 * Created on May 25, 2011, 12:03 PM
 */

#include "AllInOneTool.h"
#include "MapViewer.h"
#include <TextureLoader.h>
#include <GLee.h>

AllInOneTool::AllInOneTool()
	: Tool("All-in-One tool", Key::B), mHasMoved(false),
		mSize(70), mHoverType(HoverType::None), mColors(0), mMove(0), mScale(0), mRotate(0), mOpacity(0.6f)
{
}

AllInOneTool::~AllInOneTool()
{
}

bool AllInOneTool::initialize(MapViewer* viewer)
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
	float speed = 5.0f * ((time - lastTime));
	lastTime = time;

	if (KeyboardState::currentState().isKeyPressed(Key::W))
		this->mViewer->mCamera.moveForward(speed);
	if (KeyboardState::currentState().isKeyPressed(Key::S))
		this->mViewer->mCamera.moveForward(-speed);
	if (KeyboardState::currentState().isKeyPressed(Key::A))
		this->mViewer->mCamera.moveLeft(speed);
	if (KeyboardState::currentState().isKeyPressed(Key::D))
		this->mViewer->mCamera.moveLeft(-speed);
}

void AllInOneTool::render(int time)
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
		
		if (this->mViewer->mSelectionProjectedOrigin.z() < 1.0f)
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
			this->mPosition = this->mViewer->mSelectionProjectedOrigin;
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
			glDisable(GL_TEXTURE_2D);

			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}
	}
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
//	glutWireRhombicDodecahedron();
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
	this->mStartX = this->mPreviousX = MouseState::currentState().getMousePositionX();
	this->mStartY = this->mPreviousY = MouseState::currentState().getMousePositionY();
	
	if (this->mHoverType != HoverType::None)
	{
		this->mHasMoved = true;
		this->mHoverType1 = this->mHoverType;
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
	
	if (button == Mouse::Left && (Vector3(this->mStartX, this->mStartY, 0)-Vector3(this->mPreviousX, this->mPreviousY, 0)).length() < 5)
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
	this->testHover(x, y);
	
	if (this->mHoverType1 == HoverType::None && MouseState::currentState().isButtonPressed(Mouse::Left))
	{
		this->mViewer->mCamera.rotate(Deg2Rad((this->mPreviousY-y)/10.0f), 0, Deg2Rad((x-this->mPreviousX)/10.0f));
	
		this->mPreviousX = x;
		this->mPreviousY = y;
	}
	else if (this->mHoverType1 == HoverType::Move)
	{
		if (this->mViewer->mSelectedPlane != 0)
		{
			this->mViewer->mSelectedPlane->mDistance += x-mPreviousX;
			this->mViewer->mSelectedBrush->updateVertices();
			this->mViewer->mSelectionOrigin = this->mViewer->mSelectedPlane->average;
		}
		else
		{
			Vector3 left = this->mViewer->mCamera.left().unit() * (x-mPreviousX);
			Vector3 up = this->mViewer->mCamera.up().unit() * (y-mPreviousY);
			this->mViewer->mSelectedBrush->move(left.x(), left.y(), left.z());
			this->mViewer->mSelectedBrush->move(up.x(), up.y(), up.z());
			this->mViewer->mSelectionOrigin = this->mViewer->mSelectedBrush->origin();
		}
		this->mPreviousX = x;
		this->mPreviousY = y;
	}
	else if (this->mHoverType1 == HoverType::Scale)
	{
		if (this->mViewer->mSelectedPlane != 0)
		{
			
		}
		else
		{
			float scale = (x - this->mPreviousX) / 100.0f;
			this->mViewer->mSelectedBrush->scale(1.0f+scale, 1.0f+scale, 1.0f+scale, this->mViewer->mSelectionOrigin);
		}
	
		this->mPreviousX = x;
		this->mPreviousY = y;
	}
	else if (this->mHoverType1 == HoverType::Rotate)
	{
		if (this->mViewer->mSelectedPlane != 0)
		{
			
		}
		else
		{
			
		}
	}
	
	return true;
}

void AllInOneTool::testHover(int mousex, int mousey)
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
