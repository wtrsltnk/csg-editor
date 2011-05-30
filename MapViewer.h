/* 
 * File:   MapViewer.h
 * Author: wouter
 *
 * Created on April 12, 2011, 7:23 PM
 */

#ifndef MAPVIEWER_H
#define	MAPVIEWER_H

#include "Tool.h"
#include "Status.h"
#include <GlContext.h>
#include <Camera.h>
#include <Scene.h>

namespace EditMode
{
	enum Modes
	{
		None = 0,
		Move,
		Scale,
		Rotate
	};
}

class MapViewer : public GlContext
{
public:
	MapViewer();
	virtual ~MapViewer();

	virtual bool onInitializeGl();
	virtual void onDestroyGl() { }
	virtual void onResize(int w, int h);
	virtual void onKeyDown(Key::Code key);
	virtual void onKeyUp(Key::Code key);
	virtual void onMouseButtonDown(Mouse::Button button);
	virtual void onMouseButtonUp(Mouse::Button button);
	virtual void onMouseMove(int x, int y);
	virtual void onIdle(const GameTime* time);

	void renderBrush(geo::Brush* brush, float lineColor[]);
	void renderBoundingBox(const float mins[], const float maxs[], float color[]);
	geo::Brush* selectBrush(int mousex, int mousey);
	geo::Plane* selectPlane(geo::Brush* brush, int mousex, int mousey);
	bool selectHandle(int mousex, int mousey);
	Tool* testMenu(int mousex, int mousey);
	bool getScreenPosition(const Vector3& worldPosition, Vector3& screenPosition);

	geo::Scene mScene;
	geo::Brush* mSelectedBrush;
	geo::Plane* mSelectedPlane;
	Vector3 mSelectionOrigin;
	Vector3 mSelectionProjectedOrigin;
	Camera mCamera;
	std::vector<Tool*> mTools;
	Tool* mSelectedTool;
	StatusView mStatus;
	bool mMenuFocus;

};

#endif	/* MAPVIEWER_H */

