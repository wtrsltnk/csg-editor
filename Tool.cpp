/* 
 * File:   Tool.cpp
 * Author: wouter
 * 
 * Created on May 23, 2011, 11:11 AM
 */

#include "Tool.h"

Tool::Tool()
{
}

Tool::~Tool()
{
}

bool Tool::initialize(MapViewer* viewer)
{
	this->mViewer = viewer;
	
	return true;
}
