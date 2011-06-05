/* 
 * File:   Tool.h
 * Author: wouter
 *
 * Created on May 23, 2011, 11:11 AM
 */

#ifndef TOOL_H
#define	TOOL_H

#include <UserInput.h>
#include <Camera.h>
#include <Scene.h>

class MapViewer;

class Tool
{
public:
	Tool(const char* title, Key::Code activatorKey);
	virtual ~Tool();
	
	virtual void select() { }
	virtual void deselect() { }

	virtual void prerender(int time) { }
	virtual void render(int time) { }
	virtual void render2D(int time) { }
	virtual void renderHitTest() { }
	virtual void renderMinitature(bool selected) { }
	virtual void renderHitTestMinitature() { }
	
	virtual bool initialize(MapViewer* viewer);
	virtual bool onKeyDown(Key::Code key) { }
	virtual bool onKeyUp(Key::Code key) { }
	virtual bool onMouseButtonDown(Mouse::Button button) { return false; }
	virtual bool onMouseButtonUp(Mouse::Button button) { return false; }
	virtual bool onMouseMove(int x, int y) { return false; }
	
	const char* title() const;
	Key::Code activatorKey() const;
	
	geo::Brush* selectBrush(int mousex, int mousey);
	bool selectHandle(int mousex, int mousey);
	static void renderGrid(int cellsize, int cellcount, Camera& camera);
	
protected:
	MapViewer* mViewer;
	char mTitle[32];
	Key::Code mActivatorKey;
};

#endif	/* TOOL_H */

