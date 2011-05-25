/* 
 * File:   AllInOneTool.h
 * Author: wouter
 *
 * Created on May 25, 2011, 12:03 PM
 */

#ifndef ALLINONETOOL_H
#define	ALLINONETOOL_H

#include "Tool.h"
#include <common/Texture.h>

namespace HoverType
{
	enum Types
	{
		None = 0,
		Move, 
		Scale, 
		Rotate
	};
}

class AllInOneTool : public Tool
{
public:
	AllInOneTool();
	virtual ~AllInOneTool();

	virtual bool initialize(MapViewer* viewer);
	
	virtual void select();
	virtual void deselect();

	virtual void prerender(int time);
	virtual void render(int time);
	virtual void renderMinitature(bool selected);
	virtual void renderHitTestMinitature();
	
	virtual bool onMouseButtonDown(Mouse::Button button);
	virtual bool onMouseButtonUp(Mouse::Button button);
	virtual bool onMouseMove(int x, int y);
	
	void testHover(int mousex, int mousey);
private:
	bool mHasMoved;
	bool mDragging1;
	int mHoverType1;
	int startx, starty;

	Vector3 mPosition;
	int mSize;
	int mHoverType;
	float mOpacity;
	Texture* mColors;
	Texture* mMove;
	Texture* mScale;
	Texture* mRotate;
};

#endif	/* ALLINONETOOL_H */

