//The purpose of this file is to replace ConsoleManip.h
#pragma once
#ifndef _SCREEN_
#define _SCREEN_

#include <string>
#include <iostream>
#include <Windows.h>
#include "Controls.h"
#include "Menus.h"
#include "Map.h"
#include "RogueHeader.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

using namespace std;

int printMenu(string*, string*, int);

enum color {WHITE, BLACK, YELLOW,  LIGHTPURPLE, RED, BLUE, GREEN, BROWN, DARKBLUEGREEN, DARKBLUE, DARKGREEN, DARKPURPLE, TAN, GRAY, DARKGRAY, LIGHTBLUEGREEN};

HANDLE con=GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hConsoleOutput;
CONSOLE_CURSOR_INFO structCursorInfo;

class Screen{
private:
	void dspTip(string);
	void clrTip(int);
	int HOOKLEDORF(string*, string*, int, int, int);
public:
	void setPos(int, int);
	void setTextColor(color, color);
	void clear();
	void drawTextBox(string);
	int drawTextBox(string, int, int, int);
	void clearTextBox(int, int, int, int);
	void drawUI();
	void drawBattleScreen();
	int printMenu(string*, int);
	int printMenu(string*, string*, int);
	int printMenu(string*, int, int, int);
	int printMenu(string*, string*, int, int, int);
void dispMenu(Player&, Map&);
} screen;

void Screen::dispMenu(Player& p, Map& m){
	int choice = 0;
	screen.clear();
	string ar[] = {"Save", "Help"};
	while (choice != -1){
		screen.clear();
		choice = printMenu(ar, 2);
		switch (choice){
		case 0:
			p.save(m.getCurrent());
			m.save();
			screen.drawTextBox("Game Saved!");
			choice = -1;
			break;
		case 1:
			int helpChoice = 0;
			string menu[] = {"Why does this game suck?", "What is this?", "Is there a point to this?", "Who's responsible for this?", "Controls"};
			helpChoice = printMenu(menu, 5);
			switch (helpChoice){
			case 0:
				screen.drawTextBox("Personally, I find hours of enjoyment opening and closing doors, and doing doughnuts by pressing turn left and move, and seeing if I can move to every tile without retracing my steps");
				break;
			case 1:
				screen.drawTextBox("This is a rogue-like-like game");
				break;
			case 2:
				screen.drawTextBox("Yes");
				break;
			case 3:
				screen.drawTextBox("electrosheep");
				break;
			case 4:
				screen.drawTextBox("Move: up arrow, w, 8");
				screen.drawTextBox("Turn left: left arrow, q, 4");
				screen.drawTextBox("Turn right: right arrow, e, 6");
				screen.drawTextBox("Interact: down arrow, enter");
				screen.drawTextBox("Back or quit: escape key");
				break;
			}
			break;
		}
	}
}

int Screen::printMenu(string m[], int index){
	string *t = NULL;
	return HOOKLEDORF(m, t, index, 10, 0);
}

int Screen::printMenu(string m[], string tip[], int index){
	return HOOKLEDORF(m, tip, index, 10, 0);
}

int Screen::printMenu(string m[], int index, int x, int y){
	string *t = NULL;
	return HOOKLEDORF(m, t, index, x, y);
}

int Screen::printMenu(string m[], string tip[], int index, int x, int y){
	return HOOKLEDORF(m, tip, index, x, y);
}

int Screen::HOOKLEDORF(string m[], string tip[], int index, int x, int y){
	// variable i stores the current choice
	int i;

	// list the menu
	for (i = 0; i < index; i++){
		screen.setPos(x, y + (i * 2));
		screen.setTextColor(BLACK, WHITE);
		cout <<m[i];
	}

	i = 0;
	while (true){
		// THIS IS THE WHITE PART
		screen.setPos(x, y + (i * 2));
		screen.setTextColor(WHITE, BLACK);
		cout << m[i];

		if (tip != NULL){
			dspTip(tip[i]);
		}

		switch(gIn()){
		case ESCAPE:
			return -1;
		case UP:
			if (tip != NULL)
				clrTip(tip[i].length());
			if(i>0){
				screen.setTextColor(BLACK, WHITE);
				screen.setPos(x, y + (i * 2));
				cout << m[i];
				i--;
			}
			break;
		case DOWN:
			if (tip != NULL)
				clrTip(tip[i].length());
			if(i < index - 1){
				screen.setTextColor(BLACK, WHITE);
				screen.setPos(x, y + (i * 2));
				cout << m[i];
				i++;
			}
			break;
		case INTERACT:
			screen.setTextColor(BLACK, WHITE);
			return i;
		}
	}
}

