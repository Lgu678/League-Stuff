#include "PhysicsCollisionListener.h"
#include "src\sssf\gsm\ai\Bot.h"
#include "src\sssf\game\Game.h"
#include "src\sssf\gsm\state\GameStateManager.h"
#include "physicsManager.h"
#include "MeleeBot.h"
#include "ForceBullet.h"
#include "Fireball.h"
#include "Laser.h"
#include "Rocket.h"
#include "LavaBoss.h"
#include "TigerBot.h"
#include "RatBot.h"
#include "LavaBurst.h"


PhysicsCollisionListener::PhysicsCollisionListener()
{
}


PhysicsCollisionListener::~PhysicsCollisionListener()
{
}

void PhysicsCollisionListener::BeginContact(b2Contact *contact)
{
	void *dataA = contact->GetFixtureA()->GetBody()->GetUserData();
	void *dataB = contact->GetFixtureB()->GetBody()->GetUserData();

	physicsManager *phy = game->getGSM()->getPhyiscs();

	// THIS IS ASSUMING EVERY PIECE OF DATA ATTACHED TO BODIES ARE ANIMATED SPRITES (MIGHT CAUSE ERRORS LATER)
	AnimatedSprite *A = static_cast<AnimatedSprite*>(dataA);
	AnimatedSprite *B = static_cast<AnimatedSprite*>(dataB);

	bool sensA = contact->GetFixtureA()->IsSensor();
	bool sensB = contact->GetFixtureB()->IsSensor();

	if (dataA == NULL || dataB == NULL) // ITS A WALL COLLISION 
	{
		// ADDITIONAL DYNAMIC CASTING WILL HAVE TO BE DONE TO DETERMAIN WHAT TYPE OF BULLET IT ACTUALLY IS
		Bullet *b;

		if (dataA != NULL) // CHECK IF DATA A IS A BULLET
		{
			b = dynamic_cast<Bullet*>(A);
			if (b != 0)
			{
				if (b->getType().compare(L"FORCE_BULLET") == 0)
				{
					dynamic_cast<ForceBullet*>(A)->handleCollision(game);//Wall collide
				}
				else
					b->handleCollision(game);
			}
		}
		
		else
		{
			b = dynamic_cast<Bullet*>(B);
			Fireball *f = dynamic_cast<Fireball*>(b);

			if (b != 0)
			{
				if (b->getType().compare(L"FORCE_BULLET") == 0)
				{
					dynamic_cast<ForceBullet*>(B)->handleCollision(game);//Wall collide
				}
				else
				b->handleCollision(game);
			}
		}
		// IF NONE OF THEM WERE BULLETS IT A BOT/BOSS OR THE PLAYER HIT THE WALL AND WE DONT NEED TO DO ANYTHING


	}
	else
	{
		// LOTS OF THINGS TO TEST
		Bullet *a;
		Bullet *b;
		a = dynamic_cast<Bullet*>(A);
		b = dynamic_cast<Bullet*>(B);

		if (a == 0 && b == 0) // COLLISION DID NOT INVOLVE BULLETS
		{
			// CHECK FOR SENSOR COLLISION
			bool sensA = contact->GetFixtureA()->IsSensor();
			bool sensB = contact->GetFixtureB()->IsSensor();


			if ((sensA && sensB) || !(sensA || sensB))
			{
				if (!(sensA || sensB)) // COLLISON WAS BETWEEN PLAYER AND BOT
				{
					if (contact->GetFixtureA()->GetFilterData().categoryBits == phy->PLAYER)
					{
 					Player *p = dynamic_cast<Player*>(A);
						p->setCollidingWithBot(true);

						AnimatedSpriteType *b = B->getSpriteType();
						AnimatedSpriteType *c = game->getGSM()->getSpriteManager()->getSpriteType(13);

						if (b == c)
						{
							p->decPlayerHealth(20);
						}

					}
					else if (contact->GetFixtureB()->GetFilterData().categoryBits == phy->PLAYER)
					{
						Player *p = dynamic_cast<Player*>(B);
						p->setCollidingWithBot(true);

						AnimatedSpriteType *b = A->getSpriteType();
						AnimatedSpriteType *c = game->getGSM()->getSpriteManager()->getSpriteType(13);

						if (b == c)
						{
							p->decPlayerHealth(20);
						}
					}

				}
			}
			else
			{
				//PLAYER IN BOT RADIUS
				if (sensA)
				{
					Bot *bot = static_cast<Bot*>(A);
					bot->setPlayerInRadius(true);

					if (contact->GetFixtureA()->GetFilterData().categoryBits == phy->BOT_DAMAGE_RADIUS)
					{
						MeleeBot *mBot = dynamic_cast<MeleeBot*>(A);
						mBot->setInAttackRadius(true);
					}
				}
				else
				{
					Bot *bot = static_cast<Bot*>(B);
					bot->setPlayerInRadius(true);

					if (contact->GetFixtureB()->GetFilterData().categoryBits == phy->BOT_DAMAGE_RADIUS)
					{
						MeleeBot *mBot = dynamic_cast<MeleeBot*>(B);
						mBot->setInAttackRadius(true);
					}
				}
			}
		}
		else
		{

     		bool sensA = contact->GetFixtureA()->IsSensor();
			bool sensB = contact->GetFixtureB()->IsSensor();

			// FOR SOME REASONS BULLETS ARE COLLIDING SO THIS SHOULDNT HAPPEN
			if (a != 0 && b != 0)
			{
				return;
			}

			if (sensA)
			{
				Bot *bot = static_cast<Bot*>(A);
				if ((contact->GetFixtureA()->GetFilterData().categoryBits == phy->BOT_DODGE) && (bot->getType() == L"MAGE_BOSS"))
				{
					MageBoss *mB = dynamic_cast<MageBoss*>(A);
					//Bot *bot = dynamic_cast<Bot*>(A);
					mB->decDash();

					Rocket *r = dynamic_cast<Rocket*>(B);
					if (r != 0)
					{
						mB->decDash();
						mB->decDash();
						mB->decDash();
						mB->decDash();

					}

					return;
				}

			}
			else if (sensB)
			{
				Bot *bot = static_cast<Bot*>(B);
				if ((contact->GetFixtureB()->GetFilterData().categoryBits == phy->BOT_DODGE) && (bot->getType() == L"MAGE_BOSS"))
				{
					MageBoss *mB = dynamic_cast<MageBoss*>(B);
					//Bot *bot = dynamic_cast<Bot*>(A);
					mB->decDash();

					Rocket *r = dynamic_cast<Rocket*>(A);
					if (r != 0)
					{
						mB->decDash();
						mB->decDash();
						mB->decDash();
						mB->decDash();

					}

					return;
				}
			}


			if (sensA || sensB)
			{
				return;
			}

			if (a != 0) // a is the bullet
			{
				b2Fixture *detectFixture = contact->GetFixtureA();
				detectFixture->GetType();
				if (contact->GetFixtureA()->GetFilterData().categoryBits != phy->ENEMY_BULLET)
				{
					Bot *bot = static_cast<Bot*>(B);
					if (bot->getType() == L"LAVA_BOSS")
					{
						LavaBoss *lavaBoss = dynamic_cast<LavaBoss*>(B);
						if (lavaBoss->getInvincible() == false)
							lavaBoss->setHealth(bot->getHealth() - a->getPrimaryDamage());
						bot->setJustShot(true);
						a->handleCollision(game);
					}
					else
					{
						bot->setHealth(bot->getHealth() - a->getPrimaryDamage());
						bot->setJustShot(true);
						if (a->getType().compare(L"FORCE_BULLET") == 0)
						{
							dynamic_cast<ForceBullet*>(A)->handleCollision(game, B);
						}
						else
							a->handleCollision(game);
					}
				}
				else
				{
					LavaBurst *lavaBurst = dynamic_cast<LavaBurst*>(A);
					if (lavaBurst != 0)
					{
						if (lavaBurst->getCurrentState() == L"FULLSIZE")
						{
							a->handleCollision(game);
							Player *p = static_cast<Player*>(B);
							//p->decPlayerHealth(a->getPrimaryDamage());
						}
						else
						{
							a->handleCollision(game);
							Player *p = static_cast<Player*>(B);
							//p->decPlayerHealth(a->getPrimaryDamage());
						}
					}
					else
					{
						a->handleCollision(game);
						Player *p = static_cast<Player*>(B);
						p->decPlayerHealth(a->getPrimaryDamage());
					}
					//game->getHud()->setHealthWidth(game->getGSM()->getSpriteManager()->getPlayer()->getPlayerHealth(), game->getGSM()->getSpriteManager()->getPlayer()->getStartingHealth());
				}

			}
			else // b is the bullet
			{
				if (contact->GetFixtureB()->GetFilterData().categoryBits != phy->ENEMY_BULLET)
				{
					Bot *bot = static_cast<Bot*>(A);

					if (bot->getType() == L"LAVA_BOSS")
					{
						LavaBoss *lavaBoss = dynamic_cast<LavaBoss*>(A);
						if (lavaBoss->getInvincible() == false)
							lavaBoss->setHealth(bot->getHealth() - b->getPrimaryDamage());
						bot->setJustShot(true);
						b->handleCollision(game);
					}
					else
					{
						bot->setHealth(bot->getHealth() - b->getPrimaryDamage());
						bot->setJustShot(true);
						if (b->getType().compare(L"FORCE_BULLET") == 0)
						{
							dynamic_cast<ForceBullet*>(B)->handleCollision(game, A);
						}
						else
							b->handleCollision(game);
					}
				}
				else
				{
					LavaBurst *lavaBurst = dynamic_cast<LavaBurst*>(B);
					if (lavaBurst != 0)
					{
						if (lavaBurst->getCurrentState() == L"FULLSIZE")
						{
							b->handleCollision(game);
							Player *p = static_cast<Player*>(A);
							//p->decPlayerHealth(b->getPrimaryDamage());
						}
						else
						{
							b->handleCollision(game);
							Player *p = static_cast<Player*>(A);
							//p->decPlayerHealth(b->getPrimaryDamage());
						}
					}
					else
					{
						b->handleCollision(game);
						Player *p = static_cast<Player*>(A);
						p->decPlayerHealth(b->getPrimaryDamage());
					}
					//game->getHud()->setHealthWidth(game->getGSM()->getSpriteManager()->getPlayer()->getPlayerHealth(), game->getGSM()->getSpriteManager()->getPlayer()->getStartingHealth());
				}

			}
		}


	}
}

