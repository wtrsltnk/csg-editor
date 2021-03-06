/* 
 * File:   Status.h
 * Author: wouter
 *
 * Created on May 23, 2011, 2:40 PM
 */

#ifndef STATUS_H
#define	STATUS_H

#include <uiFont.h>

class StatusView
{
public:
	StatusView();
	virtual ~StatusView();

	bool isStatusVisible();
	void setStatus(const char* text, int duration = 2000);
	void render(int time);
	
	ui::Font* mFont;
private:
	int mStart;
	int mDuration;
	int mLastTime;
	char* mText;

};

#endif	/* STATUS_H */

