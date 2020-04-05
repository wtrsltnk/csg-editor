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
#include <glad/glad.h>
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

int gluProject(float objx, float objy, float objz, float *modelview, float *projection, int *viewport, float *windowCoordinate)
{
    // Transformation vectors
    float fTempo[8];
    // Modelview transform
    fTempo[0]=modelview[0]*objx+modelview[4]*objy+modelview[8]*objz+modelview[12]; // w is always 1
    fTempo[1]=modelview[1]*objx+modelview[5]*objy+modelview[9]*objz+modelview[13];
    fTempo[2]=modelview[2]*objx+modelview[6]*objy+modelview[10]*objz+modelview[14];
    fTempo[3]=modelview[3]*objx+modelview[7]*objy+modelview[11]*objz+modelview[15];
    // Projection transform, the final row of projection matrix is always [0 0 -1 0]
    // so we optimize for that.
    fTempo[4]=projection[0]*fTempo[0]+projection[4]*fTempo[1]+projection[8]*fTempo[2]+projection[12]*fTempo[3];
    fTempo[5]=projection[1]*fTempo[0]+projection[5]*fTempo[1]+projection[9]*fTempo[2]+projection[13]*fTempo[3];
    fTempo[6]=projection[2]*fTempo[0]+projection[6]*fTempo[1]+projection[10]*fTempo[2]+projection[14]*fTempo[3];
    fTempo[7]=-fTempo[2];
    // The result normalizes between -1 and 1
    if(fTempo[7]==0.0) // The w value
     return 0;
    fTempo[7]=1.0/fTempo[7];
    // Perspective division
    fTempo[4]*=fTempo[7];
    fTempo[5]*=fTempo[7];
    fTempo[6]*=fTempo[7];
    // Window coordinates
    // Map x, y to range 0-1
    windowCoordinate[0]=(fTempo[4]*0.5+0.5)*viewport[2]+viewport[0];
    windowCoordinate[1]=(fTempo[5]*0.5+0.5)*viewport[3]+viewport[1];
    // This is only correct when glDepthRange(0.0, 1.0)
    windowCoordinate[2]=(1.0+fTempo[6])*0.5;	// Between 0 and 1
    return 1;
}

void gluPerspective( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar )
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;

    //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fH = tan( fovY / 360 * pi ) * zNear;
    fW = fH * aspect;

    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
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

    this->mCamera.rotate(-90 * 3.14159265f / 180.0f, 0.0f, 0.0f);

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
    for (auto entity : this->mScene.mEntities)
	{
        for (auto brush : entity->mBrushes)
            this->renderBrush(brush, lineColor);
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
            for(auto index : (*p).mIndices)
                glVertex3fv(&(this->mSelectedBrush->mVertices[index][0]));
			glEnd();
		}
        glPolygonMode(GL_FRONT, GL_FILL);
		this->getScreenPosition(this->mSelectionOrigin, this->mSelectionProjectedOrigin);
	}
	
	if (this->mSelectedTool != 0)
		this->mSelectedTool->render(time->getTotalTime());
	
	glPopMatrix();

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
            for(auto index : (*p).mIndices)
                glVertex3fv(&(brush->mVertices[index][0]));
			glEnd();
		}
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3ubv(brush->mColor);
    auto v = glm::normalize(glm::vec3(0.0, 1.0, 0.0));
	int i = 1;
	for(std::vector<geo::Plane>::iterator p = brush->mPlanes.begin(); p != brush->mPlanes.end(); ++p)
	{
		glBegin(GL_POLYGON);
        glm::vec3 n = glm::normalize((*p).mNormal);
        float add = (glm::dot(v, n) * 5.0f);
		glColor3ub(brush->mColor[0] + add, brush->mColor[1] + add, brush->mColor[2] + add);
        for(auto index : (*p).mIndices)
            glVertex3fv(&(brush->mVertices[index][0]));
		glEnd();
	}

	glColor3f(1,0,0);
	glBegin(GL_POINTS);
    for (auto vertex : brush->mVertices)
        glVertex3fv(&(vertex[0]));
	glEnd();
}

bool MapViewer::getScreenPosition(const glm::vec3& worldPosition, glm::vec3& screenPosition)
{
	GLint viewport[4];
	GLfloat modelview[16];
	GLfloat projection[16];
	GLfloat win[3];

	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	glGetFloatv(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	gluProject(
            worldPosition.x, worldPosition.y, worldPosition.z,
			modelview, projection, viewport, 
			win);
	
    screenPosition.x = win[0];
    screenPosition.y = win[1];
    screenPosition.z = win[2];
	
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
