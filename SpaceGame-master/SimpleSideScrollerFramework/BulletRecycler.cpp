#include "BulletRecycler.h"
#include "src\sssf\gsm\state\GameStateManager.h"


BulletRecycler::BulletRecycler()
{
}


BulletRecycler::~BulletRecycler()
{
	
}

void BulletRecycler::reset(GameStateManager *gsm)
{

	map<wstring, list<Bullet *> *>::iterator testIt = recyclableBullets.begin();
	while (testIt != recyclableBullets.end())
	{
		list<Bullet*> *toDelete = testIt->second;
		list<Bullet*>::iterator listIt = toDelete->begin();
		while (listIt != toDelete->end())
		{
			Bullet *effect = *listIt;
			gsm->getPhyiscs()->deleteSpriteBody(effect);
			delete effect;
			effect = NULL;
			toDelete->erase(listIt++);
		}
		toDelete->clear();
		delete toDelete;
		toDelete = NULL;
		recyclableBullets.erase(testIt++);


	}
	recyclableBullets.clear();


	map<wstring, Bullet *>::iterator regEff = registeredBulletTypes.begin();
	while (regEff != registeredBulletTypes.end())
	{
		Bullet *eff = regEff->second;
		gsm->getPhyiscs()->deleteSpriteBody(eff);
		delete eff;
		eff = NULL;
		registeredBulletTypes.erase(regEff++);

	}

	registeredBulletTypes.clear();


}


void BulletRecycler::initRecyclableBullets(Game *game, wstring BulletType, unsigned int maxBulletsNeeded)
{
	// FIRST MAKE SURE WE DON'T ALREADY HAVE A BUNCH OF RECYCLABLE
	// BulletS FOR THIS TYPE. TO DO THIS, WE USE SOME C++ WEIRDNESS.
	map<wstring, list<Bullet *> *>::iterator testIt = recyclableBullets.find(BulletType);
	if (testIt == recyclableBullets.end())
	{
		// MAKE THE RECYCLE LIST
		recyclableBullets[BulletType] = new list<Bullet *>();

		// AND ADD THE BulletS
		addMoreBullets(game, recyclableBullets[BulletType], BulletType, maxBulletsNeeded);
	}
}

/*
It is assumed that a game will have many different types of Bullets, thinking
in different ways, some of them even custom Bullets defined by the game app rather
than by this framework. So, in order to make these Bullets recyclable, we have to
register them with this recycler so that this recycler knows how to make new
ones as needed.
*/
void BulletRecycler::registerBulletType(wstring BulletType, Bullet *sampleBullet)
{
	// FIRST MAKE SURE WE DON'T ALREADY HAVE A BUNCH OF RECYCLABLE
	// BulletS FOR THIS TYPE. TO DO THIS, WE USE SOME C++ WEIRDNESS.
	if (!isRegisteredBulletType(BulletType))
	{
		// REGISTER THE Bullet
		registeredBulletTypes[BulletType] = sampleBullet;
	}
}

bool BulletRecycler::isRegisteredBulletType(wstring BulletType)
{
	// FIRST MAKE SURE THIS IS A REGISTERED Bullet TYPE,
	// IF IT IS NOT, WE NEED TO RETURN NULL
	map<wstring, Bullet *>::iterator testIt = registeredBulletTypes.find(BulletType);
	if (testIt == registeredBulletTypes.end())
		return false;
	else
		return true;
}


Bullet* BulletRecycler::retrieveBullet(Game *game, wstring BulletType)
{
	// FIRST MAKE SURE THIS IS A REGISTERED Bullet TYPE,
	// IF IT IS NOT, WE NEED TO RETURN NULL
	if (!isRegisteredBulletType(BulletType))
	{
		return NULL;
	}
	else
	{
		// GET THE CORRECT LIST OF Bullet TYPES
		list<Bullet *> *BulletsOfTypeWeNeed = recyclableBullets[BulletType];

		// MAKE SURE WE ARE NOT OUT OF THIS TYPE OF Bullet
		if (BulletsOfTypeWeNeed->size() == 0)
			addMoreBullets(game, BulletsOfTypeWeNeed, BulletType, RECYCLABLE_BULLET_INCREMENT);

		// NOW GET THE LAST ELEMENT
		Bullet* BulletToReturn = BulletsOfTypeWeNeed->back();
		BulletToReturn->resetSprite();
		BulletsOfTypeWeNeed->pop_back();
		
		return BulletToReturn;
	}
}

void BulletRecycler::recycleBullet(wstring BulletType, Bullet* BulletToRecycle)
{
	if (isRegisteredBulletType(BulletType))
	{
		list<Bullet *> *recycleBullets = recyclableBullets[BulletType];
		recycleBullets->push_back(BulletToRecycle);
	}
}

void BulletRecycler::addMoreBullets(Game *game, list<Bullet *> *BulletsNeededList, wstring BulletType, unsigned int numToAdd)
{
	// NOW MAKE THE BulletS
	Bullet* sampleBullet = registeredBulletTypes[BulletType];

	for (unsigned int i = 0; i < numToAdd; i++)
	{
		Bullet *BulletClone = sampleBullet->clone(game);
		BulletsNeededList->push_back(BulletClone);
	}
}
