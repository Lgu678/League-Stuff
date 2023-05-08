/**
* 
*/

#include "stdafx.h"
#include "OdysseyDataLoader.h"


#include "OdysseyButtonEventHandler.h"
#include "OdysseyDataLoader.h"
#include "Odyssey.h"
#include "OdysseyKeyEventHandler.h"
#include "OdysseyTextGenerator.h"
#include "Bullet.h"
#include "MeleeBot.h"
#include "src\sssf\gsm\ai\Bot.h"
#include "src\sssf\gsm\ai\BotRecycler.h"
#include "BulletRecycler.h"
#include "Laser.h"
#include "ForceBullet.h"
#include "Rocket.h"
#include "Effect.h"
#include "EffectRecycler.h"
#include "ExplosionEffect.h"
#include "SkullBot.h"
#include "SpiderBot.h"
#include "LavaBoss.h"
#include "Andromalius.h"
#include "MageBoss.h"
#include "Fireball.h"
#include "MageBullet.h"
#include "AndromaliusBullet.h"
#include "LavaBurst.h"
#include "DarkEnergyEffect.h"
#include "LavaBall.h"
#include "Lightning.h"
#include "TigerBot.h"
#include "RatBot.h"
#include "SkullAttackEffect.h"
#include "Trishot.h"

// GAME OBJECT INCLUDES

#include "../SimpleSideScrollerFramework/src/sssf/game/Game.h"
#include "../SimpleSideScrollerFramework/src/sssf/graphics/GameGraphics.h"
#include "../SimpleSideScrollerFramework/src/sssf/gsm/state/GameState.h"
#include "../SimpleSideScrollerFramework/src/sssf/gsm/world/TiledLayer.h"
#include "../SimpleSideScrollerFramework/src/sssf/gui/Cursor.h"
#include "../SimpleSideScrollerFramework/src/sssf/gui/GameGUI.h"
#include "../SimpleSideScrollerFramework/src/sssf/gui/ScreenGUI.h"
#include "../SimpleSideScrollerFramework/src/sssf/gui/SplashScreenGUI.h"
#include "../SimpleSideScrollerFramework/src/sssf/input/GameInput.h"
#include "../SimpleSideScrollerFramework/src/sssf/os/GameOS.h"
#include "../SimpleSideScrollerFramework/src/sssf/text/GameText.h"


// WINDOWS PLATFORM INCLUDES
#include "../SimpleSideScrollerFramework/src/sssf/platforms/Windows/WindowsOS.h"
#include "../SimpleSideScrollerFramework/src/sssf/platforms/Windows/WindowsInput.h"
#include "../SimpleSideScrollerFramework/src/sssf/platforms/Windows/WindowsTimer.h"

// DIRECTX INCLUDES
#include "../SimpleSideScrollerFramework/src/sssf/platforms/DirectX/DirectXGraphics.h"
#include "../SimpleSideScrollerFramework/src/sssf/platforms/DirectX/DirectXTextureManager.h"

// TMX MAP LOADING
#include "../TMXMapImporter/src/tmxmi/TMXMapImporter.h"

// ANIMATED SPRITE TYPE LOADING
#include "../PoseurSpriteTypesImporter/src/psti/PoseurSpriteTypesImporter.h"
#include "SimpleMovement.h"

OdysseyDataLoader::OdysseyDataLoader()
{
	luaPState = LuaState::Create();
}


OdysseyDataLoader::~OdysseyDataLoader()
{
	LuaState::Destroy(luaPState);
	luaPState = NULL;
}

