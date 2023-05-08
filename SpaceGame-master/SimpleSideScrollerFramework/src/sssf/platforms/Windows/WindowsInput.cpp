/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WindowsInput.cpp

	See WindowsInput.h for a class description.
*/

#include "../../../../stdafx.h"
#include "../../gui/Button.h"
#include "../../gui/Cursor.h"
#include "../../game/Game.h"
#include "../../gsm/state/GameState.h"
#include "../../gsm/state/GameStateManager.h"
#include "../../gui/GameGUI.h"
#include "../../gui/ScreenGUI.h"
#include "../../input/GameInput.h"
#include "../../input/KeyEventHandler.h"
#include "../../os/GameOS.h"
#include "../../platforms/Windows/WindowsInput.h"
#include "../../platforms/Windows/WindowsOS.h"
#include "../../../../Bullet.h"

/*
	WindowsInput - Default constructor, it will initialize the input state variables.
*/
WindowsInput::WindowsInput()
{
	mousePoint = new POINT();
	mousePoint->x = 0;
	mousePoint->y = 0;
}

/*
	~WindowsInput - Destructor, it will destroy the mousePoint pointer.
*/
WindowsInput::~WindowsInput()
{
	delete mousePoint;
}

/*
	processInput - This method updates first gets all input from Windows
	for the mouse and the keyboard. It then asks the event handlers
	to respond to the input.
*/
void WindowsInput::processInput(Game *game)
{
	WindowsOS *os = (WindowsOS*)game->getOS();
	os->updateWindowInfo();
	WINDOWINFO wi = os->getWindowInfo();
	updateCursorPosition(game, wi, game->getGUI()->getCursor());
	updateInputState();
	respondToKeyboardInput(game);
	respondToMouseInput(game);
}

/*
	respondToMouseInput - This method sends the updated cursor position
	to the GameGUI so that it can update the Button and Cursor states.
	It then checks to see if the left mouse button is pressed, and if
	so, it asks the gui to check to see if it needs to fire an event.
	This should be called once per frame, after input is retrieved.
*/
void WindowsInput::respondToMouseInput(Game *game)
{
	GameGUI *gui = game->getGUI();
	GameStateManager *gsm = game->getGSM();

	gui->updateGUIState(mousePoint->x, mousePoint->y, gsm->getCurrentGameState());
	gsm->getPhyiscs()->setMouseLoc(mousePoint->x + (game->getGUI()->getViewport()->getViewportX()), mousePoint->y - (game->getGUI()->getViewport()->getViewportOffsetY()) + (game->getGUI()->getViewport()->getViewportY()));
	Player *p = gsm->getSpriteManager()->getPlayer();
	b2Body *pBody = gsm->getPhyiscs()->getPlayerBody();

	b2Vec2 clickedPoint = gsm->getPhyiscs()->getMouseClickedPoint();
	b2Vec2 aCalc;
	if (pBody != NULL && gsm->getCurrentGameState() == GS_GAME_IN_PROGRESS)
	{
		aCalc.x = (clickedPoint.x - pBody->GetPosition().x);
		aCalc.y = (clickedPoint.y - pBody->GetPosition().y);
		float angle = atan2f(aCalc.y, aCalc.x);
		p->setMouseAngle(angle);
	}

	
	

	//&& (inputState[VK_LBUTTON].isFirstPress)
	if ((GetAsyncKeyState(VK_LBUTTON) & 0X8000))
	{
		gui->checkCurrentScreenForAction(game);
		if (gsm->isGameInProgress())
		{
			game->getGSM()->getPhyiscs()->setMouseClicked(true);
			game->getGSM()->getPhyiscs()->setMouseVec(mousePoint->x + (game->getGUI()->getViewport()->getViewportX()), mousePoint->y - (game->getGUI()->getViewport()->getViewportOffsetY()) + (game->getGUI()->getViewport()->getViewportY()));
			// DO STUFF HERE FOR HANDLING WHICH BULLET BASED ON GUN, FOR NOW HARDCODED LASER

			
			p->decNextShotTime();
			unsigned int shotTime = p->getNextShotTime();

			if (p->getSelectedGun() == p->DEFAULT && shotTime == 0)
			{

				game->getAudio()->playSound(L"data\\sounds\\laser_pro.wav",false);
				Bullet *bullet = gsm->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"LASER");
				bullet->setDamageType('P');
				bullet->setCurrentState(L"PRIMARY_FIRE");
				gsm->getPhyiscs()->activateBullet(bullet);
				gsm->getSpriteManager()->addActiveBullet(bullet);
				p->setNextShotTime(2);
			}
			else if (p->getSelectedGun() == p->SPREAD && shotTime == 0)
			{
				game->getAudio()->playSound(L"data\\sounds\\laser_pro.wav",false);
				Bullet *bullet1 = gsm->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"TRISHOT");
				bullet1->setDamageType('P');
				Bullet *bullet2 = gsm->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"TRISHOT");
				bullet2->setDamageType('P');
				Bullet *bullet3 = gsm->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"TRISHOT");
				bullet3->setDamageType('P');
				bullet1->setCurrentState(L"PRIMARY_FIRE");
				bullet2->setCurrentState(L"PRIMARY_FIRE");
				bullet3->setCurrentState(L"PRIMARY_FIRE");
				gsm->getPhyiscs()->activateBulletSpread(bullet1, bullet2, bullet3);
				gsm->getSpriteManager()->addActiveBullet(bullet1);
				gsm->getSpriteManager()->addActiveBullet(bullet2);
				gsm->getSpriteManager()->addActiveBullet(bullet3);
				p->setNextShotTime(10);

			}
			else if (p->getSelectedGun() == p->ROCKET && shotTime == 0)
			{
				game->getAudio()->playSound(L"data\\sounds\\rocket.wav",false);
				Bullet *bullet1 = gsm->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"ROCKET");
				bullet1->setCurrentState(L"PRIMARY_FIRE");
				bullet1->setDamageType('P');
				gsm->getPhyiscs()->activateBullet(bullet1);
				gsm->getSpriteManager()->addActiveBullet(bullet1);
				p->setNextShotTime(20);
			}
			else if (p->getSelectedGun() == p->FORCE && shotTime == 0)
			{
				//game->getAudio()->playSound(L"data\\sounds\\rocket.wav", false);
				Bullet *bullet1 = gsm->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"FORCE_BULLET");
				bullet1->setCurrentState(L"PRIMARY_FIRE");
				bullet1->setDamageType('P');
				gsm->getPhyiscs()->activateBullet(bullet1);
				gsm->getSpriteManager()->addActiveBullet(bullet1);
				p->setNextShotTime(10);
			}
			else if (p->getSelectedGun() == p->FLAMETHROWER)
			{
				if (p->getNextShotTime() == 0)
				p->startFlameThrower();

			}
		}
		//ProjectileManager::createBullet(game, "PRIMARY", mousePoint);
	}
	else
	{ 
		if (p->getSelectedGun() == p->FLAMETHROWER)
		{
			p->stopFlameThrower();
		}
	}


	/*else if ((GetAsyncKeyState(VK_RBUTTON) & 0X8000) && (inputState[VK_RBUTTON].isFirstPress))
	{
		// invoke physics here
		if (gsm->isGameInProgress())
		{
			Player *p = gsm->getSpriteManager()->getPlayer();
			//p->decNextShotTime();
			//unsigned int shotTime = p->getNextShotTime();
			gui->checkCurrentScreenForAction(game);
			game->getGSM()->getPhyiscs()->setMouseClicked(true);
			game->getGSM()->getPhyiscs()->setMouseVec(mousePoint->x + (game->getGUI()->getViewport()->getViewportX()), mousePoint->y - (game->getGUI()->getViewport()->getViewportOffsetY()) + (game->getGUI()->getViewport()->getViewportY()));

			if (p->getSelectedGun() == p->DEFAULT)// && shotTime == 0)
			{
				/*game->getAudio()->playSound(L"data\\sounds\\laser.wav",false);
				Bullet *bullet = gsm->getSpriteManager()->getBulletRecycler()->retrieveBullet(game, L"LASER");
				bullet->setCurrentState(L"SECONDARY_FIRE");
				bullet->setDamageType('S');
				gsm->getPhyiscs()->activateBullet(bullet);
				gsm->getSpriteManager()->addActiveBullet(bullet);
				//p->setNextShotTime(2);
			}
			//ProjectileManager::createBullet(game, "SECONDARY", mousePoint);
		}
	}*/
}

