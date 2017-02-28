/* 
 * File:   MorphTool.cpp
 * Author: wouter
 * 
 * Created on June 4, 2011, 8:51 PM
 */

#include "MorphTool.h"
#include "MapViewer.h"
#include <GL/glextl.h>
#include <iostream>

using namespace std;

MorphTool::MorphTool()
	: Tool("Morph tool", Key::M)
{
}

MorphTool::~MorphTool()
{
}

bool MorphTool::initialize(MapViewer* viewer)
{
	return Tool::initialize(viewer);
}

void MorphTool::select()
{
}

void MorphTool::deselect()
{
}

void MorphTool::prerender(int time)
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

void MorphTool::render(int time)
{
	if (this->mViewer->mSelectedBrush != 0)
	{
		glBegin(GL_POLYGON);
        for(auto vertex : this->mViewer->mSelectedBrush->mVertices)
		{
            this->renderBox(vertex.x, vertex.y, vertex.z, 2.0f);
		}
		glEnd();
	}
}

void MorphTool::render2D(int time)
{
}

bool MorphTool::onMouseButtonDown(Mouse::Button button)
{
	this->mHasMoved = false;
	this->mStartX = this->mPreviousX = MouseState::currentState().x();
	this->mStartY = this->mPreviousY = MouseState::currentState().y();
	
	return true;
}

bool MorphTool::onMouseButtonUp(Mouse::Button button)
{
	return true;
}

#define PI 3.14159265
#define Deg2Rad(Ang) ((float)( Ang * PI / 180.0 ))

bool MorphTool::onMouseMove(int x, int y)
{
	if (MouseState::currentState().isButtonPressed(Mouse::Left))
	{
        this->mViewer->mCamera.rotate(Deg2Rad((this->mPreviousY-y)/10.0f), 0, Deg2Rad((x-this->mPreviousX)/10.0f));
	
		this->mPreviousX = x;
		this->mPreviousY = y;
	}
	return true;
}

void MorphTool::renderBox(float x, float y, float z, float size)
{
	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x-size, y-size, z+size);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+size, y-size, z+size);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+size, y+size, z+size);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x-size, y+size, z+size);	// Top Left Of The Texture and Quad
		// Back Face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x-size, y-size, z-size);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x-size, y+size, z-size);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+size, y+size, z-size);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+size, y-size, z-size);	// Bottom Left Of The Texture and Quad
		// Top Face
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x-size, y+size, z-size);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x-size, y+size, z+size);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+size, y+size, z+size);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+size, y+size, z-size);	// Top Right Of The Texture and Quad
		// Bottom Face
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x-size, y-size, z-size);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+size, y-size, z-size);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+size, y-size, z+size);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x-size, y-size, z+size);	// Bottom Right Of The Texture and Quad
		// Right face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+size, y-size, z-size);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+size, y+size, z-size);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+size, y+size, z+size);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+size, y-size, z+size);	// Bottom Left Of The Texture and Quad
		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x-size, y-size, z-size);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x-size, y-size, z+size);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x-size, y+size, z+size);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x-size, y+size, z-size);	// Top Left Of The Texture and Quad
	glEnd();
}
