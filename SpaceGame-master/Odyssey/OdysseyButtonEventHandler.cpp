#include "stdafx.h"
#include "OdysseyButtonEventHandler.h"
#include "Odyssey.h"
#include "src\sssf\game\Game.h"
#include "src\sssf\gsm\state\GameStateManager.h"


OdysseyButtonEventHandler::OdysseyButtonEventHandler()
{
}


OdysseyButtonEventHandler::~OdysseyButtonEventHandler()
{
}

void OdysseyButtonEventHandler::handleButtonEvents(Game *game, wstring command)
{
	if (command.compare(W_LEVEL_1_COMMAND) == 0)
	{
		game->setCurrentLevelFileName(W_LEVEL_1_NAME);
		game->getAudio()->playSound(L"data\\sounds\\soundeff1.wav", false);
		game->getAudio()->stopLoopingAudio();
		game->startGame();
		game->getAudio()->stopAllAudio();
		game->getAudio()->playSound(W_LEVEL_ONE_MUSIC_PATH, true);
	}
	if (command.compare(W_LEVEL_2_COMMAND) == 0)
	{
		game->setCurrentLevelFileName(W_LEVEL_2_NAME);
		game->getAudio()->playSound(L"data\\sounds\\soundeff1.wav", false);
		game->getAudio()->stopLoopingAudio();
		game->startGame();
		game->getAudio()->stopAllAudio();
		game->getAudio()->playSound(W_LEVEL_TWO_MUSIC_PATH, true);
	}
	if (command.compare(W_LEVEL_3_COMMAND) == 0)
	{
		game->setCurrentLevelFileName(W_LEVEL_3_NAME);
		game->getAudio()->playSound(L"data\\sounds\\soundeff1.wav", false);
		game->getAudio()->stopLoopingAudio();
		game->startGame();
		game->getAudio()->stopAllAudio();
		game->getAudio()->playSound(W_LEVEL_TWO_MUSIC_PATH, true);
	}
}
