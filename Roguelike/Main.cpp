// Roguelike tutorial taken from http://crawl.akrasiac.org/craig/
//  Need to implement creature class, character subclass

// project files
#include "Menus.h"
#include "Screen.h"
#include "RogueHeader.h"
#include "Map.h"
#include "Controls.h"

// include some libraries
#include <conio.h>
#include <string>
#include <iostream>

using namespace std;

// function protos
void printStartupMenu(Player&);
void printTitleScreen();
void CloseDoorCommand(Player&, Map&);
void OpenDoorCommand(Player&, Map&);
bool getInput(Player&, Map&);
void dispMenu(Player&);
void getCharInfo(Player&);
int printMenu(string[], int);
int printMenu(string[], int, int, int);
void getClass(Player&);
void getGender(Player&);

// used as a reference so you don't have to remember the int number
//enum tile {ROCKFLOOR, WALL, CLOSEDDOOR, OPENDOOR, GRASS, TREE};
//enum direction {NORTH, SOUTH, EAST, WEST};

int main(){
	// Console crap
	hConsoleOutput = GetStdHandle( STD_OUTPUT_HANDLE );
	GetConsoleCursorInfo( hConsoleOutput, &structCursorInfo );
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo( hConsoleOutput, &structCursorInfo );

	// Initialization
	Player p;
	Map current;
	current.load('1');

	// Title screen
	printTitleScreen();
	screen.clear();
	printStartupMenu(p);
	screen.clear();
	screen.drawUI();
	p.printCharacterInfo();

	// Prime
	current.drawMap();
	p.draw();
	// Main Game Loop
	while (getInput(p, current)){
		// There is no need to draw the world map every iteration
		// All that is needed is to move the player and replace the tile he moved from
		// Only two things need to be drawn, not a million
		// Draw the world map
		//current.drawMap();
		// Draw the player to the screen
		//p.draw();
		// Grab the user input
	}
}



bool getInput(Player& p, Map& m){
	/* note :	72 -> UP button
				75 -> RIGHT button
				77 -> LEFT button
				80 -> DOWN button
    */
	int nDeltaX = 0;
	int nDeltaY = 0;
	direction n;

		// Process the input
		switch (gIn()){
			// Rotate player
		case LEFT:
			n = p.getFace();
			if (n == NORTH)
				p.setFace(direction(WEST));
			else
				p.setFace(direction(n - 1));
			break;
			// Move up
		case UP:
			// Instead of moving a specific direction, simply move forward
			switch (p.getFace()){
			case NORTH:
				nDeltaY = -1;
				break;
			case SOUTH:
				nDeltaY = 1;
				break;
			case EAST:
				nDeltaX = 1;
				break;
			case WEST:
				nDeltaX= -1;
				break;
			}
			break;
			// Rotate player
		case RIGHT:
			n = p.getFace();
			if (n == WEST)
				p.setFace(NORTH);
			else
				p.setFace(direction(n + 1));
			break;
		case DOWN:
			p.interact(m);
			break;
		case MENU:
			dispMenu(p, m);
			screen.drawUI();
			p.printCharacterInfo();
			m.drawMap();
			p.draw();
			break;
			// Interact with whatever is in front of the dude
		case INTERACT:
			p.interact(m);
			break;
			// Quit
		case ESCAPE:
			return false;
			break;
		}
		// If we can move, move
		if (m.IsPassable(p.getXPos() + nDeltaX, p.getYPos() + nDeltaY)){
			p.move(p.getXPos() + nDeltaX, p.getYPos() + nDeltaY, m);
		}
		return true;
}

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