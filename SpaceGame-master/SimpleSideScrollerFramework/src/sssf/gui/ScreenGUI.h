/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	ScreenGUI.h

	This class represents a single screen GUI, like the main
	menu. In a game application, there is always a GUI of some sort,
	whether it is application controls or game controls. So even while
	the game is being played, we can interact with a different GUI.

	This class manages a single GUI, and a game application may have many.
	Each GUI can have many Buttons, many OverlayImages, and a screen name.
*/

#pragma once
#include "../../../stdafx.h"
#include "../game/Game.h"
#include "../graphics/GameGraphics.h"
#include "../graphics/RenderList.h"
#include "../gui/Button.h"
#include "../gui/OverlayImage.h"

class ScreenGUI
{
private:
	// THE BUTTONS FOR THIS SCREEN
	list<Button*>		*buttons;

	// ADDITIONAL IMAGES FOR THIS SCREEN, LIKE BORDERS OR 
	list<OverlayImage*> *overlayImages;

	// SCREEN NAME, LIKE "MAIN MENU"
	wchar_t				*screenName;

	int textIndex;

	int textHigh;
	int textLow;

public:
	// INLINED ACCESSOR METHODS
	int getNumButtons()			{ return buttons->size();		}
	int getNumOverlayImages()	{ return overlayImages->size(); }
	wstring getScreenName()			{ return screenName;			}

	int getTextIndex()			{ return textIndex; }
	int getTextHigh()			{ return textHigh; }
	int getTextLow()			{ return textLow; }

	void setTextIndex(int x)
	{
		textIndex = x;
	}

	void setTextHigh(int x)
	{
		textHigh = x;
	}

	void setTextLow(int x)
	{
		textLow = x;
	}

	// INLINED MUTATOR METHODS
	void setScreenName(wchar_t *initScreenName)
	{
		screenName = initScreenName;
	}

	// METHODS DEFINED IN ScreenGUI.cpp
	ScreenGUI();
	~ScreenGUI();
	void addButton(Button *buttonToAdd);
	void addOverlayImage(OverlayImage *imageToAdd);
	void addRenderItemsToRenderList(RenderList *renderList);
	bool fireButtonCommand(Game *game);
	void registerButtonEventHandler(ButtonEventHandler *eventHandler);
	void updateAllButtons(long mouseX, long mouseY);
};