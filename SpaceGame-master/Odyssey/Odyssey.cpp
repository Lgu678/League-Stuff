// Odyssey.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Odyssey.h"

#include "OdysseyButtonEventHandler.h"
#include "OdysseyDataLoader.h"
#include "OdysseyKeyEventHandler.h"
#include "OdysseyTextGenerator.h"

// GAME OBJECT INCLUDES
#include "../SimpleSideScrollerFramework/src/sssf/game/Game.h"
#include "../SimpleSideScrollerFramework/src/sssf/graphics/GameGraphics.h"
#include "../SimpleSideScrollerFramework/src/sssf/gsm/state/GameState.h"
#include "../SimpleSideScrollerFramework/src/sssf/gui/Cursor.h"
#include "../SimpleSideScrollerFramework/src/sssf/gui/GameGUI.h"
#include "../SimpleSideScrollerFramework/src/sssf/gui/ScreenGUI.h"
#include "../SimpleSideScrollerFramework/src/sssf/input/GameInput.h"
#include "../SimpleSideScrollerFramework/src/sssf/os/GameOS.h"
#include "../SimpleSideScrollerFramework/src/sssf/text/GameText.h"

// WINDOWS PLATFORM INCLUDES
#include "../SimpleSideScrollerFramework/src/sssf/platforms/Windows/WindowsOS.h"
#include "../SimpleSideScrollerFramework/src/sssf/platforms/Windows/WindowsInput.h"
#include "../SimpleSideScrollerFramework/src/sssf/platforms/Windows/WindowsTimer.h"

// DIRECTX INCLUDES
#include "../SimpleSideScrollerFramework/src/sssf/platforms/DirectX\DirectXGraphics.h"
#include "../SimpleSideScrollerFramework/src/sssf/platforms/DirectX\DirectXTextureManager.h"

#include "../LuaPlusFramework/LuaPlus.h"

#define MAX_LOADSTRING 100

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	Game *odysseyGame = new Game();

	// FIRST WE'LL SETUP THE DATA LOADER, SINCE IT MAY NEED TO READ
	// IN DATA TO SETUP OTHER STUFF
	OdysseyDataLoader *odysseyDataLoader = new OdysseyDataLoader();
	odysseyDataLoader->initWinHandle(hInstance, nCmdShow);
	odysseyGame->setDataLoader(odysseyDataLoader);
	odysseyDataLoader->loadGame(odysseyGame, W_INIT_FILE);

	// WHAT WE SHOULD BE DOING HERE IS LOADING THE GAME DATA FROM FILES. THIS
	// MEANS THE GUIS THEMSELVES AS WELL AS THE LEVELS. THAT WILL BE LEFT
	// FOR BECHMARK HWS. FOR NOW WE WILL JUST HARD CODE THE LOADING OF THE GUI

	// LOAD THE GUI STUFF, AGAIN, NOTE THAT THIS SHOULD REALLY
	// BE DONE FROM A FILE, NOT HARD CODED
	odysseyDataLoader->loadGUI(odysseyGame, L"");

	// SPECIFY WHO WILL HANDLE BUTTON EVENTS
	OdysseyButtonEventHandler *odysseyButtonHandler = new OdysseyButtonEventHandler();
	GameGUI *gui = odysseyGame->getGUI();
	gui->registerButtonEventHandler((ButtonEventHandler*)odysseyButtonHandler);

	// SPECIFY WHO WILL HANDLE KEY EVENTS
	OdysseyKeyEventHandler *odysseyKeyHandler = new OdysseyKeyEventHandler();
	odysseyGame->getInput()->registerKeyHandler((KeyEventHandler*)odysseyKeyHandler);
	//TEST


	// START THE GAME LOOP
	odysseyGame->runGameLoop();

	// GAME'S OVER SHUTDOWN ALL THE STUFF WE CONSTRUCTED HERE
	delete (WindowsOS*)odysseyGame->getOS();
	delete (WindowsInput*)odysseyGame->getInput();
	delete (WindowsTimer*)odysseyGame->getTimer();
	delete (DirectXGraphics*)odysseyGame->getGraphics();
	delete (OdysseyTextGenerator*)odysseyGame->getText()->getTextGenerator();
	delete odysseyButtonHandler;
	delete odysseyKeyHandler;
	delete odysseyGame;
	//test comment
	// AND RETURN
	
	return 0;
}