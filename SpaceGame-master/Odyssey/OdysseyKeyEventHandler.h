#pragma once

#include "stdafx.h"
#include "../SimpleSideScrollerFramework/src/sssf/input/KeyEventHandler.h"

const unsigned int UP_KEY = VK_UP;
const unsigned int DOWN_KEY = VK_DOWN;
const unsigned int LEFT_KEY = VK_LEFT;
const unsigned int RIGHT_KEY = VK_RIGHT;

class Game;

class OdysseyKeyEventHandler : public KeyEventHandler
{
public:
	OdysseyKeyEventHandler();
	~OdysseyKeyEventHandler();
	void handleKeyEvents(Game *game);
private:
	int lastGun;
};

