/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameText.h

	This class allows for easy rendering of text
	to the game screen.
*/

#pragma once
#include "../../../stdafx.h"

class Game;
class TextGenerator;

const int MAX_TEXT_OBJECTS = 10;

class TextToDraw
{
private:
	wstring *text;

public:
	TextToDraw()	{}
	~TextToDraw()	{}
	int maxChars;
	int numChars;
	int x;
	int y;
	int width;
	int height;
	int	textAlphaCounter;
	int	textAlpha;
	int incrementer = 15;

	void setText(wstring *textToSet)
	{
		text = textToSet;
	}
	wstring* getText() { return text; }
};

class GameText
{
private:
	int textLow;
	int textHigh;
	int textIndex;
	vector<TextToDraw*> *textToDraw;
	TextGenerator *textGenerator;

	// USED TO PRINT DEBUGGING OUTPUT
	TextFileWriter writer;

public:
	// INLINED ACCESSOR METHODS
	int				getNumTextObjectsToDraw()		{	return textToDraw->size();		}
	TextGenerator*	getTextGenerator()				{	return textGenerator;			}
	TextToDraw*		getTextToDrawAtIndex(int index)	{	return textToDraw->at(index);	}	

	int getTextIndex()
	{
		return textIndex;
	}

	void setTextIndex(int x)
	{
		textIndex = x;
	}

	int getTextLow()
	{
		return textLow;
	}

	void setTextLow(int x)
	{
		textLow = x;
	}

	int getTextHigh()
	{
		return textHigh;
	}

	void setTextHigh(int x)
	{
		textHigh = x;
	}



	

	// INLINED MUTATOR METHOD

	

	void setTextGenerator(TextGenerator *initTextGenerator)
	{
		textGenerator = initTextGenerator;
	}

	// METHODS DEFINED IN GameText.cpp
	GameText();
	~GameText();
	void addText(wstring *textToAdd, int initX, int initY, int initWidth, int initHeight);
	void changeTextOnly(wstring *textToAdd, int index);
	void initDebugFile(string debugFileName);
	void moveText(int index, int xMove, int yMove);
	void writeDebugOutput(string output);
};