/* 
 * File:   MorphTool.h
 * Author: wouter
 *
 * Created on June 4, 2011, 8:51 PM
 */

#ifndef MORPHTOOL_H
#define	MORPHTOOL_H

#include "Tool.h"

class MorphTool : public Tool
{
public:
	MorphTool();
	virtual ~MorphTool();

	virtual bool initialize(MapViewer* viewer);
	
	virtual void select();
	virtual void deselect();

	virtual void prerender(int time);
	virtual void render(int time);
	virtual void render2D(int time);
	
	virtual bool onMouseButtonDown(Mouse::Button button);
	virtual bool onMouseButtonUp(Mouse::Button button);
	virtual bool onMouseMove(int x, int y);
	
private:
	void renderBox(float x, float y, float z, float size);
	
	bool mHasMoved;
	int mPreviousX, mPreviousY;
	int mStartX, mStartY;

};

#endif	/* MORPHTOOL_H */