void OdysseyDataLoader::loadGame(Game *game, wstring gameInitFile)
{
	// AND LET'S READ IN THE GAME SETUP INFO
	// FIRST LOAD ALL THE PROPERTIES
	map<wstring, wstring> *properties = new map<wstring, wstring>();
	loadGameProperties(game, properties, gameInitFile);
	// WE NEED THE TITLE AND USE_FULLSCREEN_MODE TO INITIALIZE OUR WINDOW
	wstring titleProp = (*properties)[W_TITLE];
	wstring useFullscreenProp = (*properties)[W_USE_FULLSCREEN_MODE];
	bool useFullscreen = false;
	if (useFullscreenProp.compare(L"true") == 0)
		useFullscreen = true;

	// GET THE SCREEN WIDTH AND HEIGHT
	int screenWidth, screenHeight;
	wstring screenWidthProp = (*properties)[W_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[W_SCREEN_HEIGHT];
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;

	// MAKE A CUSTOM GameOS OBJECT (WindowsOS) FOR SOME WINDOWS
	// PLATFORM STUFF, INCLUDING A Window OF COURSE

	if ((*properties)[W_CENTER_SCREEN].compare(L"true") == 0)
		game->setCenterScreen(true);
	else
		game->setCenterScreen(false);

	//game->setCenterScreen( (bool) (*properties)[L"CENTER_SCREEN"]);
	WindowsOS *odysseyOS = new WindowsOS(hInstance,
		nCmdShow,
		useFullscreen,
		titleProp,
		screenWidth, screenHeight,
		game);

	int textFontSize;
	wstring textFontSizeProp = (*properties)[W_TEXT_FONT_SIZE];
	wstringstream(textFontSizeProp) >> textFontSize;

	// RENDERING WILL BE DONE USING DirectX
	DirectXGraphics *odysseyGraphics = new DirectXGraphics(game);
	odysseyGraphics->init(screenWidth, screenHeight);
	odysseyGraphics->initGraphics(odysseyOS, useFullscreen);
	odysseyGraphics->initTextFont(textFontSize);

	// AND NOW LOAD THE COLORS THE GRAPHICS WILL USE
	// AS A COLOR KEY AND FOR RENDERING TEXT
	initColors(odysseyGraphics, properties);

	// WE'LL USE WINDOWS PLATFORM METHODS FOR GETTING INPUT
	WindowsInput *odysseyInput = new WindowsInput();

	// AND WINDOWS FOR THE TIMER AS WELL
	WindowsTimer *odysseyTimer = new WindowsTimer();

	// NOW INITIALIZE THE Game WITH ALL THE
	// PLATFORM AND GAME SPECIFIC DATA WE JUST CREATED
	game->initPlatformPlugins((GameGraphics*)odysseyGraphics,
		(GameInput*)odysseyInput,
		(GameOS*)odysseyOS,
		(GameTimer*)odysseyTimer);

	TextureManager *worldTextureManager = game->getGraphics()->getWorldTextureManager();
	worldTextureManager->loadTexture(L"data/img/blanktexture.png"); // AN EMPTY TEXTURE

	// LOAD OUR CUSTOM TEXT GENERATOR, WHICH DRAWS
	// TEXT ON THE SCREEN EACH FRAME
	OdysseyTextGenerator *odysseyTextGenerator = new OdysseyTextGenerator();
	odysseyTextGenerator->initText(game);
	GameText *text = game->getText();
	text->initDebugFile(W_DEBUG_FILE);
	text->writeDebugOutput("--Debug Start--");
	text->setTextGenerator((TextGenerator*)odysseyTextGenerator);

	// INIT THE VIEWPORT TOO
	initViewport(game->getGUI(), properties);

	PoseurSpriteTypesImporter psti;
	psti.loadSpriteTypes(game, SPRITE_TYPES_LIST);
	

	// WE DON'T NEED THE PROPERTIES MAP ANYMORE, THE GAME IS ALL LOADED
	delete properties;

}
void OdysseyDataLoader::loadGUI(Game *game, wstring guiInitFile)
{
	GameGUI *gui = game->getGUI();
	GameGraphics *graphics = game->getGraphics();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	// SETUP THE CURSOR VIA OUR HELPER METHOD
	initCursor(gui, guiTextureManager);
	initSplashScreen(game, gui, guiTextureManager);
	initMainMenu(gui, guiTextureManager);
	initPauseScreen(game, gui, guiTextureManager);
	initInGameGUI(game, gui, guiTextureManager);
	initLevelSelectScreen(game, gui, guiTextureManager);
	initVictoryScreen(game, gui, guiTextureManager);
	initDeathScreen(game, gui, guiTextureManager);
	initHelpScreen(game, gui, guiTextureManager);

	/*int healthBar = game->getGraphics()->getGUITextureManager()->loadTexture(W_HEALTH_HUNDRED_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 256;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = healthBar;
	gui->addOverlayImage(imageToAdd);*/
	

}
void OdysseyDataLoader::loadWorld(Game *game, wstring levelInitFile)
{
	game->getHud()->resetHud();
	// LOSTS OF HARD CODED STUFF FOR LEVEL 1
	
	TMXMapImporter tmxMapImporter;

	if(game->getCurrentLevelFileName() == W_LEVEL_1_NAME)
		tmxMapImporter.loadWorld(game, W_LEVEL_1_DIR, W_LEVEL_1_NAME);
	else if (game->getCurrentLevelFileName() == W_LEVEL_2_NAME)
		tmxMapImporter.loadWorld(game, W_LEVEL_2_DIR, W_LEVEL_2_NAME);
	else if (game->getCurrentLevelFileName() == W_LEVEL_3_NAME)
		tmxMapImporter.loadWorld(game, W_LEVEL_3_DIR, W_LEVEL_3_NAME);

	game->getGSM()->getPhyiscs()->setGame(game);
	game->getGSM()->getPhyiscs()->registerGameToListener();

	int counter = 0;

	for (int i = 0; i < game->getGSM()->getWorld()->getLayers()->size(); i++)
	{
		if (game->getGSM()->getWorld()->getLayers()->at(i)->hasCollidableTiles())
		{
			TiledLayer *t = dynamic_cast<TiledLayer*>(game->getGSM()->getWorld()->getLayers()->at(i));
			int layerWidth = t->getLayerWidth();
			int layerHeight = t->getLayerHeight();
			int tileWidth = t->getTileWidth();
			int tileHeight = t->getTileHeight();

			int cols = layerWidth / tileWidth;
			int rows = layerHeight / tileHeight;

			for (int j = 0; j < rows; j++)
				for (int k = 0; k < cols; k++)
				{
					Tile *tile = t->getTile(j, k);
					if (tile->textureID != 0)
					{
						int tileX = (int)((k * tileWidth));
						int tileY = (int)((j * tileHeight));

						
						game->getGSM()->getPhyiscs()->initWallForGame(tileX, tileY);
						counter++;
					}
				}
			
			
		}
		
	}

	int c = counter;

	/*//HARD CODED COLLIDABLE TRIANGLE TEST
	b2Vec2 triangleVerts[3];
	triangleVerts[0].Set(191.0f, 318.0f);
	triangleVerts[1].Set(249.0f, 289.0f);
	triangleVerts[2].Set(250.0f, 316.0f);
	int32 count = 3;

	game->getGSM()->getPhyiscs()->initTriangleWallForGame(triangleVerts, count);*/
	

	//PoseurSpriteTypesImporter psti;
	//psti.loadSpriteTypes(game, SPRITE_TYPES_LIST);

	Viewport *viewport = game->getGUI()->getViewport();

	GameStateManager *gsm = game->getGSM();
	SpriteManager *spriteManager = gsm->getSpriteManager();
	Player *player = spriteManager->getPlayer();
	
	AnimatedSpriteType *playerSpriteType = spriteManager->getSpriteType(1);//3 is enemy
	player->setSpriteType(playerSpriteType);
	player->setAlpha(255);
	player->setCurrentState(IDLE);
	player->setPlayerHealth(200);
	player->setStartingHealth(200);
	player->registerPlayer(game);
	
	player->reset();
	BulletRecycler *bulletRecycler = spriteManager->getBulletRecycler();
//	BulletRecycler *enemyBulletRecycler = spriteManager->getEnemyBulletRecycler();
	EffectRecycler *effectRecycler = spriteManager->getEffectRecycler();
	
	

	AnimatedSpriteType *laserSS = spriteManager->getSpriteType(2);
	AnimatedSpriteType *rocketSS = spriteManager->getSpriteType(4);
	AnimatedSpriteType *explosion = game->getGSM()->getSpriteManager()->getSpriteType(5);
	AnimatedSpriteType *fireballSS = spriteManager->getSpriteType(9);
	AnimatedSpriteType *mageBulletSS = spriteManager->getSpriteType(10);
	AnimatedSpriteType *acidSS = spriteManager->getSpriteType(11);
	AnimatedSpriteType *test = spriteManager->getSpriteType(12);
	AnimatedSpriteType *lavaBurstSS = spriteManager->getSpriteType(14);
	AnimatedSpriteType *darkenergySS = spriteManager->getSpriteType(15);
	AnimatedSpriteType *lavaBallSS = spriteManager->getSpriteType(17);
	AnimatedSpriteType *lightningSS = spriteManager->getSpriteType(18);
	AnimatedSpriteType *skullAttackSS = spriteManager->getSpriteType(21);
	AnimatedSpriteType *andromaliusBulletSS = spriteManager->getSpriteType(23);
	AnimatedSpriteType *forceBulletSS = spriteManager->getSpriteType(26);
	AnimatedSpriteType *triSS = spriteManager->getSpriteType(27);

	SkullAttackEffect *sampleSAE = new SkullAttackEffect();
	sampleSAE->setSpriteType(skullAttackSS);
	sampleSAE->setAlpha(255);
	sampleSAE->setCurrentState(L"ATTACK");

	effectRecycler->registerEffectType(L"SKULLATTACKEFFECT", sampleSAE);
	effectRecycler->initRecyclableEffects(game, L"SKULLATTACKEFFECT", 40);


	Lightning *sampleLightning = new Lightning();

	sampleLightning->setSpriteType(lightningSS);
	sampleLightning->setAlpha(255);
	sampleLightning->setCurrentState(PRIMARY_FIRE);
	game->getGSM()->getPhyiscs()->initEnemyBullet(sampleLightning);

	bulletRecycler->registerBulletType(L"LIGHTNING", sampleLightning);
	bulletRecycler->initRecyclableBullets(game, L"LIGHTNING", 40);

	ExplosionEffect *sampleExplosion = new ExplosionEffect();
	sampleExplosion->setSpriteType(explosion);
	sampleExplosion->setAlpha(255);
	sampleExplosion->setCurrentState(L"EXPLOSION");

	effectRecycler->registerEffectType(L"EXPLOSION", sampleExplosion);
	effectRecycler->initRecyclableEffects(game, L"EXPLOSION", 40);

	DarkEnergyEffect *sampleDE = new DarkEnergyEffect();
	sampleDE->setSpriteType(darkenergySS);
	sampleDE->setAlpha(255);
	sampleDE->setCurrentState(L"IDLE");

	effectRecycler->registerEffectType(L"DARKENERGY", sampleDE);
	effectRecycler->initRecyclableEffects(game, L"DARKENERGY", 40);

	Laser *sampleLaser = new Laser();
	
	sampleLaser->setSpriteType(laserSS);
	sampleLaser->setAlpha(255);
	sampleLaser->setCurrentState(PRIMARY_FIRE);
	game->getGSM()->getPhyiscs()->initBullet(sampleLaser);


	bulletRecycler->registerBulletType(L"LASER", sampleLaser);
	bulletRecycler->initRecyclableBullets(game, L"LASER", 40);

	Trishot *sampleTrishot = new Trishot();

	sampleTrishot->setSpriteType(triSS);
	sampleTrishot->setAlpha(255);
	sampleTrishot->setCurrentState(PRIMARY_FIRE);
	game->getGSM()->getPhyiscs()->initBullet(sampleTrishot);


	bulletRecycler->registerBulletType(L"TRISHOT", sampleTrishot);
	bulletRecycler->initRecyclableBullets(game, L"TRISHOT", 40);




	ForceBullet *sampleForceBullet = new ForceBullet();
	sampleForceBullet->setSpriteType(forceBulletSS);
	sampleForceBullet->setAlpha(255);
	sampleForceBullet->setCurrentState(PRIMARY_FIRE);
	game->getGSM()->getPhyiscs()->initBullet(sampleForceBullet);
	bulletRecycler->registerBulletType(L"FORCE_BULLET", sampleForceBullet);
	bulletRecycler->initRecyclableBullets(game, L"FORCE_BULLET", 40);


	Rocket *sampleRocket = new Rocket();
	sampleRocket->setSpriteType(rocketSS);
	sampleRocket->setAlpha(255);
	sampleRocket->setCurrentState(PRIMARY_FIRE);
	game->getGSM()->getPhyiscs()->initBullet(sampleRocket);

	bulletRecycler->registerBulletType(L"ROCKET", sampleRocket);
	bulletRecycler->initRecyclableBullets(game, L"ROCKET", 40);


	Fireball *sampleFireball = new Fireball();

	sampleFireball->setSpriteType(fireballSS);
	sampleFireball->setAlpha(255);
	sampleFireball->setCurrentState(PRIMARY_FIRE);
	game->getGSM()->getPhyiscs()->initEnemyBullet(sampleFireball);

	bulletRecycler->registerBulletType(L"FIREBALL", sampleFireball);
	bulletRecycler->initRecyclableBullets(game, L"FIREBALL", 40);

	MageBullet *sampleMageBullet = new MageBullet();

	sampleMageBullet->setSpriteType(mageBulletSS);
	sampleMageBullet->setAlpha(255);
	sampleMageBullet->setCurrentState(PRIMARY_FIRE);
	game->getGSM()->getPhyiscs()->initEnemyBullet(sampleMageBullet);

	bulletRecycler->registerBulletType(L"MAGE_BULLET", sampleMageBullet);
	bulletRecycler->initRecyclableBullets(game, L"MAGE_BULLET", 40);


	AndromaliusBullet *sampleAndroMaliusBullet = new AndromaliusBullet();

	sampleAndroMaliusBullet->setSpriteType(andromaliusBulletSS);
	sampleAndroMaliusBullet->setAlpha(255);
	sampleAndroMaliusBullet->setCurrentState(PRIMARY_FIRE);
	game->getGSM()->getPhyiscs()->initEnemyBullet(sampleAndroMaliusBullet);

	bulletRecycler->registerBulletType(L"ANDROMALIUS_BULLET", sampleAndroMaliusBullet);
	bulletRecycler->initRecyclableBullets(game, L"ANDROMALIUS_BULLET", 40);


	LavaBurst *sampleLavaBurst = new LavaBurst();

	sampleLavaBurst->setSpriteType(lavaBurstSS);
	sampleLavaBurst->setAlpha(255);
	sampleLavaBurst->setCurrentState(PRIMARY_FIRE);
	game->getGSM()->getPhyiscs()->initLavaBurstBullet(sampleLavaBurst);

	bulletRecycler->registerBulletType(L"LAVA_BURST", sampleLavaBurst);
	bulletRecycler->initRecyclableBullets(game, L"LAVA_BURST", 100);

	LavaBall *sampleLavaBall = new LavaBall();

	sampleLavaBall->setSpriteType(lavaBallSS);
	sampleLavaBall->setAlpha(255);
	sampleLavaBall->setCurrentState(PRIMARY_FIRE);
	game->getGSM()->getPhyiscs()->initLavaBullet(sampleLavaBall);

	bulletRecycler->registerBulletType(L"LAVA_BALL", sampleLavaBall);
	bulletRecycler->initRecyclableBullets(game, L"LAVA_BALL", 40);

	int result = -1;


	if (levelInitFile == W_LEVEL_1_NAME)
	{
		result = luaPState->DoFile(LEVEL_1_LUA_PATH.c_str());
	}

	if (levelInitFile == W_LEVEL_2_NAME)
	{
		result = luaPState->DoFile(LEVEL_2_LUA_PATH.c_str());
	}

	if (levelInitFile == W_LEVEL_3_NAME)
	{
		result = luaPState->DoFile(LEVEL_3_LUA_PATH.c_str());
	}

	if (result != -1)
	{
		
		LuaObject pX = luaPState->GetGlobal("playerStartX");
		LuaObject pY = luaPState->GetGlobal("playerStartY");
		int playerX = pX.GetInteger();
		int playerY = pX.GetInteger();

		player->setInitPos(playerX, playerY);

		gsm->getPhyiscs()->initPlayer(player, playerX, playerY);
		gsm->getSpriteManager()->getPlayer()->initStatusSprite();
		gsm->getSpriteManager()->getPlayer()->initFlameThrower();


		this->loadBotsFromLua(levelInitFile, game);
	}
	

}

void OdysseyDataLoader::loadBotsFromLua(wstring levelName, Game *game)
{


	GameStateManager *gsm = game->getGSM();
	SpriteManager *spriteManager = gsm->getSpriteManager();
	Player *player = spriteManager->getPlayer();
	BotRecycler *recycler = spriteManager->getBotRecycler();
	
	LuaObject botTable = luaPState->GetGlobal("botTypes");

		AnimatedSpriteType *botSpriteType = NULL;

		for (LuaTableIterator it(botTable); it; it.Next())
		{
			const char* key = it.GetKey().GetString();
			LuaObject botType = botTable.GetByName(key);

			string bType(key);
			wstring bTypeW(bType.begin(), bType.end());

			if (bTypeW == W_SKULL_BOT)
			{
				botSpriteType = spriteManager->getSpriteType(3);
				SkullBot *sampleSkullBot = new SkullBot();

				sampleSkullBot->setSpriteType(botSpriteType);
				game->getGSM()->getPhyiscs()->initMeleeBot(sampleSkullBot, 32, 32, 25);
				sampleSkullBot->setHealth(100);

				recycler->registerBotType(W_SKULL_BOT, sampleSkullBot);
				recycler->initRecyclableBots(game, W_SKULL_BOT, 19);
			}
			if (bTypeW == W_SPIDER_BOT)
			{
				botSpriteType = spriteManager->getSpriteType(6);
				SpiderBot *sampleSpiderBot = new SpiderBot();
				sampleSpiderBot->setSpriteType(botSpriteType);
				game->getGSM()->getPhyiscs()->initMeleeBot(sampleSpiderBot, 64, 64, 60);
				sampleSpiderBot->setHealth(200);

				recycler->registerBotType(W_SPIDER_BOT, sampleSpiderBot);
				recycler->initRecyclableBots(game, W_SPIDER_BOT, 13);
			}

			if (bTypeW == W_LAVA_BOSS)
			{
				botSpriteType = spriteManager->getSpriteType(7);
				LavaBoss *sampleLavaBoss = new LavaBoss();

				sampleLavaBoss->setSpriteType(botSpriteType);
				game->getGSM()->getPhyiscs()->initLavaBoss(sampleLavaBoss, 192, 234, 50); // CHANGE VALUES LATER 
				sampleLavaBoss->setHealth(5000);

				recycler->registerBotType(W_LAVA_BOSS, sampleLavaBoss);
				recycler->initRecyclableBots(game, W_LAVA_BOSS, 2);
			}

			if (bTypeW == W_ANDROMALIUS_BOSS)
			{
				botSpriteType = spriteManager->getSpriteType(22);
				Andromalius *sampleAndromaliusBoss = new Andromalius();

				sampleAndromaliusBoss->setSpriteType(botSpriteType);
				game->getGSM()->getPhyiscs()->initLavaBoss(sampleAndromaliusBoss, 192, 234, 50); // CHANGE VALUES LATER 
				sampleAndromaliusBoss->setHealth(600);

				recycler->registerBotType(W_ANDROMALIUS_BOSS, sampleAndromaliusBoss);
				recycler->initRecyclableBots(game, W_ANDROMALIUS_BOSS, 2);
			}

			if (bTypeW == W_MAGE_BOSS)
			{
				botSpriteType = spriteManager->getSpriteType(8);
				MageBoss *sampleMageBoss = new MageBoss();

				sampleMageBoss->setSpriteType(botSpriteType);
				game->getGSM()->getPhyiscs()->initMageBoss(sampleMageBoss, 84, 96, 50); // CHANGE VALUES LATER 
				sampleMageBoss->setHealth(500);

				recycler->registerBotType(W_MAGE_BOSS, sampleMageBoss);
				recycler->initRecyclableBots(game, W_MAGE_BOSS, 2);
			}

			if (bTypeW == W_TIGER_BOT)
			{
				botSpriteType = spriteManager->getSpriteType(19);
				TigerBot *sampleTigerBot = new TigerBot();

				sampleTigerBot->setSpriteType(botSpriteType);
				game->getGSM()->getPhyiscs()->initMeleeBot(sampleTigerBot, 84, 96, 80); // CHANGE VALUES LATER 
				sampleTigerBot->setHealth(500);
				recycler->registerBotType(W_TIGER_BOT, sampleTigerBot);
				recycler->initRecyclableBots(game, W_TIGER_BOT, 13);
			}

			if (bTypeW == W_RAT_BOT)
			{
				botSpriteType = spriteManager->getSpriteType(20);
				RatBot *sampleRatBot = new RatBot();

				sampleRatBot->setSpriteType(botSpriteType);
				game->getGSM()->getPhyiscs()->initMeleeBot(sampleRatBot, 124, 35, 50); // CHANGE VALUES LATER 
				sampleRatBot->setHealth(500);
				recycler->registerBotType(W_RAT_BOT, sampleRatBot);
				recycler->initRecyclableBots(game, W_RAT_BOT, 2);
			}


			for (LuaTableIterator it2(botType); it2; it2.Next())
			{
				const char* key2 = it2.GetKey().GetString();
				LuaObject position = botType.GetByName(key2);

				int x = position.GetByName("x").GetInteger();
				int y = position.GetByName("y").GetInteger();


				Bot *bot = recycler->retrieveBot(game, bTypeW);

				if (bTypeW == W_SKULL_BOT)
				{
					bot->setSpriteType(botSpriteType);
					bot->setAlpha(255);
					bot->setCurrentState(L"IDLE");
					SkullBot *testBot = dynamic_cast<SkullBot*>(bot);
					spriteManager->addBot(testBot);
					game->getGSM()->getPhyiscs()->activateBot(testBot, x, y);
					int count = position.GetCount();
					if (position.GetByName("m").IsConvertibleToInteger())
						testBot->changeMovementType(position.GetByName("m").GetInteger());
				}

				if (bTypeW == W_SPIDER_BOT)
				{
					bot->setSpriteType(botSpriteType);
					bot->setAlpha(255);
					bot->setCurrentState(L"IDLE");
					SpiderBot *testBot = dynamic_cast<SpiderBot*>(bot);
					spriteManager->addBot(testBot);
					game->getGSM()->getPhyiscs()->activateBot(testBot, x, y);
					if (position.GetByName("m").IsConvertibleToInteger())
						testBot->changeMovementType(position.GetByName("m").GetInteger());

				}

				if (bTypeW == W_LAVA_BOSS)
				{
					bot->setSpriteType(botSpriteType);
					bot->setAlpha(255);
					bot->setCurrentState(L"SUBMERGE");
					LavaBoss *testBot = dynamic_cast<LavaBoss *>(bot);
					spriteManager->addBot(testBot);
					game->getGSM()->getPhyiscs()->activateBot(testBot, x, y);
					if (position.GetByName("m").IsConvertibleToInteger())
						testBot->changeMovementType(position.GetByName("m").GetInteger());
				}

				if (bTypeW == W_ANDROMALIUS_BOSS)
				{
					bot->setSpriteType(botSpriteType);
					bot->setAlpha(255);
					bot->setCurrentState(L"DOWN");
					Andromalius *testBot = dynamic_cast<Andromalius *>(bot);
					spriteManager->addBot(testBot);
					game->getGSM()->getPhyiscs()->activateBot(testBot, x, y);
					//if (position.GetByName("m").IsConvertibleToInteger())
					//	testBot->changeMovementType(position.GetByName("m").GetInteger());
				}

				if (bTypeW == W_MAGE_BOSS)
				{
					bot->setSpriteType(botSpriteType);
					bot->setAlpha(255);
					bot->setCurrentState(L"IDLE");
					MageBoss *testBot = dynamic_cast<MageBoss *>(bot);
					testBot->registerGame(game);
					testBot->setRangeX(300);
					testBot->setRangeY(300);
					spriteManager->addBot(testBot);
					bot->setDefaultX(x);
					bot->setDefaultY(y);
					game->getGSM()->getPhyiscs()->activateBot(testBot, x, y);
					if (position.GetByName("m").IsConvertibleToInteger())
						testBot->changeMovementType(position.GetByName("m").GetInteger());
				}
				if (bTypeW == W_TIGER_BOT)
				{
					bot->setSpriteType(botSpriteType);
					bot->setAlpha(255);
					bot->setCurrentState(L"IDLE");
					TigerBot *testBot = dynamic_cast<TigerBot *>(bot);
					spriteManager->addBot(testBot);
					game->getGSM()->getPhyiscs()->activateBot(testBot, x, y);
					if (position.GetByName("m").IsConvertibleToInteger())
						testBot->changeMovementType(position.GetByName("m").GetInteger());
				}

				if (bTypeW == W_RAT_BOT)
				{
					bot->setSpriteType(botSpriteType);
					bot->setAlpha(255);
					bot->setCurrentState(L"IDLE");
					RatBot *testBot = dynamic_cast<RatBot *>(bot);
					spriteManager->addBot(testBot);
					game->getGSM()->getPhyiscs()->activateBot(testBot, x, y);
					if (position.GetByName("m").IsConvertibleToInteger())
						testBot->changeMovementType(position.GetByName("m").GetInteger());
				}

			}
		}


	
	/*
	else if (levelName == W_LEVEL_2_NAME)
	{
		int result = luaPState->DoFile(LEVEL_2_LUA_PATH.c_str());
		LuaObject botTable = luaPState->GetGlobal("botTypes");
		LuaObject pX = luaPState->GetGlobal("playerStartX");
		LuaObject pY = luaPState->GetGlobal("playerStartY");
		int playerX = pX.GetInteger();
		int playerY = pX.GetInteger();

		gsm->getPhyiscs()->initPlayer(player, playerX, playerY);
		gsm->getSpriteManager()->getPlayer()->initStatusSprite();

		AnimatedSpriteType *botSpriteType = NULL;

		for (LuaTableIterator it(botTable); it; it.Next())
		{
			const char* key = it.GetKey().GetString();
			LuaObject botType = botTable.GetByName(key);

			string bType(key);
			wstring bTypeW(bType.begin(), bType.end());

			if (bTypeW == W_SKULL_BOT)
			{
				botSpriteType = spriteManager->getSpriteType(3);
				SkullBot *sampleSkullBot = new SkullBot();

				sampleSkullBot->setSpriteType(botSpriteType);
				game->getGSM()->getPhyiscs()->initMeleeBot(sampleSkullBot, 32, 32, 25);
				sampleSkullBot->setHealth(100);

				recycler->registerBotType(W_SKULL_BOT, sampleSkullBot);
				recycler->initRecyclableBots(game, W_SKULL_BOT, 13);

			}

			if (bTypeW == W_SPIDER_BOT)
			{
				botSpriteType = spriteManager->getSpriteType(6);
				SpiderBot *sampleSpiderBot = new SpiderBot();
				sampleSpiderBot->setSpriteType(botSpriteType);
				game->getGSM()->getPhyiscs()->initMeleeBot(sampleSpiderBot, 64, 64, 60);
				sampleSpiderBot->setHealth(200);

				recycler->registerBotType(W_SPIDER_BOT, sampleSpiderBot);
				recycler->initRecyclableBots(game, W_SPIDER_BOT, 13);
			}

			if (bTypeW == W_MAGE_BOSS)
			{
				botSpriteType = spriteManager->getSpriteType(8);
				MageBoss *sampleMageBoss = new MageBoss();

				sampleMageBoss->setSpriteType(botSpriteType);
				game->getGSM()->getPhyiscs()->initMageBoss(sampleMageBoss, 84, 96, 50); // CHANGE VALUES LATER 
				sampleMageBoss->setHealth(500);

				recycler->registerBotType(W_MAGE_BOSS, sampleMageBoss);
				recycler->initRecyclableBots(game, W_MAGE_BOSS, 2);
			}



			for (LuaTableIterator it2(botType); it2; it2.Next())
			{
				const char* key2 = it2.GetKey().GetString();
				LuaObject position = botType.GetByName(key2);

				int x = position.GetByName("x").GetInteger();
				int y = position.GetByName("y").GetInteger();

				Bot *bot = recycler->retrieveBot(game, bTypeW);

				if (bTypeW == W_SKULL_BOT)
				{
					bot->setSpriteType(botSpriteType);
					bot->setAlpha(255);
					bot->setCurrentState(L"IDLE");
					SkullBot *testBot = dynamic_cast<SkullBot*>(bot);
					spriteManager->addBot(testBot);
					game->getGSM()->getPhyiscs()->activateBot(testBot, x, y);
					//testBot->setWalkTicks(30);
				}

				if (bTypeW == W_SPIDER_BOT)
				{
					bot->setSpriteType(botSpriteType);
					bot->setAlpha(255);
					bot->setCurrentState(L"IDLE");
					SpiderBot *testBot = dynamic_cast<SpiderBot*>(bot);
					spriteManager->addBot(testBot);
					game->getGSM()->getPhyiscs()->activateBot(testBot, x, y);
				}

				if (bTypeW == W_MAGE_BOSS)
				{
					bot->setSpriteType(botSpriteType);
					bot->setAlpha(255);
					bot->setCurrentState(L"IDLE");
					MageBoss *testBot = dynamic_cast<MageBoss *>(bot);
					testBot->registerGame(game);
					spriteManager->addBot(testBot);
					testBot->setRangeX(412);
					testBot->setRangeY(412);
					bot->setDefaultX(x);
					bot->setDefaultY(y);
					game->getGSM()->getPhyiscs()->activateBot(testBot, x, y);
				}
			}
		}

	}
	else if (levelName == W_LEVEL_3_NAME)
	{
		int result = luaPState->DoFile(LEVEL_3_LUA_PATH.c_str());
		LuaObject botTable = luaPState->GetGlobal("botTypes");
		LuaObject pX = luaPState->GetGlobal("playerStartX");
		LuaObject pY = luaPState->GetGlobal("playerStartY");
		int playerX = pX.GetInteger();
		int playerY = pX.GetInteger();

		gsm->getPhyiscs()->initPlayer(player, playerX, playerY);
		gsm->getSpriteManager()->getPlayer()->initStatusSprite();

		AnimatedSpriteType *botSpriteType = NULL;

		for (LuaTableIterator it(botTable); it; it.Next())
		{
			const char* key = it.GetKey().GetString();
			LuaObject botType = botTable.GetByName(key);

			string bType(key);
			wstring bTypeW(bType.begin(), bType.end());

			if (bTypeW == W_SKULL_BOT)
			{
				botSpriteType = spriteManager->getSpriteType(3);
				SkullBot *sampleSkullBot = new SkullBot();

				sampleSkullBot->setSpriteType(botSpriteType);
				game->getGSM()->getPhyiscs()->initMeleeBot(sampleSkullBot, 32, 32, 25);
				sampleSkullBot->setHealth(100);

				recycler->registerBotType(W_SKULL_BOT, sampleSkullBot);
				recycler->initRecyclableBots(game, W_SKULL_BOT, 13);

			}

			if (bTypeW == W_SPIDER_BOT)
			{
				botSpriteType = spriteManager->getSpriteType(6);
				SpiderBot *sampleSpiderBot = new SpiderBot();
				sampleSpiderBot->setSpriteType(botSpriteType);
				game->getGSM()->getPhyiscs()->initMeleeBot(sampleSpiderBot, 64, 64, 60);
				sampleSpiderBot->setHealth(200);

				recycler->registerBotType(W_SPIDER_BOT, sampleSpiderBot);
				recycler->initRecyclableBots(game, W_SPIDER_BOT, 13);
			}

			if (bTypeW == W_MAGE_BOSS)
			{
				botSpriteType = spriteManager->getSpriteType(8);
				MageBoss *sampleMageBoss = new MageBoss();

				sampleMageBoss->setSpriteType(botSpriteType);
				game->getGSM()->getPhyiscs()->initMageBoss(sampleMageBoss, 84, 96, 50); // CHANGE VALUES LATER 
				sampleMageBoss->setHealth(500);

				recycler->registerBotType(W_MAGE_BOSS, sampleMageBoss);
				recycler->initRecyclableBots(game, W_MAGE_BOSS, 2);
			}



			for (LuaTableIterator it2(botType); it2; it2.Next())
			{
				const char* key2 = it2.GetKey().GetString();
				LuaObject position = botType.GetByName(key2);

				int x = position.GetByName("x").GetInteger();
				int y = position.GetByName("y").GetInteger();

				Bot *bot = recycler->retrieveBot(game, bTypeW);

				if (bTypeW == W_SKULL_BOT)
				{
					bot->setSpriteType(botSpriteType);
					bot->setAlpha(255);
					bot->setCurrentState(L"IDLE");
					SkullBot *testBot = dynamic_cast<SkullBot*>(bot);
					spriteManager->addBot(testBot);
					game->getGSM()->getPhyiscs()->activateBot(testBot, x, y);
					//testBot->setWalkTicks(30);
				}

				if (bTypeW == W_SPIDER_BOT)
				{
					bot->setSpriteType(botSpriteType);
					bot->setAlpha(255);
					bot->setCurrentState(L"IDLE");
					SpiderBot *testBot = dynamic_cast<SpiderBot*>(bot);
					spriteManager->addBot(testBot);
					game->getGSM()->getPhyiscs()->activateBot(testBot, x, y);
				}

				if (bTypeW == W_MAGE_BOSS)
				{
					bot->setSpriteType(botSpriteType);
					bot->setAlpha(255);
					bot->setCurrentState(L"IDLE");
					MageBoss *testBot = dynamic_cast<MageBoss *>(bot);
					testBot->registerGame(game);
					spriteManager->addBot(testBot);
					testBot->setRangeX(412);
					testBot->setRangeY(412);
					bot->setDefaultX(x);
					bot->setDefaultY(y);
					game->getGSM()->getPhyiscs()->activateBot(testBot, x, y);
				}
			}
		}


		//Hard coded movement patterns
		/*list<Bot*>::iterator botIterator;
		botIterator = game->getGSM()->getSpriteManager()->getBotsIterator();
		Bot *bot = (*botIterator);
		SkullBot* skBot = dynamic_cast<SkullBot*>(bot);
		skBot->changeMovementType(SimpleMovement::LINEAR_HORIZONTAL);
		botIterator++;
		bot = (*botIterator);
		skBot = dynamic_cast<SkullBot*>(bot);
		skBot->changeMovementType(SimpleMovement::LINEAR_VERTICAL);*/

	//}*/
}
// THESE ARE HARD-CODED EXAMPLES OF GUI DATA LOADING
void OdysseyDataLoader::hardCodedLoadGUIExample(Game *game)
{

}

// SOME HELPER METHODS FOR SETTING UP THE GAME
void OdysseyDataLoader::initColors(GameGraphics *graphics, map<wstring, wstring> *properties)
{
	int fontRed, fontGreen, fontBlue;
	wstring fontRedProp = (*properties)[W_FONT_COLOR_RED];
	wstring fontGreenProp = (*properties)[W_FONT_COLOR_GREEN];
	wstring fontBlueProp = (*properties)[W_FONT_COLOR_BLUE];
	wstringstream(fontRedProp) >> fontRed;
	wstringstream(fontGreenProp) >> fontGreen;
	wstringstream(fontBlueProp) >> fontBlue;

	// COLOR USED FOR RENDERING TEXT
	graphics->setFontColor(fontRed, fontGreen, fontBlue);

	int keyRed, keyGreen, keyBlue;
	wstring keyRedProp = (*properties)[W_COLOR_KEY_RED];
	wstring keyGreenProp = (*properties)[W_COLOR_KEY_GREEN];
	wstring keyBlueProp = (*properties)[W_COLOR_KEY_BLUE];
	wstringstream(keyRedProp) >> keyRed;
	wstringstream(keyGreenProp) >> keyGreen;
	wstringstream(keyBlueProp) >> keyBlue;

	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	// NOTE, IF YOU WISH TO USE PNG IMAGES THAT CONTAIN ALPHA
	// CHANNEL DATA, YOU DON'T NEED A COLOR KEY
	graphics->setColorKey(keyRed, keyGreen, keyBlue);
}
void OdysseyDataLoader::initViewport(GameGUI *gui, map<wstring, wstring> *properties)
{
	Viewport *viewport = gui->getViewport();

	int viewportOffsetX, viewportOffsetY, toggleOffsetY, screenWidth, screenHeight;
	wstring viewportOffsetXProp = (*properties)[W_VIEWPORT_OFFSET_X];
	wstring viewportOffsetYProp = (*properties)[W_VIEWPORT_OFFSET_Y];
	wstring toggleOffsetYProp = (*properties)[W_TOGGLE_OFFSET_Y];
	wstring screenWidthProp = (*properties)[W_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[W_SCREEN_HEIGHT];
	wstringstream(viewportOffsetXProp) >> viewportOffsetX;
	wstringstream(viewportOffsetYProp) >> viewportOffsetY;
	wstringstream(toggleOffsetYProp) >> toggleOffsetY;
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;
	int viewportWidth = screenWidth - viewportOffsetX;
	int viewportHeight = screenHeight - viewportOffsetY;
	viewport->setViewportWidth(viewportWidth);
	viewport->setViewportHeight(viewportHeight);
	viewport->setViewportOffsetX(viewportOffsetX);
	viewport->setViewportOffsetY(viewportOffsetY);
	viewport->setToggleOffsetY(toggleOffsetY);
	
}

// AND SOME HELPER METHODS FOR LOADING THE GUI
void OdysseyDataLoader::initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// SETUP THE CURSOR
	vector<unsigned int> *imageIDs = new vector<unsigned int>();
	int imageID;

	// - FIRST LOAD THE GREEN CURSOR IMAGE
	imageID = guiTextureManager->loadTexture(W_GREEN_CURSOR_PATH);
	imageIDs->push_back(imageID);

	

	// - NOW BUILD AND LOAD THE CURSOR
	Cursor *cursor = new Cursor();
	cursor->initCursor(imageIDs,
		*(imageIDs->begin()),
		0,
		0,
		0,
		255,
		32,
		32);
	gui->setCursor(cursor);
}
void OdysseyDataLoader::initSplashScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	SplashScreenGUI *splashScreenGUI = new SplashScreenGUI();

	// WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
	unsigned int normalTextureID = guiTextureManager->loadTexture(W_SPLASH_SCREEN_PATH);
	unsigned int mouseOverTextureID = normalTextureID;

	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 256;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = normalTextureID;
	splashScreenGUI->addOverlayImage(imageToAdd);


	/*OverlayImage* selectorOverlay = new OverlayImage();
	selectorOverlay->x = 20;
	selectorOverlay->y = 20;
	selectorOverlay->z = 0;
	selectorOverlay->alpha = 256;
	selectorOverlay->width = 32;
	selectorOverlay->height = 32;
	selectorOverlay->imageID = game->getGraphics()->getGUITextureManager()->loadTexture(L"data/gui/menu/Terminal_Arrow_Bold_Green.png");
	splashScreenGUI->addOverlayImage(selectorOverlay);
	splashScreenGUI->setSelector(selectorOverlay);*/
	// INIT THE QUIT BUTTON
	/*Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		0,
		0,
		0,
		255,
		game->getGraphics()->getScreenWidth(),
		game->getGraphics()->getScreenHeight(),
		false,
		W_GO_TO_MM_COMMAND);
	splashScreenGUI->addButton(buttonToAdd);*/

	// AND REGISTER IT WITH THE GUI
	//gui->getScreen(GS_SPLASH_SCREEN);

	gui->addScreenGUI(GS_SPLASH_SCREEN, splashScreenGUI);
}


