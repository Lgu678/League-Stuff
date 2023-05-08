#include "stdafx.h"
#include "OdysseyKeyEventHandler.h"
#include "../SimpleSideScrollerFramework/src/sssf/game/Game.h"
#include "../SimpleSideScrollerFramework/src/sssf/input/KeyEventHandler.h"
#include "../SimpleSideScrollerFramework/src/sssf/input/GameInput.h"
#include "../SimpleSideScrollerFramework/src/sssf/gui/GameGUI.h"
#include "../SimpleSideScrollerFramework/src/sssf/gui/ScreenGUI.h"
#include "../SimpleSideScrollerFramework/src/sssf/gui/SplashScreenGUI.h"
#include "../SimpleSideScrollerFramework/src/sssf/timer/GameTimer.h"
#include "Odyssey.h"
#include "../Odyssey/OdysseyTextGenerator.h"
#include "Laser.h"


static const int SELECTOR_LOWER_BOUND = 120;
static const int SELECTOR_UPPER_BOUND = 320;


OdysseyKeyEventHandler::OdysseyKeyEventHandler(){}


OdysseyKeyEventHandler::~OdysseyKeyEventHandler(){}


void OdysseyKeyEventHandler::handleKeyEvents(Game *game)
{
	GameInput* input = game->getInput();
	GameTimer* timer = game->getTimer();
	SplashScreenGUI* splash = (SplashScreenGUI*)(game->getGUI()->getScreen(GS_SPLASH_SCREEN));


	if (input->isKeyDown(VK_CONTROL) && input->isKeyDownForFirstTime('H'))
	{
		if (game->getCurrentLevelFileName().empty() == true)
		{
			
		}
		else
		{
			game->getGSM()->getSpriteManager()->getPlayer()->setPlayerHealth(200);
			game->getGSM()->getSpriteManager()->getPlayer()->setPlayerLives(10);
		}
	}

	if (input->isKeyDown(VK_CONTROL) && input->isKeyDownForFirstTime('1'))
	{
		if (game->getCurrentLevelFileName().empty() == true)
		{
			game->setCurrentLevelFileName(W_LEVEL_1_NAME);
			game->startGame();
		}
		else
		{
			game->quitGame();
			game->setCurrentLevelFileName(W_LEVEL_1_NAME);
			game->startGame();
		}
	}

	if (input->isKeyDown(VK_CONTROL) && input->isKeyDownForFirstTime('2'))
	{
		if (game->getCurrentLevelFileName().empty() == true)
		{
			game->setCurrentLevelFileName(W_LEVEL_2_NAME);
			game->startGame();
			game->getAudio()->stopAllAudio();
			game->getAudio()->playSound(W_LEVEL_ONE_MUSIC_PATH, true);
		}
		else
		{
			game->quitGame();
			game->setCurrentLevelFileName(W_LEVEL_2_NAME);
			game->startGame();
			game->getAudio()->stopAllAudio();
			game->getAudio()->playSound(W_LEVEL_TWO_MUSIC_PATH, true);
		}
	}

	if (input->isKeyDownForFirstTime(VK_ESCAPE) && (game->getGSM()->getCurrentGameState() == GS_MENU_CONTROLS_MENU))
	{
		game->getGSM()->goToMainMenu();
	}
	if (game->getGSM()->getCurrentGameState() == GS_PAUSED)
	{
		if (input->isKeyDownForFirstTime(VK_ESCAPE))
			game->getGSM()->goToGame();

		if (input->isKeyDownForFirstTime(VK_UP))
		{
			if (game->getGUI()->getScreen(GS_PAUSED)->getTextIndex() == game->getText()->getTextLow())
			{
			}
			else
			{
				game->getGUI()->getScreen(GS_PAUSED)->setTextIndex(game->getGUI()->getScreen(GS_PAUSED)->getTextIndex() - 1);
			}

		}

		if (input->isKeyDownForFirstTime(VK_DOWN))
		{
			if (game->getGUI()->getScreen(GS_PAUSED)->getTextIndex() == game->getText()->getTextHigh())
			{
			}
			else
			{
				game->getGUI()->getScreen(GS_PAUSED)->setTextIndex(game->getGUI()->getScreen(GS_PAUSED)->getTextIndex() + 1);
			}
		}
		game->getText()->setTextIndex(game->getGUI()->getScreen(GS_PAUSED)->getTextIndex());

		if (input->isKeyDownForFirstTime(VK_RETURN))
		{
			int selected = game->getText()->getTextIndex();


			if (selected == 6)
			{
				// CONTINUE
				game->getGSM()->goToGame();
			}
			if (selected == 7)
			{

				ifstream checkFile(SAVE_PATH);
				if (checkFile) // IS A GAME IN PRORGESS?
				{
					game->getText()->setTextLow(1);
					game->getText()->setTextHigh(4);
					game->getText()->setTextIndex(1);
					//game->getGSM()->goToMainMenu();
				}
				else
				{
					game->getText()->setTextLow(2);
					game->getText()->setTextHigh(4);
					game->getText()->setTextIndex(2);
					game->getGUI()->getScreen(GS_MAIN_MENU)->setTextIndex(2);
					//game->getGSM()->goToMainMenu();
				}
				checkFile.close();

				game->quitGame();
				return;

			}

		}

		



	}
	else if (game->getGSM()->getCurrentGameState() == GS_GAME_IN_PROGRESS)
	{
		//if (input->isKeyDownForFirstTime(VK_F12))
		//{
		//	BulletRecycler *bulletRecycler = game->getGSM()->getSpriteManager()->getBulletRecycler();
		//	(bulletRecycler->retrieveBullet(game, L"LASER"))->setPrimaryDamage(1000);
		//}

		if (input->isKeyDown(VK_CONTROL) && input->isKeyDownForFirstTime('I'))
		{
			game->getGSM()->getSpriteManager()->getPlayer()->setDamageImune(true);
			game->getGSM()->getSpriteManager()->getPlayer()->setImmuneTime(10000);
		}

		if (input->isKeyDownForFirstTime(VK_ESCAPE))
		{
			game->getText()->setTextLow(6);
			game->getText()->setTextHigh(7);
			game->getText()->setTextIndex(6);
			game->getGSM()->goToPauseScreen();

		}
	}

	if (game->getGSM()->getCurrentGameState() == GS_LEVEL_SELECT)
	{

		if (input->isKeyDownForFirstTime(VK_ESCAPE))
		{
			game->getGUI()->getScreen(GS_MAIN_MENU)->setTextIndex(1); // HARD CODED ITS LAZY BUT WORKS BECAUSE WE ONLY HAVE A FEW MAIN MENU OPTIONS


			ifstream checkFile(SAVE_PATH);
			if (checkFile) // IS A GAME IN PRORGESS?
			{
				game->getText()->setTextLow(1);
				game->getText()->setTextHigh(4);
				game->getText()->setTextIndex(1);
				game->getGSM()->goToMainMenu();
			}
			else
			{
				game->getText()->setTextLow(2);
				game->getText()->setTextHigh(4);
				game->getText()->setTextIndex(2);
				game->getGUI()->getScreen(GS_MAIN_MENU)->setTextIndex(2);
				game->getGSM()->goToMainMenu();
			}
			checkFile.close();
		}
		
	}

	if (game->getGSM()->getCurrentGameState() == GS_MAIN_MENU)
	{
		if (input->isKeyDownForFirstTime(VK_UP))
		{
			if (game->getGUI()->getScreen(GS_MAIN_MENU)->getTextIndex() == game->getText()->getTextLow())
			{
			}
			else
			{
				game->getGUI()->getScreen(GS_MAIN_MENU)->setTextIndex(game->getGUI()->getScreen(GS_MAIN_MENU)->getTextIndex() - 1);
			}

		}
		
		if (input->isKeyDownForFirstTime(VK_DOWN))
		{
			if (game->getGUI()->getScreen(GS_MAIN_MENU)->getTextIndex() == game->getText()->getTextHigh())
			{
			}
			else
			{
				game->getGUI()->getScreen(GS_MAIN_MENU)->setTextIndex(game->getGUI()->getScreen(GS_MAIN_MENU)->getTextIndex() + 1);
			}
		}

		game->getText()->setTextIndex(game->getGUI()->getScreen(GS_MAIN_MENU)->getTextIndex());


		if (input->isKeyDownForFirstTime(VK_RETURN))
		{
			int selected = game->getText()->getTextIndex();

			if (selected == 1)
			{
				// CONTINUE
				game->getGSM()->goToLevelSelectScreen();
			}
			if (selected == 2)
			{
				// NEW GAME
				game->getGSM()->goToLevelSelectScreen();
			}
			if (selected == 3)
			{
				//CONTROLS
				game->getGSM()->goToHelpScreen();
			}
			if (selected == 4)
			{
				//QUIT
				game->shutdown();
			}
		}
	}

	if (game->getGSM()->getCurrentGameState() == GS_SPLASH_SCREEN)
	{

		if (input->isKeyDownForFirstTime(VK_RETURN))
		{
			game->getGUI()->getScreen(GS_MAIN_MENU)->setTextIndex(1); // HARD CODED ITS LAZY BUT WORKS BECAUSE WE ONLY HAVE A FEW MAIN MENU OPTIONS

			game->getAudio()->playSound(L"data\\sounds\\loudnoise.wav", false);

			ifstream checkFile(SAVE_PATH);
			if (checkFile) // IS A GAME IN PRORGESS?
			{
				game->getText()->setTextLow(1);
				game->getText()->setTextHigh(4);
				game->getText()->setTextIndex(1);
				game->getGSM()->goToMainMenu();
			}
			else
			{
				game->getText()->setTextLow(2);
				game->getText()->setTextHigh(4);
				game->getText()->setTextIndex(2);
				game->getGUI()->getScreen(GS_MAIN_MENU)->setTextIndex(2);
				game->getGSM()->goToMainMenu();
			}
			checkFile.close();
		}

		/*if (input->isKeyDownForFirstTime(VK_UP) && splash->getSelector()->y > SELECTOR_LOWER_BOUND)
			splash->getSelector()->y -= 50;
		else if (input->isKeyDownForFirstTime(VK_DOWN) && splash->getSelector()->y < SELECTOR_UPPER_BOUND)
			splash->getSelector()->y += 50;*/



	}



	if (game->getGSM()->getCurrentGameState() == GS_GAME_IN_PROGRESS)
	{

		

		bool viewportMoved = false;
		float viewportVx = 0.0f;
		float viewportVy = 0.0f;

		Viewport *viewport = game->getGUI()->getViewport();
		if (viewportMoved)
			viewport->moveViewport((int)floor(viewportVx + 0.5f), (int)floor(viewportVy + 0.5f), game->getGSM()->getWorld()->getWorldWidth(), game->getGSM()->getWorld()->getWorldHeight());

		
		bool move = false;
		
		
		if (input->isKeyDown(W_KEY))
		{
			game->getGSM()->getPhyiscs()->movePlayerDown();
			if (!input->isKeyDown(A_KEY) && !input->isKeyDown(D_KEY))//Hoverstate
				game->getGSM()->getSpriteManager()->getPlayer()->setCurrentState(L"WALKING_NORTH");
			move = true;
		}
		if (input->isKeyDown(A_KEY))
		{
			game->getGSM()->getPhyiscs()->movePlayerLeft();
			if (!input->isKeyDown(W_KEY) && !input->isKeyDown(S_KEY))//Hoverstate
			game->getGSM()->getSpriteManager()->getPlayer()->setCurrentState(L"WALKING_WEST_GREEN");
			move = true;
		}
		if (input->isKeyDown(S_KEY))
		{
			game->getGSM()->getPhyiscs()->movePlayerUp();
			if (!input->isKeyDown(A_KEY) && !input->isKeyDown(D_KEY))//Hoverstate
				game->getGSM()->getSpriteManager()->getPlayer()->setCurrentState(L"WALKING_SOUTH_GREEN");
			move = true;
		}
		if (input->isKeyDown(D_KEY))
		{
			game->getGSM()->getPhyiscs()->movePlayerRight();
			if (!input->isKeyDown(W_KEY) && !input->isKeyDown(S_KEY))//Hoverstate
				game->getGSM()->getSpriteManager()->getPlayer()->setCurrentState(L"WALKING_EAST_GREEN");
			move = true;
		}
		if (!move)
			game->getGSM()->getSpriteManager()->getPlayer()->setCurrentState(L"IDLE_GREEN");

		if (input->isKeyDown(W_KEY) && input->isKeyDown(D_KEY))
		{
			game->getGSM()->getSpriteManager()->getPlayer()->setCurrentState(L"HOVERING_EAST");
		}
		if (input->isKeyDown(W_KEY) && input->isKeyDown(A_KEY))
		{
			game->getGSM()->getSpriteManager()->getPlayer()->setCurrentState(L"HOVERING_WEST");
		}
		if (input->isKeyDown(S_KEY) && input->isKeyDown(D_KEY))
		{
			game->getGSM()->getSpriteManager()->getPlayer()->setCurrentState(L"HOVERING_EAST");
		}
		if (input->isKeyDown(S_KEY) && input->isKeyDown(A_KEY))
		{
			game->getGSM()->getSpriteManager()->getPlayer()->setCurrentState(L"HOVERING_WEST");
		}


		if (input->isKeyDown(ONE_KEY))
		{
			game->getGSM()->getSpriteManager()->getPlayer()->setSelectedGun(game->getGSM()->getSpriteManager()->getPlayer()->DEFAULT);
			game->getGSM()->getSpriteManager()->getPlayer()->stopFlameThrower();
		}

		if (input->isKeyDown(TWO_KEY))
		{
			game->getGSM()->getSpriteManager()->getPlayer()->setSelectedGun(game->getGSM()->getSpriteManager()->getPlayer()->SPREAD);
			game->getGSM()->getSpriteManager()->getPlayer()->stopFlameThrower();
		}
		if (input->isKeyDown(THREE_KEY))
		{
			game->getGSM()->getSpriteManager()->getPlayer()->setSelectedGun(game->getGSM()->getSpriteManager()->getPlayer()->ROCKET);
			game->getGSM()->getSpriteManager()->getPlayer()->stopFlameThrower();
		}
		if (input->isKeyDown(FOUR_KEY))
		{
			game->getGSM()->getSpriteManager()->getPlayer()->setSelectedGun(game->getGSM()->getSpriteManager()->getPlayer()->FLAMETHROWER);
		}
		if (input->isKeyDown(FIVE_KEY))
		{
			game->getGSM()->getSpriteManager()->getPlayer()->setSelectedGun(game->getGSM()->getSpriteManager()->getPlayer()->FORCE);
		}



	}

	if (game->getGSM()->getCurrentGameState() == GS_WIN_SCREEN)
	{
		if (input->isKeyDownForFirstTime(VK_RETURN))
		{
			game->quitGame();
		}
	}

	if (game->getGSM()->getCurrentGameState() == GS_GAME_OVER)
	{
		if (input->isKeyDownForFirstTime(VK_UP))
		{
			if (game->getGUI()->getScreen(GS_GAME_OVER)->getTextIndex() == game->getText()->getTextLow())
			{
			}
			else
			{
				game->getGUI()->getScreen(GS_GAME_OVER)->setTextIndex(game->getGUI()->getScreen(GS_GAME_OVER)->getTextIndex() - 1);
			}

		}

		if (input->isKeyDownForFirstTime(VK_DOWN))
		{
			if (game->getGUI()->getScreen(GS_GAME_OVER)->getTextIndex() == game->getText()->getTextHigh())
			{
			}
			else
			{
				game->getGUI()->getScreen(GS_GAME_OVER)->setTextIndex(game->getGUI()->getScreen(GS_GAME_OVER)->getTextIndex() + 1);
			}
		}

		game->getText()->setTextIndex(game->getGUI()->getScreen(GS_GAME_OVER)->getTextIndex());


		if (input->isKeyDownForFirstTime(VK_RETURN))
		{
			int selected = game->getText()->getTextIndex();

			if (selected == 8)
			{
				game->getGSM()->resetLevel(game, game->getCurrentLevelFileName());
				Viewport *v = game->getGUI()->getViewport();
				v->setScrollSpeedX(0);
				v->setScrollSpeedY(0);
				v->setViewportX(game->getGSM()->getSpriteManager()->getPlayer()->getInitX());
				v->setViewportY(game->getGSM()->getSpriteManager()->getPlayer()->getInitY());

				game->getGSM()->goToGame();
			}
			if (selected == 9)
			{
				ifstream checkFile(SAVE_PATH);
				if (checkFile) // IS A GAME IN PRORGESS?
				{
					game->getText()->setTextLow(1);
					game->getText()->setTextHigh(4);
					game->getText()->setTextIndex(1);
					//game->getGSM()->goToMainMenu();
				}
				else
				{
					game->getText()->setTextLow(2);
					game->getText()->setTextHigh(4);
					game->getText()->setTextIndex(2);
					game->getGUI()->getScreen(GS_MAIN_MENU)->setTextIndex(2);
					//game->getGSM()->goToMainMenu();
				}
				checkFile.close();

				game->quitGame();
				return;
			}
			
		}
	}

	}

	