/*
	updateCursorPosition - This method asks Windows for the position
	of the cursor in screen coordinates. The cursor position is fixed
	to account for windows borders. The values are recorded for use
	and the cursor is updated.
*/
void WindowsInput::updateCursorPosition(Game *game, WINDOWINFO wi, Cursor *cursor)
{
	GetCursorPos(mousePoint);
	GameGraphics *graphics = game->getGraphics();
	// Fix the mouse location
	mousePoint->x = mousePoint->x - wi.rcWindow.left;
	mousePoint->y = mousePoint->y - wi.rcWindow.top;
	if (mousePoint->x < 0)
	{
		mousePoint->x = 0;
	}
	if (mousePoint->x >= graphics->getScreenWidth())
	{
		mousePoint->x = graphics->getScreenWidth() - 1;
	}
	if (mousePoint->y < 0)
	{
		mousePoint->y = 0;
	}
	if (mousePoint->y >= graphics->getScreenHeight())
	{
		mousePoint->y = graphics->getScreenHeight() - 1;
	}

	cursor->setX(mousePoint->x);
	cursor->setY(mousePoint->y);
}

/*
	updateInputState - This method checks all keys and updates
	their states. This should be called once per frame.
*/
void WindowsInput::updateInputState()
{
	// RESET isPressed FOR ALL KEYS
	for (int i = 0; i < 256; i++)
		inputState[i].isPressed = false;

	// FILL IN isPressed FOR THOSE PRESSED
	for (int j = 0; j < 256; j++)
	{
		if (GetAsyncKeyState(j) & 0X8000)
			inputState[j].isPressed = true;
	}

	// UPDATE wasHeldDown & isFirstPress
	for (int k = 0; k < 256; k++) 
	{
		if (inputState[k].isPressed) 
		{
			if (inputState[k].wasHeldDown)
				inputState[k].isFirstPress = false;
			else 
			{
				inputState[k].wasHeldDown = true;
				inputState[k].isFirstPress = true;
			}
		}
		else 
		{
			inputState[k].wasHeldDown = false;
			inputState[k].isFirstPress = false;
		}
	}
}

/*
	shutdown - we are simply using Windows for getting input, but, if we were using
	another library, like DirectInput for getting game controller data, we may have
	to tie up some loose ends here.
*/
void WindowsInput::shutdown()
{
	// IF WE WERE USING ANOTHER LIBRARY, WE MAY NEED TO
	// SHUT THINGS DOWN
}