void OdysseyDataLoader::initHelpScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager)
{

	SplashScreenGUI *splashScreenGUI = new SplashScreenGUI();

	// WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
	unsigned int normalTextureID = guiTextureManager->loadTexture(W_HELP_SCREEN_PATH);
	unsigned int mouseOverTextureID = normalTextureID;

	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 256;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = normalTextureID;
	splashScreenGUI->addOverlayImage(imageToAdd);

	gui->addScreenGUI(GS_MENU_CONTROLS_MENU, splashScreenGUI);
}

void OdysseyDataLoader::initVictoryScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager)
{

	SplashScreenGUI *victoryScreen = new SplashScreenGUI();

	victoryScreen->setTextHigh(7);
	victoryScreen->setTextLow(6);
	victoryScreen->setTextIndex(8);
	// WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
	unsigned int normalTextureID = guiTextureManager->loadTexture(W_WIN_SCREEN_PATH);
	unsigned int mouseOverTextureID = normalTextureID;

	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 256;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = normalTextureID;
	victoryScreen->addOverlayImage(imageToAdd);

	gui->addScreenGUI(GS_WIN_SCREEN, victoryScreen);
}

void OdysseyDataLoader::initDeathScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager)
{

	SplashScreenGUI *deathScreen = new SplashScreenGUI();

	deathScreen->setTextHigh(9);
	deathScreen->setTextLow(8);
	deathScreen->setTextIndex(9);
	// WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
	unsigned int normalTextureID = guiTextureManager->loadTexture(W_DEATH_SCREEN_PATH);
	unsigned int mouseOverTextureID = normalTextureID;

	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 256;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = normalTextureID;
	deathScreen->addOverlayImage(imageToAdd);

	gui->addScreenGUI(GS_GAME_OVER, deathScreen);
}

