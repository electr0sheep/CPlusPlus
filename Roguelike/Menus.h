// I NEED TO REWORK THE MENUS WITH TOOLTIPS
// IF YOU DON'T PASS IT STRINGS OF ALL THE
// SAME LENGTH, YOU WILL HAVE LEFTOVERS ON
// THE SCREEN
/*
If I want to not use a string variable to work with menus, I think
I will need an array of char*s, or string literals
*/
#pragma once
#ifndef _MENUS_
#define _MENUS_

#include "RogueHeader.h"
#include "Screen.h"
#include "Map.h"
#include "Controls.h"
#include <iostream>
#include <string>

using namespace std;

// new function to display the tooltip
// dpsTip and clrTip should only be called by HOOKLEDORF
void dspTip(string s){
	screen.setPos(38,2);
	screen.setTextColor(BLACK, WHITE);
	//screen.setPos(38,2);
	cout << right << s << left;
}

// want a new dspTip that displays a tip at a specific
// location instead of a generic one
void dspTip(string s, int x, int y){
	screen.setPos(x, y);
	screen.setTextColor(BLACK, WHITE);
	cout << right << s << left;
}

void clrTip(int space, int x, int y){
	screen.setTextColor(BLACK, WHITE);
	screen.setPos(x, y);
	cout << right;
	for (int a = 0; a < space; a++)
		cout << " ";
	cout << left;
}

// 
void clrTip(int space){
	screen.setTextColor(BLACK, WHITE);
	screen.setPos(38,2);
	cout << right;
	for (int a = 0; a < space; a++)
		cout << " ";
	cout << left;
}

// THIS FUNCTION IS NOT MEANT TO BE CALLED BY CLIENT CODE
// ONLY FUNCTIONS WITHIN THIS HEADER
// bulk of function was taken from somewhere off the internet
int HOOKLEDORF(string m[], string tip[], int index, int x, int y){
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

// Reworked to make calls to a generic function
//  reduced code by 3/4
// These four functions print a selectable menu
//  CAN I MAKE IT SO I DON'T HAVE TO PASS STRING VARIABLES,
//  BUT CONSTANTS AS PARAMETERS?
//  MOST LIKELY, BUT "HELLO" WILL BE OF TYPE char*
int printMenu(string m[], int index){
	string *t = NULL;
	return HOOKLEDORF(m, t, index, 10, 0);
}

int printMenu(string m[], string tip[], int index){
	return HOOKLEDORF(m, tip, index, 10, 0);
}

int printMenu(string m[], int index, int x, int y){
	string *t = NULL;
	return HOOKLEDORF(m, t, index, x, y);
}

int printMenu(string m[], string tip[], int index, int x, int y){
	return HOOKLEDORF(m, tip, index, x, y);
}

/*
int printMenu(string m[], string tip[], int index, int xMenu, int yMenu, int xTip, int yTip, int length){
	
}
*/

// called when the user presses 'm'
void dispMenu(Player& p, Map& m){
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

// first thing that pops up
void printTitleScreen(){
	screen.setTextColor(BLACK, YELLOW);
	// screwed up because double \\ don't print
	cout <<"===================      ============  ===========  //=======\\  ===    === \n"
		 <<"||        ||||   ||      ||        || //        || //        |||| ||  || ||\n"
		 <<"||    ======||   ||      ||    ======||     //====||    //==// || ||  || ||\n"
		 <<"||    ||    ||   ||      ||    ||    ||    ||     ||    \\==\\   || ||  || ||\n"
		 <<"||    ====  ||   ||      ||    ====  ||    ||     \\         || ||  ====  ||\n"
		 <<"||      ||  ||   ||      ||      ||  ||    ||      \\==\\     || ||  ====  ||\n"
		 <<"||    ====  ||   ||      ||    ====  ||    ||          //   // || ||  || ||\n"
		 <<"||    ||    ||   ||      ||    ||    ||    ||       ////   //  || ||  || ||\n"
		 <<"||    ======||   ========||    ======||     \\==== ||     //    || ||  || ||\n"
		 <<"||        ||||         ||||        || \\        ||  \\===//       ===    ===\n"
		 <<"=====================================  ==========\n"
		 <<"===========================        ==========    ====================  ======\n"
		 <<"||           ||||          \\     //          \\   ||      ||||      || //     \\\n"
		 <<"=====     =====||   |==|   ||    //            \\ ||   =====||   =====||   () ||\n"
		 <<"    ||   ||    ||   |==|   ||  ||      ||      ||||   ||   ||   ||   ||     //\n"
		 <<"    ||   ||    ||         //   ||     (||)     ||||   ==== ||   ==== ||   ==\n"
		 <<"    ||   ||    ||         \\    ||    ((||))    ||||     || ||     || ||  ||\n"
		 <<"    ||   ||    ||   ||\\    \\   ||     (||)     ||||   ==== ||   ==== ||  ||\n"
		 <<"    ||   ||    ||   || \\    \\  ||      ||      ||||   ||   ||   ||   ||  ||\n"
		 <<"    ||   ||    ||   ||  \\    \\  \\            //  ||   =====||   =====||  ||\n"
		 <<"    ||   ||    ||   ||   \\    \\  \\          //   ||      ||||      ||||  ||\n"
		 <<"    =======     =====     =====   ==========     =========================";
	gIn();
}

// see the below comment, except this gets the class
void getClass(Player& p){
	int choice;
	string s[] = {"Fighter", "Thief", "Wizard"};
	string tip[] = {"A real tough guy", "A sneaky terd", "He knows magic"};
	cout << "What class are you?: ";
	do{
		choice = printMenu(s, tip, 3, 20, 15); 
	} while (choice == -1);
	p.setClass(s[choice]);
}

// gets the players gender, I need to take this function out
//  it is more confusing than helpful
void getGender(Player& p){
	int choice;
	string s[] = {"Male", "Female"};
	cout << "What gender are you?: ";
	do{
		choice = printMenu(s, 2, 20, 15);
	} while (choice == -1);
	p.setGender(s[choice]);
}

// occurs when the player selects "new game" instead of load
void getCharInfo(Player& p){
	screen.clear();
	screen.setTextColor(BLACK, WHITE);
	string s;
	screen.setPos(20,10);
	cout << "Welcome to the world of awesome";
	gIn();
	screen.clear();
	screen.setPos(20,10);
	cout << "What is your name?: ";
	cin >> s;
	p.setName(s);
	screen.clear();
	screen.setPos(20,10);
	getClass(p);
	screen.clear();
	screen.setPos(20,10);
	getGender(p);
}

// menu that occurs right after the title screen
void printStartupMenu(Player& p){
	screen.setTextColor(BLACK, WHITE);
	string s[] = {"New Game", "Load Game"};
	int choice;
	do{
		choice = printMenu(s, 2);
		if (choice == 1)
			choice = p.load();
		else
			getCharInfo(p);
	} while (choice == -1);
}
#endif /* _MENUS_ */