void Screen::clrTip(int space){
	screen.setTextColor(BLACK, WHITE);
	screen.setPos(38,2);
	cout << right;
	for (int a = 0; a < space; a++)
		cout << " ";
	cout << left;
}

void Screen::dspTip(string s){
	screen.setPos(38,2);
	screen.setTextColor(BLACK, WHITE);
	cout << right << s << left;
}

void Screen::setPos(int x,int y){
    COORD Coord;
    Coord.X=x;
    Coord.Y=y;
 
    SetConsoleCursorPosition(con,Coord);
}

// simply sets the color of the text
// first hex number indicates background color
// next hex number indicates text color
/* color values
0 = black
1 = dark purple/blue
2 = green
3 = light blue
4 = dark red/brown
A = green
B = blue
C = red
D = purple
E = yellow
F = white
*/

// sets the text color
void Screen::setTextColor(color background, color foreground){
	int newColor;
	// first set the background
	switch (background){
	case WHITE:
		newColor = 0xF0;
		break;
	case YELLOW:
		newColor = 0xE0;
		break;
	case LIGHTPURPLE:
		newColor = 0xD0;
		break;
	case RED:
		newColor = 0xC0;
		break;
	case BLUE:
		newColor = 0xB0;
		break;
	case GREEN:
		newColor = 0xA0;
		break;
	case LIGHTBLUEGREEN:
		newColor = 0x90;
		break;
	case DARKGRAY:
		newColor = 0x80;
		break;
	case GRAY:
		newColor = 0x70;
		break;
	case TAN:
		newColor = 0x60;
		break;
	case DARKPURPLE:
		newColor = 0x50;
		break;
	case BROWN:
		newColor = 0x40;
		break;
	case DARKBLUEGREEN:
		newColor = 0x30;
		break;
	case DARKGREEN:
		newColor = 0x20;
		break;
	case DARKBLUE:
		newColor = 0x10;
	case BLACK:
		newColor = 0x00;
		break;
	}
	// then the foreground
	switch (foreground){
	case WHITE:
		newColor += 0x0F;
		break;
	case YELLOW:
		newColor += 0x0E;
		break;
	case LIGHTPURPLE:
		newColor += 0x0D;
		break;
	case RED:
		newColor += 0x0C;
		break;
	case BLUE:
		newColor += 0x0B;
		break;
	case GREEN:
		newColor += 0x0A;
		break;
	case LIGHTBLUEGREEN:
		newColor += 0x09;
		break;
	case DARKGRAY:
		newColor += 0x08;
		break;
	case GRAY:
		newColor += 0x07;
		break;
	case TAN:
		newColor += 0x06;
		break;
	case DARKPURPLE:
		newColor += 0x05;
		break;
	case BROWN:
		newColor += 0x04;
		break;
	case DARKBLUEGREEN:
		newColor += 0x03;
		break;
	case DARKGREEN:
		newColor += 0x02;
		break;
	case DARKBLUE:
		newColor += 0x01;
	// NOTE: there e is no need to set the black foreground, because it has been set in the background part
	case BLACK:
		newColor += 0x00;
		break;
	}
    SetConsoleTextAttribute(con, newColor);
}

// clears all characters from the screen, leaving it blank
void Screen::clear(){
	this->setTextColor(BLACK, WHITE);
	// Change the screen mode to allow wrapping
	SetConsoleMode(hConsoleOutput, ENABLE_WRAP_AT_EOL_OUTPUT);
	// Read the current attribute
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &info);
	
	// Clear the screen
	DWORD dwWritten;
	COORD dwCoord = {0, 0};
	FillConsoleOutputCharacter(hConsoleOutput, ' ', SCREEN_WIDTH * SCREEN_HEIGHT, dwCoord, &dwWritten);
	FillConsoleOutputAttribute(hConsoleOutput, info.wAttributes, SCREEN_WIDTH * SCREEN_HEIGHT, dwCoord, &dwWritten);

	// Move cursor back to start of the screen (coordinates 0,0)
	this->setPos(0, 0);

	// Change back to the normal console mode
	SetConsoleMode(hConsoleOutput, ENABLE_PROCESSED_OUTPUT);
}

