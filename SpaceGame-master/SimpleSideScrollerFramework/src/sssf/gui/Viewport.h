/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	Viewport.h

	This class stores information about the viewport to help
	with rendering. The viewport may move around the game world
	by calling 
	
	, which changes viewportX and 
	viewportY. It also has information about the size of the 
	viewport. Note that the viewport width and height refer
	to the full rendering area of the game, including any GUI
	regions. To compute the size of the game rendering area,
	which might not include GUI toolbars, for example, one may
	subtract the viewport offsets from the width and height
	values.
*/
#pragma once
#include "../../../stdafx.h"

class Viewport
{
private:
	float scrollSpeedX;
	float scrollSpeedY;
	float viewportX;
	float viewportY;
	float viewportWidth;
	float viewportHeight;
	int viewportOffsetX;
	int viewportOffsetY;
	bool viewportToggled;
	int toggleOffsetY;
	int normalOffsetY;

public:
	// INLINED ACCESSOR METHODS
	float getScrollSpeedX()		{ return scrollSpeedX; }
	float getScrollSpeedY()		{ return scrollSpeedY;			}
	float getViewportX()			{ return viewportX; }
	float getViewportY()			{ return viewportY; }
	int getViewportWidth()		{ return viewportWidth;			}
	int getViewportHeight()		{ return viewportHeight;		}
	int getViewportOffsetX()	{ return viewportOffsetX;		}
	int getViewportOffsetY()	{ return viewportOffsetY;		}

	// INLINED MUTATOR METHODS
	void setScrollSpeedX(float initScrollSpeedX)
	{ scrollSpeedX = initScrollSpeedX;		}
	void setScrollSpeedY(float initScrollSpeedY)
	{ scrollSpeedY = initScrollSpeedY;		}
	void setViewportX(int initViewportX)				
	{ viewportX = initViewportX;			}
	void setViewportY(int initViewportY)				
	{ viewportY = initViewportY;			}
	void setViewportWidth(int initViewportWidth)
	{ viewportWidth = initViewportWidth;	}
	void setViewportHeight(int initViewportHeight)
	{ viewportHeight = initViewportHeight;	}
	void setViewportOffsetX(int initViewportOffsetX)	
	{ viewportOffsetX = initViewportOffsetX;}
	void setViewportOffsetY(int initViewportOffsetY)	
	{ viewportOffsetY = initViewportOffsetY;
	  normalOffsetY = viewportOffsetY;
	}
	void setToggleOffsetY(int initToggleOffsetY)
	{ toggleOffsetY = initToggleOffsetY; }

	// METHODS DEFINED IN Viewport.cpp
	Viewport();
	~Viewport();
	bool areWorldCoordinatesInViewport(float x, float y, int width, int height);
	bool areViewportCoordinatesInViewport(int x, int y, int width, int height);
	bool areScreenCoordinatesInViewport(int x, int y);
	void moveViewport(float incX, float incY, int worldWidth, int worldHeight);
	void toggleDebugView();
};