/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	SpriteManager.cpp

	See SpriteManager.h for a class description.
*/

#pragma once
#include "../../../../stdafx.h"
#include "../../gsm/ai/Bot.h"
#include "../../graphics/GameGraphics.h"
#include "../../gsm/sprite/AnimatedSprite.h"
#include "../../gsm/sprite/AnimatedSpriteType.h"
#include "../../gsm/sprite/SpriteManager.h"
#include "../../gsm/state/GameStateManager.h"
#include "../../gsm/world/Tile.h"
#include "../../gsm/world/World.h"
#include "../../gsm/world/WorldLayer.h"
#include "../../gsm/world/TiledLayer.h"
#include "../../../../MeleeBot.h"
#include "../../../../MageBoss.h"
#include "../../../../../Odyssey/Odyssey.h"
#include "../Odyssey/OdysseyTextGenerator.h"
#include "../SimpleSideScrollerFramework/src/sssf/input/KeyEventHandler.h"
#include "../SimpleSideScrollerFramework/src/sssf/input/GameInput.h"
#include "../SimpleSideScrollerFramework/src/sssf/gui/GameGUI.h"
#include "../SimpleSideScrollerFramework/src/sssf/gui/ScreenGUI.h"
#include "../SimpleSideScrollerFramework/src/sssf/gui/SplashScreenGUI.h"
#include "../../../../MageBullet.h"
#include "../../../../../Odyssey/OdysseyDataLoader.h"
#include "../../../../FlamethrowerRayCast.h"

/*
	addSpriteToRenderList - This method checks to see if the sprite
	parameter is inside the viewport. If it is, a RenderItem is generated
	for that sprite and it is added to the render list.
*/


void SpriteManager::addSpriteToRenderList(AnimatedSprite *sprite,
										  RenderList *renderList,
										  Viewport *viewport)
{
	// GET THE SPRITE TYPE INFO FOR THIS SPRITE
	AnimatedSpriteType *spriteType = sprite->getSpriteType();
	//PhysicalProperties *pp = sprite->getPhysicalProperties();

	if (sprite->getSpriteType() == NULL)
	{
		return;
	}
	float x = sprite->getDefaultX();
	float y = sprite->getDefaultY();
	float rotation = 0.0f;

	

	// BODY COORDS TAKES PRECADENCE OVER DEFAULT COORDS
	b2Body *body = sprite->getBody();
	if (body != NULL)
	{
		x = body->GetPosition().x / 0.009765625;
		y = body->GetPosition().y / 0.009765625;
		rotation = body->GetAngle();

		if (sprite->getSpriteType() == this->getSpriteType(6))
		{
			x = x - 32;
			y = y - 32;

		}

		if (sprite->getSpriteType() == this->getSpriteType(8))
		{
			x = x - 32;
			y = y - 32;

		}

		if (sprite->getSpriteType() == this->getSpriteType(7))
		{
			x = 1532;
			y = 2470;
		}

		if (sprite->getSpriteType() == this->getSpriteType(10))
		{
			x = x + 10;
			y = y + 10;
		}

		if (sprite->getSpriteType() == this->getSpriteType(12))
		{
			if (sprite->getCurrentState() == L"SHIELD")
			{
				x = x - 30;
				y = y - 35;
			}
		}

		if (sprite->getSpriteType() == this->getSpriteType(17))
		{
			x = x - 126;
		}

		if (sprite->getSpriteType() == this->getSpriteType(25))
		{
			rotation = player.getMouseAngle();
			x = x - 173;
			y = y - 97;
		}

		if (sprite->getSpriteType() == this->getSpriteType(27))
		{
			x = x - 35;
			//y = y - 30;
		}
		
	}
	else
	{
		Effect *e = dynamic_cast<Effect*>(sprite);
		if (e != 0)
		{
			rotation = e->getAngle();
			x = x - 50;


		}
		
		
	}

	// IS THE SPRITE VIEWABLE?
	if (viewport->areWorldCoordinatesInViewport(	// HARD CODED VALUES FOR TESTING
									x,
									y,
									spriteType->getTextureWidth(),
									spriteType->getTextureHeight()))
	{
		// SINCE IT'S VIEWABLE, ADD IT TO THE RENDER LIST
		RenderItem itemToAdd;
		itemToAdd.id = sprite->getFrameIndex();
		renderList->addRenderItem(	sprite->getCurrentImageID(),
									(int)floor((x-viewport->getViewportX()+0.5f)),
									(int)floor((y-viewport->getViewportY()+0.5f)),
									0,
									sprite->getAlpha(),
									spriteType->getTextureWidth(),
									spriteType->getTextureHeight(),
									rotation);
	}
}



