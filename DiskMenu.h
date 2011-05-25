/* 
 * File:   DiskMenu.h
 * Author: wouter
 *
 * Created on May 24, 2011, 7:18 PM
 */

#ifndef DISKMENU_H
#define	DISKMENU_H

#include <common/vector3.h>
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

class DiskMenu
{
public:
	DiskMenu();
	virtual ~DiskMenu();

	bool initialize();
	
	void render(int time);
	void testHover(int mousex, int mousey);
	
	Vector3 mPosition;
	int mSize;
	int mHover;
	Texture* mColors;
	Texture* mMove;
	Texture* mScale;
	Texture* mRotate;
};

#endif	/* DISKMENU_H */

