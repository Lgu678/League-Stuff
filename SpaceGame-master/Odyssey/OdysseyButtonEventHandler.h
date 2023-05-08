#pragma once
#include "stdafx.h"
#include "../SimpleSideScrollerFramework/src/sssf/input/ButtonEventHandler.h"



class Game;

class OdysseyButtonEventHandler : public ButtonEventHandler
{
public:
	OdysseyButtonEventHandler();
	~OdysseyButtonEventHandler();

	void handleButtonEvents(Game *game,
		wstring command);

};

