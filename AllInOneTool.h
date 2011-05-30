/* 
 * File:   AllInOneTool.h
 * Author: wouter
 *
 * Created on May 25, 2011, 12:03 PM
 */

#ifndef ALLINONETOOL_H
#define	ALLINONETOOL_H

#include "Tool.h"
#include <Texture.h>

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
	class Config
	{
	public:
		Config() : mUseGrid(true) { }
		
		bool mUseGrid;
		
	};
	static Config sConfig;
public:
	AllInOneTool();
	virtual ~AllInOneTool();

	virtual bool initialize(MapViewer* viewer);
	
	virtual void select();
	virtual void deselect();

	virtual void prerender(int time);
	virtual void render(int time);
	virtual void render2D(int time);
	
	virtual bool onMouseButtonDown(Mouse::Button button);
	virtual bool onMouseButtonUp(Mouse::Button button);
	virtual bool onMouseMove(int x, int y);
	
	void testHover(int mousex, int mousey);
private:
	bool mHasMoved;
	bool mDragging1;
	int mHoverType1;
	int mPreviousX, mPreviousY;
	int mStartX, mStartY;
	Vector3 mInitialPosition;

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

