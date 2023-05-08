/*
	Author: Richard McKenna
	Stony Brook University
	Computer Science Department

	GameGraphics.cpp

	See GameGraphics.h for a class description.
	*/

#include "../../../stdafx.h"
#include "../game/Game.h"
#include "../game/IllegalArgumentException.h"
#include "../graphics/GameGraphics.h"
#include "../graphics/TextureManager.h"
#include "../gsm/state/GameStateManager.h"
#include "../gsm/world/World.h"
#include "../gui/GameGUI.h"
#include "../os/GameOS.h"
#include "../text/GameText.h"
#include "../text/TextFileWriter.h"

/*
	GameGraphics - Default constructor, nothing to initialize.
	*/
int frameCounter = -1;
int lastR = 255, lastG = 255, lastB = 255;
bool toggleAlpha = true, toggleRainbow = false;

GameGraphics::GameGraphics()
{
	debugTextShouldBeRendered = false;
}

/*
	~GameGraphics - Destructor, it cleans up the render lists and texture
	managers. This should only be called when the application is closing.
	*/
GameGraphics::~GameGraphics()
{
	delete guiRenderList;
	delete guiTextureManager;
	delete worldRenderList;
	delete worldTextureManager;
}

/*
	clearWorldTextures - When the game leaves a level we have to clear
	out these data structures. Calling clear on these will delete
	all the objects inside.
	*/
void GameGraphics::clearWorldTextures()
{
	// CLEAR LEVEL DATA STRUCURES
	worldTextureManager->clear();
	worldRenderList->clear();
}

/*
	fillRenderLists - This method causes the render lists to be
	filled with the things that have to be drawn this frame.
	*/
void GameGraphics::fillRenderLists(Game *game)
{
	// GENERATE RENDER LISTS FOR GAME WORLD AND GUI
	GameStateManager *gsm = game->getGSM();
	gsm->addGameRenderItemsToRenderList(game);
	GameGUI *gui = game->getGUI();
	gui->addRenderItemsToRenderList(game);
}

/*
	init - This method constructs the data structures for managing textures
	and render lists. It calls the createTextureManager, which is technology
	specific, and so is implemented only by child classes.
	*/
void GameGraphics::init(int initScreenWidth, int initScreenHeight)
{
	// INIT SCREEN DIMENSIONS
	screenWidth = initScreenWidth;
	screenHeight = initScreenHeight;

	// GUI TEXTURES (like buttons, cursor, etc.)
	guiRenderList = new RenderList(DEFAULT_INIT_GUI_RENDER_LIST_SIZE);
	guiTextureManager = createTextureManager();

	// LEVEL TEXTURES (like sprites, tiles, particles, etc.)
	worldRenderList = new RenderList(DEFAULT_INIT_LEVEL_RENDER_LIST_SIZE);
	worldTextureManager = createTextureManager();
}

/*
	renderText - This method will go through the GameText argument,
	pull out each TextToDraw object, and use a technology-specific
	method in a child class, renderTextToDraw, to render each
	piece of text.
	*/
void GameGraphics::renderText(GameText *text)
{
	// FOR NOW WE ONLY USE TEXT RENDERING FOR DEBUG TEXT, BUT
	// IT COULD ALSO BE USED FOR GAME TEXT LIKE HEALTH STATS
	if (debugTextShouldBeRendered)
	{
		/*int numTextObjects = text->getNumTextObjectsToDraw();
		for (int i = 0; i < numTextObjects; i++)
		{
			TextToDraw *textToDraw = text->getTextToDrawAtIndex(i);
			renderTextToDraw(textToDraw);
		}*/
		
	}

	
	if (game->getGSM()->getCurrentGameState() == GS_GAME_IN_PROGRESS)
	{
		TextToDraw *textToDraw = text->getTextToDrawAtIndex(5);
		game->getGraphics()->setFontColorAlpha(255, 255, 255, 0);
		renderTextToDraw(textToDraw);
	}

	if (game->getGSM()->getCurrentGameState() == GS_PAUSED)
	{

	}

	//Splash Screen Text FX
	if (game->getGSM()->getCurrentGameState() == GS_SPLASH_SCREEN)
	{

		TextToDraw *textToDraw = text->getTextToDrawAtIndex(0);

		if (toggleAlpha)
		{

			if (textToDraw->textAlpha == 255)
			{
				textToDraw->incrementer = -15;
			}

			if (textToDraw->textAlpha == 0)
			{
				textToDraw->incrementer = 15;
			}

			if (textToDraw->textAlphaCounter == 0)
			{
				textToDraw->textAlphaCounter = 2;
				textToDraw->textAlpha += textToDraw->incrementer;
			}
		}

		if (frameCounter > 10 || frameCounter == -1 && toggleRainbow)
		{
			lastR = rand() % 255;
			lastG = rand() % 255;
			lastB = rand() % 255;
			game->getGraphics()->setFontColorAlpha(textToDraw->textAlpha, lastR, lastG, lastB);
			frameCounter = 0;
		}
		else
			game->getGraphics()->setFontColorAlpha(textToDraw->textAlpha, lastR, lastG, lastB);

		if (toggleRainbow)
			frameCounter++;
		renderTextToDraw(textToDraw);
		if (toggleAlpha)
			textToDraw->textAlphaCounter = textToDraw->textAlphaCounter--;
	}

	if (game->getGSM()->getCurrentGameState() == GS_MAIN_MENU || game->getGSM()->getCurrentGameState() == GS_PAUSED || game->getGSM()->getCurrentGameState() == GS_GAME_OVER)
	{


		TextToDraw *highLight = text->getTextToDrawAtIndex(text->getTextIndex());

		for (int i = text->getTextLow(); i <= text->getTextHigh(); i++)
		{
			TextToDraw *textToDraw = text->getTextToDrawAtIndex(i);
			if (i > 0)
			{
				lastR = lastG = lastB = 255;//White
			}
			if (textToDraw == highLight)
			{
				if (textToDraw->textAlpha == 255)
				{
					textToDraw->incrementer = -15;
				}

				if (textToDraw->textAlpha == 0)
				{
					textToDraw->incrementer = 15;
				}

				if (textToDraw->textAlphaCounter == 0)
				{
					textToDraw->textAlphaCounter = 2;//
					textToDraw->textAlpha += textToDraw->incrementer;
					
				}
				game->getGraphics()->setFontColorAlpha(textToDraw->textAlpha, lastR, lastG, lastB);
				renderTextToDraw(textToDraw);
				textToDraw->textAlphaCounter = textToDraw->textAlphaCounter--;
			}
			else
			{
				game->getGraphics()->setFontColorAlpha(255, lastR, lastG, lastB);
				renderTextToDraw(textToDraw);
			}
		}
		


		





	}
		
		
}