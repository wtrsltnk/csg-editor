/* 
 * File:   Status.cpp
 * Author: wouter
 * 
 * Created on May 23, 2011, 2:40 PM
 */

#include "Status.h"
#include <glad/glad.h>

StatusView::StatusView()
	: mFont(0), mStart(0), mDuration(0), mLastTime(0), mText(0)
{
}

StatusView::~StatusView()
{
	if (this->mText != 0)
		delete []this->mText;
}

bool StatusView::isStatusVisible()
{
	return (this->mStart + this->mDuration > this->mLastTime);
}

void StatusView::setStatus(const char* text, int duration)
{
	if (this->mText != 0)
		delete []this->mText;
	this->mText = new char[strlen(text) + 1];
	strcpy(this->mText, text);
	this->mStart = this->mLastTime;
	this->mDuration = duration;
}

void StatusView::render(int time)
{
	if (this->mStart + this->mDuration > time || this->mDuration == -1)
	{
		glActiveTexture(GL_TEXTURE0);
		
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		
		glDisable(GL_DEPTH_TEST);
		
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		this->mFont->drawText(10, 10, this->mText, ui::RGBA(255, 255, 255, 255));
		
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		
		glEnable(GL_DEPTH_TEST);
	}
	this->mLastTime = time;
}
