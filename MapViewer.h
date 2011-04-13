/* 
 * File:   MapViewer.h
 * Author: wouter
 *
 * Created on April 12, 2011, 7:23 PM
 */

#ifndef MAPVIEWER_H
#define	MAPVIEWER_H

#include <GlutApplication.h>
#include <common/camera.h>
#include <geo/Scene.h>

class MapViewer : public GlutApplication
{
public:
	MapViewer();
	virtual ~MapViewer();

	virtual bool initialize(int argc, char* argv[]);
	virtual void resize(int w, int h);
	virtual void onKeyDown(Key::Code key);
	virtual void onKeyUp(Key::Code key) { }
	virtual void onMouseButtonDown(Mouse::Button button);
	virtual void onMouseButtonUp(Mouse::Button button) { }
	virtual void onMouseMove(int x, int y);
	virtual void render(int time);

	void renderBrush(geo::Brush* brush);
	
private:
	geo::Scene mScene;
	Camera mCamera;

};

#endif	/* MAPVIEWER_H */

