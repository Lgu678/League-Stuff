#include "Player.h"
#include "src\sssf\gsm\state\GameStateManager.h"


Player::Player()
{
	selectedGun = gunSelection::DEFAULT;
	playerHealth = 200;
	nextShowCountdown = 2;
	collidingWithBot = false;
	playerLives = 3;
	playerStartingHealth = 200;
	outOfLives = false;
	outOfHealth = false;
	immuneCounter = 66;
	isImmune = false;
	statusLoop = 0;
	status = NULL;
	flameThrower = NULL;
	flameThrowerOn = false;
	mouseAngle = 0.0f;

}


Player::~Player()
{
	delete status;
	delete flameThrower;
	flameThrower = NULL;
	status = NULL;
}

void Player::initStatusSprite()
{
	status = new AnimatedSprite();
	status->setAlpha(255);
	status->setSpriteType(game->getGSM()->getSpriteManager()->getSpriteType(12));
	status->setCurrentState(L"NONE");
	status->setBody(this->getBody());
}

void Player::initFlameThrower()
{
	flameThrower = new AnimatedSprite();
	flameThrower->setAlpha(255);
	flameThrower->setSpriteType(game->getGSM()->getSpriteManager()->getSpriteType(25));
	flameThrower->setCurrentState(L"OFF");
	flameThrower->setBody(this->getBody());
}



void Player::updateStatus()
{

	if (statusLoop == 0)
	{
		status->setCurrentState(L"NONE");
	}
	else
	{
		status->updateSprite();
		int size = status->getSpriteType()->getSequenceSize(status->getCurrentState());
		int index = status->getFrameIndex();
		if (status->getFrameIndex() == size - 2)
		{
			statusLoop--;
		}
	}
}

void Player::updateFlameThrower()
{
	if (flameThrower->getCurrentState() != L"OFF")
	{
		
		flameThrower->updateSprite();
		if (flameThrower->getCurrentState() == L"STARTUP")
		{
			int size = flameThrower->getSpriteType()->getSequenceSize(L"STARTUP");
			int index = flameThrower->getFrameIndex();

			if (index == size - 2)
			{
				flameThrower->setCurrentState(L"PRIMARY_FIRE");
			}
		}

		if (flameThrower->getCurrentState() == L"PRIMARY_FIRE")
			flameThrowerOn = true; 

		if (flameThrower->getCurrentState() == L"STOP")
		{
			flameThrowerOn = false;
			int size = flameThrower->getSpriteType()->getSequenceSize(L"STOP");
			int index = flameThrower->getFrameIndex();
			if (index == size - 2)
			{
				flameThrower->setCurrentState(L"OFF");
			}
		}

	}
	else
		flameThrowerOn = false;
}

void Player::startFlameThrower()
{
	if (flameThrower->getCurrentState() == L"OFF")
	flameThrower->setCurrentState(L"STARTUP");
}

void Player::stopFlameThrower()
{
	if (flameThrowerOn)
	{
		flameThrower->setCurrentState(L"STOP");
		nextShowCountdown = 30;
	}
	else
	{
		flameThrower->setCurrentState(L"OFF");
		flameThrowerOn = false;
	}
}

void Player::setStatus(wstring stat, int time)
{
	statusLoop = time;
	status->setCurrentState(stat);
}

void Player::updateSprite()
{
	AnimatedSprite::updateSprite();
	
	if (isImmune)
		immuneCounter--;
	if (immuneCounter == 0)
	{
		immuneCounter = 66;
		isImmune = false;
		playerHealth = playerStartingHealth;
		playerLives--;
		outOfHealth = false;
		game->getHud()->setHealthWidth(playerHealth, playerStartingHealth);
		this->setStatus(L"NONE", 0);
		statusLoop = 0;
	}
}

void Player::decPlayerHealth(int x)
{
	if (isImmune)
	{
	}
	else
	{
		playerHealth = playerHealth - x;
		game->getHud()->setHealthWidth(playerHealth, playerStartingHealth);
		if (playerHealth <= 0)
		{
			outOfHealth = true;

			if (playerLives == 0)
			{
				outOfLives = true;
				this->setMarkedForDeath(true);
			}
			else
			{			
				isImmune = true;
				game->getHud()->loseLife();
				this->setStatus(L"SHIELD", 100);
			}
		}
	}
}
