/* 
 * File:   AllInOneTool.h
 * Author: wouter
 *
 * Created on May 25, 2011, 12:03 PM
 */

#ifndef ALLINONETOOL_H
#define	ALLINONETOOL_H

#include "Tool.h"

class AllInOneTool : public Tool
{
public:
	AllInOneTool();
	virtual ~AllInOneTool();

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

};

#endif	/* ALLINONETOOL_H */