/*
	addSpriteItemsToRenderList - This method goes through all of the sprites,
	including the player sprite, and adds the visible ones to the render list.
	This method should be called each frame.
*/
void SpriteManager::addSpriteItemsToRenderList(	Game *game)
{
	GameStateManager *gsm = game->getGSM();
	GameGUI *gui = game->getGUI();
	if (gsm->isWorldRenderable())
	{
		GameGraphics *graphics = game->getGraphics();
		RenderList *renderList = graphics->getWorldRenderList();
		Viewport *viewport = gui->getViewport();

		

		// ADD THE HEALTH BAR
		//addSpriteToRenderList(&health, renderList, viewport);

		// NOW ADD THE REST OF THE SPRITES
		list<Bot*>::iterator botIterator;
		botIterator = bots.begin();
		while (botIterator != bots.end())
		{			
			Bot *bot = (*botIterator);
			addSpriteToRenderList(bot, renderList, viewport);

			MageBoss *test = dynamic_cast<MageBoss*>(bot);

			if (test != 0)
			{
				if (test->areOrbsActive())
				{
					
					for (int i = 0; i < 4; i++)
					{
						AnimatedSprite *orb = test->getOrbs(i);
						addSpriteToRenderList(orb, renderList, viewport);
					}
				}
			}

			botIterator++;
		}

		// ADD THE PLAYER SPRITE
		addSpriteToRenderList(&player, renderList, viewport);
		addSpriteToRenderList(player.getStatus(), renderList, viewport);
		addSpriteToRenderList(player.getFlameThrower(), renderList, viewport);


		list<Bullet*>::iterator bulletsIt;
		bulletsIt = activeBullets.begin();
		
		while (bulletsIt != activeBullets.end())
		{
			Bullet* bullet = (*bulletsIt);
			addSpriteToRenderList(bullet, renderList, viewport);
			bulletsIt++;
	
		}

		/*list<Bullet*>::iterator enemyBulletsIt;
		enemyBulletsIt = activeEnemyBullets.begin();

		while (enemyBulletsIt != activeEnemyBullets.end())
		{
			Bullet* bullet = (*enemyBulletsIt);
			addSpriteToRenderList(bullet, renderList, viewport);
			enemyBulletsIt++;

		}*/


		list<Effect*>::iterator effectIt;
		effectIt = activeEffects.begin();

		while (effectIt != activeEffects.end())
		{
			Effect* effect = (*effectIt);
			addSpriteToRenderList(effect, renderList, viewport);
			effectIt++;
		}

		
	}
	if (gsm->getCurrentGameState() == GS_LEVEL_SELECT)
	{
		addShootingStarsToRenderList(game);
	}

}

void SpriteManager::addShootingStarsToRenderList(Game *game)
{
	GameStateManager *gsm = game->getGSM();
	GameGUI *gui = game->getGUI();
	GameGraphics *graphics = game->getGraphics();
	RenderList *renderList = graphics->getWorldRenderList();
	Viewport *viewport = gui->getViewport();


	list<ShootingStar*>::iterator starIterator;
	starIterator = stars.begin();
	while (starIterator != stars.end())
	{
		ShootingStar *star = (*starIterator);
		addSpriteToRenderList(star, renderList, viewport);
		starIterator++;
	}
}

void SpriteManager::updateShootingStars(Game *game)
{
	list<ShootingStar*>::iterator starIterator;
	starIterator = stars.begin();
	while (starIterator != stars.end())
	{
		ShootingStar *star = (*starIterator);
		star->updateSprite();
		starIterator++;

	}
}


