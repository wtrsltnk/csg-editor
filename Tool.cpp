/* 
 * File:   Tool.cpp
 * Author: wouter
 * 
 * Created on May 23, 2011, 11:11 AM
 */

#include "Tool.h"

Tool::Tool(const char* title)
{
	int i = 0;
	while (title[i] != '\0')
		this->mTitle[i] = title[i++];
	this->mTitle[i] = title[i];
}

Tool::~Tool()
{
}

bool Tool::initialize(MapViewer* viewer)
{
	this->mViewer = viewer;
	
	return true;
}

const char* Tool::title() const
{
	return this->mTitle;
}
