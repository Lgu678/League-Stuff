#pragma once
#include "../Odyssey/stdafx.h"
#include "../SimpleSideScrollerFramework/src/sssf/data_loader/GameDataLoader.h"
#include "../SimpleSideScrollerFramework/src/sssf/game/Game.h"
#include "../SimpleSideScrollerFramework/src/sssf/gsm/sprite/AnimatedSpriteType.h"
#include "../SimpleSideScrollerFramework/src/sssf/platforms/DirectX/DirectXTextureManager.h"
#include "../LuaPlusFramework/LuaPlus.h"
#include "physicsManager.h"
using namespace LuaPlus;

class OdysseyDataLoader : public GameDataLoader
{

private:
	HINSTANCE hInstance;
	int nCmdShow;
	LuaState* luaPState;

public:
	OdysseyDataLoader();
	~OdysseyDataLoader();

	void initWinHandle(HINSTANCE initHInstance, int initNCmdShow)
	{
		hInstance = initHInstance;
		nCmdShow = initNCmdShow;
	}

	void loadGame(Game *game, wstring gameInitFile);
	void loadGUI(Game *game, wstring guiInitFile);
	void loadWorld(Game *game, wstring levelInitFile);

	// THESE ARE HARD-CODED EXAMPLES OF GUI DATA LOADING
	void hardCodedLoadGUIExample(Game *game);

	// SOME HELPER METHODS FOR SETTING UP THE GAME
	void initColors(GameGraphics *graphics, map<wstring, wstring> *properties);
	void initViewport(GameGUI *gui, map<wstring, wstring> *properties);
	
	// AND SOME HELPER METHODS FOR LOADING THE GUI
	void initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initSplashScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initMainMenu(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	//void initVictoryScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initPauseScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initInGameGUI(Game* game, GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initLevelSelectScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initHelpScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initVictoryScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initDeathScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void loadBotsFromLua(wstring levelName, Game *game);
};

