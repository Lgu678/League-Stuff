#include "stdafx.h"
#include "src\sssf\gui\OverlayImage.h"

class Hud {
	/* Health Bar
	Hundred - 4
	Seventy Five - 3
	Fifty - 2
	Twenty Five - 1
	Zero - 0
	*/

	/* Lives Bar
	Three - 3
	Two - 2
	One - 1
	Zero - 0
	*/
	/*
public :
	void initHealthBarStates(int n[]) { healthBarStates = n; }
	void initLivesBarStates(int n[]) { livesBarStates = n; }
	void setHealthBarOverlay(OverlayImage* overlayImage) { healthBar = overlayImage;  }
	void setLivesBarOverlay(OverlayImage* overlayImage) { livesBar = overlayImage; }
	bool isOutOfLives() { return outOfLives;  }

	Hud() {  }
	~Hud() {}
	void resetHud() 
	{ 
		currentHealthBarState = 4; currentLivesBarState = 3; outOfLives = false; 
		livesBar->imageID = livesBarStates[currentLivesBarState];
		healthBar->imageID = healthBarStates[currentHealthBarState];
	}
	void loseHealth()
	{		
		if (currentHealthBarState > 0)
		{
			currentHealthBarState--;
		}
		else
		{
			if (currentLivesBarState > 0)
			{
				currentLivesBarState--;
				currentHealthBarState = 4;
			}
			else
				outOfLives = true;
		}

		livesBar->imageID = livesBarStates[currentLivesBarState];
		healthBar->imageID = healthBarStates[currentHealthBarState];
	}

private:
	OverlayImage* healthBar;
	OverlayImage* livesBar;
	int* healthBarStates;
	int* livesBarStates;
	int currentHealthBarState;
	int currentLivesBarState;
	bool outOfLives;
	*/

public:
	Hud() { healthWidth = 300; }
	~Hud() {}
	void initLivesBarStates(int n[]) { livesBarStates = n; }
	void setLivesBarOverlay(OverlayImage* overlayImage) { livesBar = overlayImage; }
	void setHealthBar(OverlayImage* overlayImage) { healthBar = overlayImage; }
	void loseLife()
	{
		if (currentLivesBarState > 0)
		{
			currentLivesBarState--;
			livesBar->imageID = livesBarStates[currentLivesBarState];
		}
	}

	void resetHud()
	{
		currentLivesBarState = 3;
		healthWidth = 300;
		if (healthBar != NULL)
			healthBar->width = 300;

		livesBar->imageID = livesBarStates[currentLivesBarState];
	}

	// WHERE X IS CURRENT HEALTH, AND Y IS THE PLAYERS STARTING HEALTH
	void setHealthWidth(int x, int y)
	{
		float div = (float)x / (float)y;
		int ratio = (div) * 100;
		healthWidth = (300 * ratio) / 100;
		if (healthWidth < 0)
			healthBar->width = 0;
		else
		healthBar->width = healthWidth;
	}
	int getHealthWidth()
	{
		return healthWidth;
	}
private:
	unsigned int healthWidth;
	int* livesBarStates;
	OverlayImage* healthBar;
	OverlayImage* livesBar;
	int currentLivesBarState;

};