void PhysicsCollisionListener::EndContact(b2Contact *contact)
{
	void *dataA = contact->GetFixtureA()->GetBody()->GetUserData();
	void *dataB = contact->GetFixtureB()->GetBody()->GetUserData();

	// THIS IS ASSUMING EVERY PIECE OF DATA ATTACHED TO BODIES ARE ANIMATED SPRITES (MIGHT CAUSE ERRORS LATER)
	AnimatedSprite *A = static_cast<AnimatedSprite*>(dataA);
	AnimatedSprite *B = static_cast<AnimatedSprite*>(dataB);

	physicsManager *phy = game->getGSM()->getPhyiscs();

	bool sensA = contact->GetFixtureA()->IsSensor();
	bool sensB = contact->GetFixtureB()->IsSensor();

	if ((sensA && sensB) || !(sensA || sensB))
	{
		if (!(sensA || sensB)) // COLLISON WAS BETWEEN PLAYER AND BOT
		{
			if (contact->GetFixtureA()->GetFilterData().categoryBits == phy->PLAYER)
			{
				Player *p = dynamic_cast<Player*>(A);
				p->setCollidingWithBot(false);
			}
			else if (contact->GetFixtureB()->GetFilterData().categoryBits == phy->PLAYER)
			{
				Player *p = dynamic_cast<Player*>(B);
				p->setCollidingWithBot(false);
			}

		}
	}
	else
	{
		//PLAYER IN BOT RADIUS
		if (sensA)
		{
			//BOT DMG RADIUS IS A SPECIFIC RADIUS TO INDICATED MELEE TYPE BOTS ARE IN "MELEE RANGE"
			if (contact->GetFixtureA()->GetFilterData().categoryBits == phy->BOT_DAMAGE_RADIUS)
			{
				MeleeBot *mBot = dynamic_cast<MeleeBot*>(A);
				mBot->setInAttackRadius(false);
			}
			else
			{
				Bot *bot = static_cast<Bot*>(A);
				bot->setPlayerInRadius(false);
			}
		}
		else
		{

			if (contact->GetFixtureB()->GetFilterData().categoryBits == phy->BOT_DAMAGE_RADIUS)
			{
				MeleeBot *mBot = dynamic_cast<MeleeBot*>(B);
				mBot->setInAttackRadius(false);
			}
			else
			{
				Bot *bot = static_cast<Bot*>(B);
				bot->setPlayerInRadius(false);
			}
		}
	}
}