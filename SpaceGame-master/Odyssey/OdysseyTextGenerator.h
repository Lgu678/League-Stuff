#pragma once
#include "stdafx.h"
#include "../SimpleSideScrollerFramework/src/sssf/text/TextGenerator.h"


class OdysseyTextGenerator : public TextGenerator
{
public:
	wstring textToGenerate;

	OdysseyTextGenerator();
	~OdysseyTextGenerator();
	void initText(Game *game);
	void updateText(Game *game);
	void appendMouseCoords(Game *game);
	void appendExtraData(Game *game);
	
	void appendStats(Game *game);
	void appendObjectives(Game* game);
};

