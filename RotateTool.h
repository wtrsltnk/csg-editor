/* 
 * File:   RotateTool.h
 * Author: wouter
 *
 * Created on May 23, 2011, 11:25 AM
 */

#ifndef ROTATETOOL_H
#define	ROTATETOOL_H

#include "Tool.h"

class RotateTool : public Tool
{
public:
	RotateTool();
	virtual ~RotateTool();
	
	virtual void render(int time);
	virtual void renderHitTest();
	virtual void renderMinitature(bool selected);
	virtual void renderHitTestMinitature();

private:

};

#endif	/* ROTATETOOL_H */

