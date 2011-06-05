/* 
 * File:   MapViewer.cpp
 * Author: wouter
 * 
 * Created on April 12, 2011, 7:23 PM
 */

#include "MapViewer.h"
#include "Status.h"
#include "AllInOneTool.h"
#include "MorphTool.h"
#include <GLee.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <MapLoader.h>
#include <GameTime.h>
#include <stdio.h>
#include <iostream>

int main()
{
	MapViewer v;
	
	if (v.create(3, 0))
		return v.startGameloop();
	return 0;
}

MapViewer::MapViewer()
	: GlContext(), mSelectedBrush(0), mSelectedTool(0)
{
}

MapViewer::~MapViewer()
{
}

bool MapViewer::onInitializeGl()
{
	geo::MapLoader loader;

	loader.load("dust_001.map", &this->mScene);
	
	this->mTools.push_back(new AllInOneTool());
	this->mTools.push_back(new MorphTool());

	for (std::vector<Tool*>::iterator itr = this->mTools.begin(); itr != this->mTools.end(); ++itr)
		(*itr)->initialize(this);
	
	this->mSelectedTool = this->mTools[0];
	this->mSelectedTool->select();
	
	this->mCamera.rotateX(-90 * 3.14159265f / 180.0f);
	
	this->mStatus.mFont = new ui::Font();
	this->mStatus.mFont->initializeFont("Ubuntu-R.ttf");
	
	return true;
}

void MapViewer::onResize(int w, int h)
{
	if (h == 0) h = 1;
	float aspect = 1.0f * (float(w)/float(h));

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, aspect, 0.1f, 8000.0f);

	glMatrixMode(GL_MODELVIEW);
}

