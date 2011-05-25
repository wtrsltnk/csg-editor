/* 
 * File:   SelectionTool.h
 * Author: wouter
 *
 * Created on May 25, 2011, 12:12 PM
 */

#ifndef SELECTIONTOOL_H
#define	SELECTIONTOOL_H

#include "Tool.h"

class SelectionTool : public Tool
{
public:
	SelectionTool();
	virtual ~SelectionTool();

	virtual void renderMinitature(bool selected);
	virtual void renderHitTestMinitature();
	
	virtual bool onMouseButtonDown(Mouse::Button button);
	virtual bool onMouseButtonUp(Mouse::Button button);
	
private:

};

#endif	/* SELECTIONTOOL_H */

