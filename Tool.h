/* 
 * File:   Tool.h
 * Author: wouter
 *
 * Created on May 23, 2011, 11:11 AM
 */

#ifndef TOOL_H
#define	TOOL_H

#include <userInput.h>

class MapViewer;

class Tool
{
public:
	Tool(const char* title);
	virtual ~Tool();
	
	virtual void select() { }
	virtual void deselect() { }

	virtual void prerender(int time) { }
	virtual void render(int time) { }
	virtual void renderHitTest() { }
	virtual void renderMinitature(bool selected) { }
	virtual void renderHitTestMinitature() { }
	
	virtual bool initialize(MapViewer* viewer);
	virtual void onKeyDown(Key::Code key) { }
	virtual void onKeyUp(Key::Code key) { }
	virtual bool onMouseButtonDown(Mouse::Button button) { return false; }
	virtual bool onMouseButtonUp(Mouse::Button button) { return false; }
	virtual bool onMouseMove(int x, int y) { return false; }
	
	const char* title() const;
	
protected:
	MapViewer* mViewer;
	char mTitle[32];
};

#endif	/* TOOL_H */

