/* 
 * File:   ScaleTool.h
 * Author: wouter
 *
 * Created on May 23, 2011, 11:25 AM
 */

#ifndef SCALETOOL_H
#define	SCALETOOL_H

#include "Tool.h"

class ScaleTool : public Tool
{
public:
	ScaleTool();
	virtual ~ScaleTool();
	
	virtual void render(int time);
	virtual void renderHitTest();
	virtual void renderMinitature(bool selected);
	virtual void renderHitTestMinitature();

private:

};

#endif	/* SCALETOOL_H */

