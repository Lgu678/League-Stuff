#pragma once
#include "ScreenGUI.h";
class SplashScreenGUI :
	public ScreenGUI
{
public:
	SplashScreenGUI();
	SplashScreenGUI(ScreenGUI* screen);
	~SplashScreenGUI();
	
	void setSelector(OverlayImage* selector) { _selector = selector; }
	OverlayImage* getSelector() { return _selector; }
private:
	OverlayImage* _selector;

};

