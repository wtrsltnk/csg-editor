/* 
 * File:   MoveTool.h
 * Author: wouter
 *
 * Created on May 23, 2011, 11:14 AM
 */

#ifndef MOVETOOL_H
#define	MOVETOOL_H

#include "Tool.h"

class MoveTool : public Tool
{
public:
	MoveTool();
	virtual ~MoveTool();
	
	virtual void select();
	virtual void deselect();

	virtual void render(int time);
	virtual void renderHitTest();
	virtual void renderMinitature(bool selected);
	virtual void renderHitTestMinitature();
	
	virtual bool onMouseButtonDown(Mouse::Button button);
	virtual bool onMouseButtonUp(Mouse::Button button);
	virtual bool onMouseMove(int x, int y);

private:
	bool mDragging;
	int startx, starty;

};

#endif	/* MOVETOOL_H */