void MapViewer::onIdle(const GameTime* time)
{
	if (this->mSelectedTool != 0)
		this->mSelectedTool->prerender(time->getTotalTime() * 100.0f);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
	glPushMatrix();
	this->mCamera.update();

	float lineColor[] = { -1, 1, 1, 1.0f };
	glEnable(GL_DEPTH_TEST);
	for(std::vector<geo::Entity*>::iterator e = this->mScene.mEntities.begin(); e != this->mScene.mEntities.end(); ++e)
	{
		for (std::vector<geo::Brush*>::iterator b = (*e)->mBrushes.begin(); b != (*e)->mBrushes.end(); ++b)
			this->renderBrush(*b, lineColor);
	}
	
	if (this->mSelectedBrush != 0)
	{
		glDisable(GL_DEPTH_TEST);
		glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
		glLineWidth(1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		for(std::vector<geo::Plane>::iterator p = this->mSelectedBrush->mPlanes.begin(); p != this->mSelectedBrush->mPlanes.end(); ++p)
		{
			glBegin(GL_POLYGON);
			for(std::vector<int>::iterator itr = (*p).mIndices.begin(); itr != (*p).mIndices.end(); ++itr)
				glVertex3fv(this->mSelectedBrush->mVertices[(*itr)]);
			glEnd();
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		this->getScreenPosition(this->mSelectionOrigin, this->mSelectionProjectedOrigin);
	}
	
	if (this->mSelectedTool != 0)
		this->mSelectedTool->render(time->getTotalTime());
	
	glPopMatrix();
	
	glViewport(0, 0, this->width(), this->height());

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0, viewport[2], 0, viewport[3], -10.0f, 10.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	this->mStatus.render(time->getTotalTime());
	if (this->mSelectedTool != 0)
		this->mSelectedTool->render2D(time->getTotalTime());
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void MapViewer::renderBrush(geo::Brush* brush, float lineColor[])
{
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (lineColor[0] > 0)
	{
		glColor3f(lineColor[0], lineColor[1], lineColor[2]);
		for(std::vector<geo::Plane>::iterator p = brush->mPlanes.begin(); p != brush->mPlanes.end(); ++p)
		{
			glBegin(GL_POLYGON);
			for(std::vector<int>::iterator itr = (*p).mIndices.begin(); itr != (*p).mIndices.end(); ++itr)
				glVertex3fv(brush->mVertices[(*itr)]);
			glEnd();
		}
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3ubv(brush->mColor);
	Vector3 v = Vector3(0.0, 1.0, 0.0).unit();
	int i = 1;
	for(std::vector<geo::Plane>::iterator p = brush->mPlanes.begin(); p != brush->mPlanes.end(); ++p)
	{
		glBegin(GL_POLYGON);
		float add = (v.dotProduct((*p).mNormal.unit()) * 5);
		glColor3ub(brush->mColor[0] + add, brush->mColor[1] + add, brush->mColor[2] + add);
		for(std::vector<int>::iterator itr = (*p).mIndices.begin(); itr != (*p).mIndices.end(); ++itr)
			glVertex3fv(brush->mVertices[(*itr)]);
		glEnd();
	}

	glColor3f(1,0,0);
	glBegin(GL_POINTS);
	for(std::vector<Vector3>::iterator itr = brush->mVertices.begin(); itr != brush->mVertices.end(); ++itr)
		glVertex3fv((*itr));
	glEnd();
}

bool MapViewer::getScreenPosition(const Vector3& worldPosition, Vector3& screenPosition)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLdouble winX, winY, winZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	gluProject(
			worldPosition.x(), worldPosition.y(), worldPosition.z(),
			modelview, projection, viewport, 
			(GLdouble*)&winX, (GLdouble*)&winY, (GLdouble*)&winZ
		);
	
	screenPosition.x(winX);
	screenPosition.y(winY);
	screenPosition.z(winZ);
	
	return true;
}

void MapViewer::onKeyDown(Key::Code key)
{
	if (this->mSelectedTool == 0 || this->mSelectedTool->onKeyDown(key) == false)
	{
		if (key == Key::Escape)
		{
			this->stopGameloop();
		}
		else
		{
			for (std::vector<Tool*>::iterator itr = this->mTools.begin(); itr != this->mTools.end(); ++itr)
			{
				if ((*itr)->activatorKey() == key)
				{
					if (this->mSelectedTool != 0)
						this->mSelectedTool->deselect();
					this->mSelectedTool = (*itr);
					this->mSelectedTool->select();
					break;
				}
			}
		}
	}
}

void MapViewer::onKeyUp(Key::Code key)
{
	if (this->mSelectedTool != 0)
		this->mSelectedTool->onKeyUp(key);
}

void MapViewer::onMouseButtonDown(Mouse::Button button)
{
	if (this->mSelectedTool != 0)
		this->mSelectedTool->onMouseButtonDown(button);
}

void MapViewer::onMouseButtonUp(Mouse::Button button)
{
	if (this->mSelectedTool != 0)
		this->mSelectedTool->onMouseButtonUp(button);
}

void MapViewer::onMouseMove(int x, int y)
{
	if (this->mSelectedTool != 0)
		this->mSelectedTool->onMouseMove(x, y);
}

void MapViewer::renderBoundingBox(const float mins[], const float maxs[], float color[])
{
	glActiveTextureARB(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(color[0], color[1], color[2], color[3]);

	glBegin(GL_LINES);
	// bottom
	glVertex3f(mins[0], mins[1], mins[2]);
	glVertex3f(maxs[0], mins[1], mins[2]);

	glVertex3f(maxs[0], mins[1], mins[2]);
	glVertex3f(maxs[0], maxs[1], mins[2]);

	glVertex3f(maxs[0], maxs[1], mins[2]);
	glVertex3f(mins[0], maxs[1], mins[2]);

	glVertex3f(mins[0], maxs[1], mins[2]);
	glVertex3f(mins[0], mins[1], mins[2]);

	// top
	glVertex3f(mins[0], maxs[1], maxs[2]);
	glVertex3f(maxs[0], maxs[1], maxs[2]);

	glVertex3f(maxs[0], maxs[1], maxs[2]);
	glVertex3f(maxs[0], mins[1], maxs[2]);

	glVertex3f(maxs[0], mins[1], maxs[2]);
	glVertex3f(mins[0], mins[1], maxs[2]);

	glVertex3f(mins[0], mins[1], maxs[2]);
	glVertex3f(mins[0], maxs[1], maxs[2]);

	//sides
	glVertex3f(mins[0], maxs[1], mins[2]);
	glVertex3f(mins[0], maxs[1], maxs[2]);

	glVertex3f(maxs[0], maxs[1], mins[2]);
	glVertex3f(maxs[0], maxs[1], maxs[2]);

	glVertex3f(maxs[0], mins[1], mins[2]);
	glVertex3f(maxs[0], mins[1], maxs[2]);

	glVertex3f(mins[0], mins[1], mins[2]);
	glVertex3f(mins[0], mins[1], maxs[2]);
	glEnd();
}