//AnimatedSprite* SpriteManager::getLifeSprite(int id)
//{
	//return lifes.at(id);
//}

/*
	addSprite - This method is for adding a new sprite to 
	this sprite manager. Once a sprite is added it can be 
	scheduled for rendering.
*/
void SpriteManager::addBot(Bot *botToAdd)
{
	bots.push_back(botToAdd);
}

void SpriteManager::addEffect(Effect *effect)
{
	activeEffects.push_back(effect);
}

void SpriteManager::addStar(ShootingStar* starToAdd)
{
	stars.push_back(starToAdd);
}


void SpriteManager::addActiveBullet(Bullet *bullet)
{
	activeBullets.push_back(bullet);
}

/*void SpriteManager::addActiveEnemyBullet(Bullet *bullet)
{
	activeEnemyBullets.push_back(bullet);
}*/

//void SpriteManager::addLife(AnimatedSprite *lifeToAdd)
//{
	//lifes.push_back(lifeToAdd);
//}

/*
	addSpriteType - This method is for adding a new sprite
	type. Note that one sprite type can have many sprites. For
	example, we may say that there may be a "Bunny" type of
	sprite, and specify properties for that type. Then there might
	be 100 different Bunnies each with their own properties, but that
	share many things in common according to what is defined in
	the shared sprite type object.
*/
unsigned int SpriteManager::addSpriteType(AnimatedSpriteType *spriteTypeToAdd)
{
	spriteTypes.push_back(spriteTypeToAdd);
	return spriteTypes.size()-1;
}

/*
	clearSprites - This empties all of the sprites and sprite types.
*/
void SpriteManager::clearSprites()
{
	spriteTypes.clear();
	bots.clear();
}

/*
	getSpriteType - This gets the sprite type object that corresponds
	to the index argument.
*/
AnimatedSpriteType* SpriteManager::getSpriteType(unsigned int typeIndex)
{
	if (typeIndex < spriteTypes.size())
		return spriteTypes.at(typeIndex);
	else
		return NULL;
}

/*
	unloadSprites - This method removes all artwork from memory that
	has been allocated for game sprites.
*/
void SpriteManager::unloadSprites(GameStateManager *gsm)
{
	// @TODO - WE'LL DO THIS LATER WHEN WE LEARN MORE ABOUT MEMORY MANAGEMENT
	list<Bullet*>::iterator bulletsIt;
	bulletsIt = activeBullets.begin();

	while (bulletsIt != activeBullets.end())
	{
		Bullet* bullet = (*bulletsIt);
		bulletRecycler.recycleBullet(bullet->getType(), bullet);
		activeBullets.erase(bulletsIt++);
	}

	list<Bullet*>::iterator enemyBulletsIt;
/*	enemyBulletsIt = activeEnemyBullets.begin();

	while (enemyBulletsIt != activeEnemyBullets.end())
	{
		Bullet* bullet = (*enemyBulletsIt);
		enemyBulletRecycler.recycleBullet(bullet->getType(), bullet);
		activeEnemyBullets.erase(bulletsIt++);
	}*/

	list<Bot*>::iterator botIterator;
	botIterator = bots.begin();
	while (botIterator != bots.end())
	{
		Bot *bot = (*botIterator);
		recycler.recycleBot(bot->getType(), bot);
		botIterator = bots.erase(botIterator++);
	}

	list<Effect*>::iterator effectIt;
	effectIt = activeEffects.begin();

	while (effectIt != activeEffects.end())
	{
		Effect* effect = (*effectIt);
		
		effectRecycler.recycleEffect(effect->getType(), effect);
		activeEffects.erase(effectIt++);
	}
	effectRecycler.reset(gsm);
	bulletRecycler.reset(gsm);
	recycler.reset(gsm);



}

Bot* SpriteManager::removeBot(Bot *botToRemove)
{
	return NULL;
	// @TODO - WE'LL DO THIS LATER WHEN WE LEARN MORE ABOUT MEMORY MANAGEMENT
}

