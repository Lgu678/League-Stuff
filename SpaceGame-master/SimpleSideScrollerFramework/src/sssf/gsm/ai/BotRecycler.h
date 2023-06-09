/**
	This class provides premade lists of commonly used bots to avoid
	dynamic construction during gameplay. 

**/
#pragma once
#include "../../../../stdafx.h"
#include "Bot.h"

static const unsigned int RECYCLABLE_BOT_INCREMENT = 10;

class BotRecycler
{
private:
	map<wstring, list<Bot *> *> recyclableBots;
	map<wstring, Bot *> registeredBotTypes;

public:
	BotRecycler();
	~BotRecycler();
	void addMoreBots(Game *game,list<Bot *> *botsNeededList, wstring botType, unsigned int numToAdd);
	void initRecyclableBots(Game *game,wstring botType, unsigned int maxBotsNeeded);
	bool isRegisteredBotType(wstring botType);
	void recycleBot(wstring botType, Bot* botToRecycle);
	void registerBotType(wstring botType, Bot *sampleBot);
	Bot* retrieveBot(Game *game,wstring botType);

	void reset(GameStateManager *gsm);
	Bot* viewSampleBot(wstring botType);
};