/* 
 * File:   CameraTool.h
 * Author: wouter
 *
 * Created on May 23, 2011, 12:15 PM
 */

#ifndef CAMERATOOL_H
#define	CAMERATOOL_H

#include "Tool.h"

class CameraTool : public Tool
{
public:
	CameraTool();
	virtual ~CameraTool();

	virtual void select();
	virtual void deselect();

	virtual void prerender(int time);
	virtual void render(int time);
	virtual void renderMinitature(bool selected);
	virtual void renderHitTestMinitature();
	
	virtual bool onMouseButtonDown(Mouse::Button button);
	virtual bool onMouseButtonUp(Mouse::Button button);
	virtual bool onMouseMove(int x, int y);
	
private:
	bool mDragging1;
	bool mDragging2;
	int startx, starty;
};

#endif	/* CAMERATOOL_H */