void OdysseyDataLoader::initLevelSelectScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	unsigned int normalTextureID = guiTextureManager->loadTexture(W_LEVEL_SELECT_SCREEN_PATH);

	ScreenGUI *levelSelectGUI = new ScreenGUI();

	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 256;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;//
	imageToAdd->imageID = normalTextureID;
	levelSelectGUI->addOverlayImage(imageToAdd);

	gui->addScreenGUI(GS_LEVEL_SELECT, levelSelectGUI);
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	AnimatedSpriteType *starST = game->getGSM()->getSpriteManager()->getSpriteType(0);


	unsigned int planet1Normal = guiTextureManager->loadTexture(W_LEVEL_ONE_NORMAL);
	unsigned int planet1Select1 = guiTextureManager->loadTexture(W_LEVEL_ONE_SELECT1);
	unsigned int planet1Select2 = guiTextureManager->loadTexture(W_LEVEL_ONE_SELECT2);
	unsigned int planet1Select3 = guiTextureManager->loadTexture(W_LEVEL_ONE_SELECT3);
	unsigned int planet1Select4 = guiTextureManager->loadTexture(W_LEVEL_ONE_SELECT4);
	unsigned int planet1Select5 = guiTextureManager->loadTexture(W_LEVEL_ONE_SELECT5);

	
	ShootingStar *star = new ShootingStar;
	star->setSpriteType(starST);
	star->setAlpha(255);
	star->setCurrentState(L"IDLE");
	game->getGSM()->getPhyiscs()->initSpriteBodyForLevelSelect(star, 1000, 50);
	spriteManager->addStar(star);

	int ids[5] = { planet1Select1, planet1Select2, planet1Select3, planet1Select4, planet1Select5 };


	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(planet1Normal, ids, 5, 200, 200, 0, 256, 300, 300, true, W_LEVEL_1_COMMAND);
	levelSelectGUI->addButton(buttonToAdd);

	unsigned int planet2Normal = guiTextureManager->loadTexture(W_LEVEL_TWO_NORMAL);
	unsigned int planet2Select1 = guiTextureManager->loadTexture(W_LEVEL_TWO_SELECT1);
	unsigned int planet2Select2 = guiTextureManager->loadTexture(W_LEVEL_TWO_SELECT2);
	unsigned int planet2Select3 = guiTextureManager->loadTexture(W_LEVEL_TWO_SELECT3);
	unsigned int planet2Select4 = guiTextureManager->loadTexture(W_LEVEL_TWO_SELECT4);
	unsigned int planet2Select5 = guiTextureManager->loadTexture(W_LEVEL_TWO_SELECT5);

	int ids2[5] = { planet2Select1, planet2Select2, planet2Select3, planet2Select4, planet2Select5 };

	buttonToAdd = new Button();
	buttonToAdd->initButton(planet2Normal, ids2, 5, 500, 500, 0, 256, 300, 300, true, W_LEVEL_2_COMMAND);
	levelSelectGUI->addButton(buttonToAdd);

	unsigned int planet3Normal = guiTextureManager->loadTexture(W_LEVEL_THREE_NORMAL);
	unsigned int planet3Select1 = guiTextureManager->loadTexture(W_LEVEL_THREE_SELECT1);
	unsigned int planet3Select2 = guiTextureManager->loadTexture(W_LEVEL_THREE_SELECT2);
	unsigned int planet3Select3 = guiTextureManager->loadTexture(W_LEVEL_THREE_SELECT3);
	unsigned int planet3Select4 = guiTextureManager->loadTexture(W_LEVEL_THREE_SELECT4);
	unsigned int planet3Select5 = guiTextureManager->loadTexture(W_LEVEL_THREE_SELECT5);

	int ids3[5] = { planet3Select1, planet3Select2, planet3Select3, planet3Select4, planet3Select5 };

	buttonToAdd = new Button();
	buttonToAdd->initButton(planet2Normal, ids3, 5, 700, 10, 0, 256, 300, 300, true, W_LEVEL_3_COMMAND);
	levelSelectGUI->addButton(buttonToAdd);


}