/*
	update - This method should be called once per frame. It
	goes through all of the sprites, including the player, and calls their
	update method such that they may update themselves.
*/
void SpriteManager::update(Game *game)
{
	// UPDATE THE PLAYER SPRITE
	
	if (player.isOutOfLives())//game->getGSM()->getSpriteManager()->getPlayer()->getPlayerHealth() <= 0)
	{
		//game->getAudio()->stopAllAudio();
		game->getText()->setTextLow(8);
		game->getText()->setTextHigh(9);
		game->getText()->setTextIndex(8);
		game->getGUI()->getScreen(GS_GAME_OVER)->setTextIndex(8);
	//	game->getGSM()->goToMainMenu();
		game->getGSM()->goToDeathScreen();
	}

	player.updateSprite();
	player.updateStatus();
	player.updateFlameThrower();

	if (player.isFlameThrowerOn())
	{
		FlamethrowerRayCast cast;
		cast.setIgnoreBody(player.getBody());
		float x = game->getGSM()->getPhyiscs()->getPlayerBody()->GetPosition().x;
		float y = game->getGSM()->getPhyiscs()->getPlayerBody()->GetPosition().y;

		float pixelScaling = game->getGSM()->getPhyiscs()->getPixelScaling();
		float32 L = 170 * pixelScaling;
		b2Vec2 point1(player.getBody()->GetPosition().x, player.getBody()->GetPosition().y);
		float angle = player.getMouseAngle();

		b2Vec2 d(L * cosf(angle), L * sinf(angle));
		b2Vec2 point2 = point1 + d;
		game->getGSM()->getPhyiscs()->getGameWorld()->RayCast(&cast, point1, point2);

		vector<b2Body*>::iterator bodyIT = cast.getBodiesFoundBegin();
		while (bodyIT != cast.getBodiesFoundEnd())
		{
			b2Body *body = *(bodyIT);
			Bot *bot = static_cast<Bot*>(body->GetUserData());
			bot->setJustShot(true);
			bot->setHealth(bot->getHealth() - 5);
			bodyIT++;
		}


	}


	list<Bullet*>::iterator bulletsIt;
	bulletsIt = activeBullets.begin();

	while (bulletsIt != activeBullets.end())
	{
	Bullet* bullet = (*bulletsIt);
			if (bullet->getBody() != NULL)
			{
				b2Vec2 bP = bullet->getBody()->GetPosition();
				float32 bX = bP.x * 100;
				float32 bY = bP.y * 100;
				int viewX = (game->getGUI()->getViewport()->getViewportX());
				int viewY = (game->getGUI()->getViewport()->getViewportY());
				int viewH = game->getGUI()->getViewport()->getViewportHeight();
				int viewW = game->getGUI()->getViewport()->getViewportWidth();

				if ((bX < viewX || bY < viewY) || (bY > (viewY + viewH) || bX > (viewX + viewW)))
				{
					//if (bullet->getType() != L"LIGHTNING" && bullet->getType() != L"FIREBALL")
					//{
						game->getGSM()->getPhyiscs()->deactivateBullet(bullet);
						bulletRecycler.recycleBullet(bullet->getType(), bullet);
						activeBullets.erase(bulletsIt++);
					//}
					//else
						//bulletsIt++;
				}
				else
				{
					if (bullet->hasCollided())
					{
							// THE BULLET IS NO LONGER AN ACTIVE BULLET SO RECYCLE IT
							game->getGSM()->getPhyiscs()->deactivateBullet(bullet);
							bulletRecycler.recycleBullet(bullet->getType(), bullet);
							//	bullets.push_front(bullet);

							activeBullets.erase(bulletsIt++);
					}
					else if (bullet->getBody()->IsActive())
					{

						MageBullet *b = dynamic_cast<MageBullet*>(bullet);
						if (b != 0)
						{
							b->decDetonateTime();
							if (b->getDetonateTime() <= 0)
								b->detonate(game);
						}
						if (bullet->getType() == L"LAVA_BURST")
						{
							bullet->update();
							bullet->handleCollision(game);
							bulletsIt++;
						}
						else
						{
							bullet->update();
							bulletsIt++;
						}
					}
					
				}
			}
			else
			bulletsIt++;
		}





	// NOW UPDATE THE REST OF THE SPRITES
	list<Bot*>::iterator botIterator;
	botIterator = bots.begin();
	//int deathCount = 0;
	numKills = 0;
	bool allDead = true;
	while (botIterator != bots.end())
	{
	Bot *bot = (*botIterator);

		if (bot->getMarkedForDeath() == true && bot->getType() != W_MAGE_BOSS)
		{
			numKills++;
				game->getGSM()->getPhyiscs()->deactivateBot(bot);
				bot->updateSprite();
				int size = bot->getSpriteType()->getSequenceSize(L"DEATH");
				int index = bot->getFrameIndex();
				if (bot->getFrameIndex() == size - 2)
				{
					bot->setCurrentState(L"DEAD");


				}
			/*	bot->setHealth(100);
				recycler.recycleBot(W_MELEE_BOT, bot);
				botIterator = bots.erase(botIterator);*/
				botIterator++;
			
			
			
		}
		else if (bot->getCurrentState().find(L"DEATH") != -1 || bot->getCurrentState().find(L"DEAD") != -1)//is lava boss skip
		{
			numKills++;
			//Prevent dead bots from thinking
			botIterator++;
		}
		else
		{
			allDead = false;
			bot->think(game);
			bot->updateSprite();

			MageBoss *test = dynamic_cast<MageBoss*>(bot);

			if (test != 0)
			{
				if (test->areOrbsActive())
				{
					
					for (int i = 0; i < 4; i++)
					{
						AnimatedSprite *orb = test->getOrbs(i);
						orb->updateSprite();
					}
				}
			}


			if (bot->getHealth() <= 0)
			{
				bot->setMarkedForDeath(true);
				if (bot->getType() != W_MAGE_BOSS)
				{
					bot->setCurrentState(L"DEATH");
				}
			}

			botIterator++;
		}
			
		}

	if (allDead)//deathCount == 13)//Win
	{
		game->getGSM()->goToVictoryScreen();
		game->getGUI()->getScreen(GS_WIN_SCREEN)->setTextIndex(8);
		game->getAudio()->stopLoopingAudio();
		game->getAudio()->stopAllAudio();
	}

	list<Effect*>::iterator effectIt;
	effectIt = activeEffects.begin();

	while (effectIt != activeEffects.end())
	{
		Effect* effect = (*effectIt);
		effect->updateSprite();
		wstring state = effect->getCurrentState();
		int size = effect->getSpriteType()->getSequenceSize(state);
		int index = effect->getFrameIndex();

		if (effect->getFrameIndex() == size - 2)
		{
			effectRecycler.recycleEffect(effect->getType(), effect);
			activeEffects.erase(effectIt++);
		}
		else
		effectIt++;
	}
}