// displays a generic text box
void Screen::drawTextBox(string s){
	// length is used to clear the string
	int length = s.length();
	int line = 0;
	this->setPos(56,0);
	// if we don't need to wrap
	if (length < 25)
		cout << s;
	else{
		int beginOfWord = 0;
		int lengthCurrentWord;
		int indexOfNewLine = 0;
		for (int a = 0; a < length; a++){
			// two things indicate a whole word
			// 1: a space
			// 2: the end of the string
			if (s[a] == ' ' || a == length - 1){
				lengthCurrentWord = a - beginOfWord;
				// if there is plenty of room for the word
				if (a < 24 + indexOfNewLine){
					for (int i = beginOfWord; i <= a; i++)
						cout << s[i];
				// if it'll just fit
				}else if (a % (24 +indexOfNewLine) == 0){
					for (int i = beginOfWord; i < a; i++)
						cout << s[i];
				// if it won't fit
				}else{
					indexOfNewLine = beginOfWord;
					line ++;
					this->setPos(56, line);
					for (int i = beginOfWord; i <= a; i++)
						cout << s[i];
				}
				beginOfWord = a + 1;
			}
		}
	}
	gIn();
	screen.setTextColor(BLACK, WHITE);
	this->setPos(56,0);
	if (length < 25)
		for (int a = 0; a < length; a++)
			cout << " ";
	else{
		for (int a = 0; a <= line; a ++){
			this->setPos(56, a);
			for (int b = 0; b < 25; b++)
				cout << " ";
		}
	}
}


// this one returns an integer value representing the "height" of the text box
// this is so that the clearTextBox function can be called later to clear the text
int Screen::drawTextBox(string s, int x, int y, int textBoxLength){
	// length is used to clear the string
	int length = s.length();
	int line = 0;
	this->setPos(x, y);
	// if we don't need to wrap
	if (length < textBoxLength)
		cout << s;
	else{
		int beginOfWord = 0;
		int lengthCurrentWord;
		int indexOfNewLine = 0;
		for (int a = 0; a < length; a++){
			// two things indicate a whole word
			// 1: a space
			// 2: the end of the string
			if (s[a] == ' ' || a == length - 1){
				lengthCurrentWord = a - beginOfWord;
				// if there is plenty of room for the word
				if (a < (textBoxLength - 1) + indexOfNewLine){
					for (int i = beginOfWord; i <= a; i++)
						cout << s[i];
				// if it'll just fit
				}else if (a % ((textBoxLength - 1) + indexOfNewLine) == 0){
					for (int i = beginOfWord; i < a; i++)
						cout << s[i];
				// if it won't fit
				}else{
					indexOfNewLine = beginOfWord;
					line ++;
					this->setPos(x, y + line);
					for (int i = beginOfWord; i <= a; i++)
						cout << s[i];
				}
				beginOfWord = a + 1;
			}
		}
	}
	return line;
}

void Screen::clearTextBox(int length, int x, int y, int line){
	screen.setTextColor(BLACK, WHITE);
	for (int a = y; a <= y + line; a++){
		this->setPos(x, a);
		for (int b = x; b <= x + length; b++)
			cout << " ";
	}
}

// sets up the framework of the screen
void Screen::drawUI(){
	int x = 55, y;
	this->setTextColor(BLACK, WHITE);
	// first print the outline of the menus
	for (y = 0; y < 24; y++){
		this->setPos(x,y);
		cout << "|";
	}
	y = 12;
	for (x = 56; x < 80; x++){
		this->setPos(x,y);
		cout << "-";
	}
}

/*HOW I WANT THE BATTLE SCREEN
---------------------------------------------------------------------------
|                    i                                                     |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
---------------------------------------------------------------------------
*/
void Screen::drawBattleScreen(){
	screen.clear();
	string text = "Holy crap it's battle time!!!!";
	screen.clearTextBox(15, 0, 10, screen.drawTextBox(text, 0, 10, 15));
/*	int choice;
	string commands[] = {"ATTACK", "DEFEND", "MAGIC"};
	string tips[] = {"Attacks the current target", "Prepare for an attack, increasing armor by 70%", "Select a spell to cast"};
	screen.clear();
	screen.drawUI();
	do
		choice = printMenu(commands, tips, 3);
	while(choice == -1);
	*/
}
#endif  /* _SCREEN_ */