void OdysseyDataLoader::initPauseScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	ScreenGUI *pauseScreen = new ScreenGUI();
	pauseScreen->setTextHigh(7);
	pauseScreen->setTextLow(6);
	pauseScreen->setTextIndex(6);
	unsigned int imageID = guiTextureManager->loadTexture(W_PAUSE_SCREEN_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 256;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	pauseScreen->addOverlayImage(imageToAdd);
	gui->addScreenGUI(GS_PAUSED, pauseScreen);


}

void OdysseyDataLoader::initMainMenu(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	ScreenGUI *mainMenuGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(W_MAIN_MENU_PATH);
	OverlayImage *imageToAdd = new OverlayImage();//
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 256;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	mainMenuGUI->addOverlayImage(imageToAdd);

	// AND LET'S ADD AN EXIT BUTTON
	/*Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_PATH);
	int mouseOverTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_MO_PATH);

	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		412,
		500,
		0,
		255,
		200,
		100,
		false,
		W_EXIT_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD A START BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs

	normalTextureID = guiTextureManager->loadTexture(W_START_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_START_IMAGE_MO_PATH);

	// - INIT THE START BUTTON
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		412,
		350,
		0,
		255,
		200,
		100,
		false,
		W_START_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);*/

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_MAIN_MENU, mainMenuGUI);
	
}
//void initVictoryScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager);
void OdysseyDataLoader::initInGameGUI(Game* game, GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	ScreenGUI *inGameGUI = new ScreenGUI();
	gui->addScreenGUI(GS_GAME_IN_PROGRESS, inGameGUI);
	

	//Hud Stuff

	//Hud textures
	/*int* healthTextures = new int[5];
	healthTextures[0] = guiTextureManager->loadTexture(W_HEALTH_ZERO_PATH);
	healthTextures[1] = guiTextureManager->loadTexture(W_HEALTH_TWENTY_FIVE_PATH);
	healthTextures[2] = guiTextureManager->loadTexture(W_HEALTH_FIFTY_PATH);
	healthTextures[3] = guiTextureManager->loadTexture(W_HEALTH_SEVENTY_FIVE_PATH);
	healthTextures[4] = guiTextureManager->loadTexture(W_HEALTH_HUNDRED_PATH);*/
	int* livesTextures = new int[4];
	livesTextures[0] = guiTextureManager->loadTexture(W_LIVES_ZERO_PATH);
	livesTextures[1] = guiTextureManager->loadTexture(W_LIVES_ONE_PATH);
	livesTextures[2] = guiTextureManager->loadTexture(W_LIVES_TWO_PATH);
	livesTextures[3] = guiTextureManager->loadTexture(W_LIVES_THREE_PATH);

	unsigned int healthID = guiTextureManager->loadTexture(W_HEALTH_BAR);
	unsigned int healthBackID = guiTextureManager->loadTexture(W_HEALTH_BACK);

	game->initializeHud();
	//game->getHud()->initHealthBarStates(healthTextures);
	game->getHud()->initLivesBarStates(livesTextures);
	

	OverlayImage *imageToAdd3 = new OverlayImage();
	imageToAdd3->x = 20;
	imageToAdd3->y = 10;
	imageToAdd3->z = 0;
	imageToAdd3->alpha = 256;
	imageToAdd3->width = 320;
	imageToAdd3->height = 14;
	imageToAdd3->imageID = healthBackID;
	inGameGUI->addOverlayImage(imageToAdd3);
	game->getHud()->setHealthBar(imageToAdd3);

	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 30;
	imageToAdd->y = 10;
	imageToAdd->z = 0;
	imageToAdd->alpha = 256;
	imageToAdd->width = 300;
	imageToAdd->height = 14;
	imageToAdd->imageID = healthID;
	inGameGUI->addOverlayImage(imageToAdd);
	game->getHud()->setHealthBar(imageToAdd);

	OverlayImage *imageToAdd2 = new OverlayImage();
	imageToAdd2->x = 400;
	imageToAdd2->y = 10;
	imageToAdd2->z = 0;
	imageToAdd2->alpha = 256;
	imageToAdd2->width = 47;
	imageToAdd2->height = 13;
	imageToAdd2->imageID = livesTextures[3];
	inGameGUI->addOverlayImage(imageToAdd2);
	game->getHud()->setLivesBarOverlay(imageToAdd2);

	//game->getHud()->loseHealth();

}