void SpriteManager::resetBots(Game *game)
{
	// NOW UPDATE THE REST OF THE SPRITES
	list<Bot*>::iterator botIterator;
	botIterator = bots.begin();
	while (botIterator != bots.end())
	{
		Bot *bot = (*botIterator);
		if (bot->getType() == L"MAGE_BOSS" || bot->getType() == L"LAVA_BOSS")
		{
			// THE BOSSES ARE TOO COMPLICATED TO RESET RIGHT NOW SO JUST DELETE THEM
			game->getGSM()->getPhyiscs()->deactivateBot(bot);
			game->getGSM()->getPhyiscs()->getGameWorld()->DestroyBody(bot->getBody());
			delete bot;
		}
		else
		{
			game->getGSM()->getPhyiscs()->deactivateBot(bot);
			bot->reset(game);
			bot->resetSprite();
			bot->setMarkedForDeath(false);
			recycler.recycleBot(bot->getType(),bot);
			
			
		}

		botIterator++;
		
	}
	bots.clear();
	OdysseyDataLoader* dl = dynamic_cast <OdysseyDataLoader*>(game->getDataLoader());
	player.reset();
	game->getGSM()->getPhyiscs()->resetPlayer(&player);
	dl->loadBotsFromLua(game->getCurrentLevelFileName(), game);


}
