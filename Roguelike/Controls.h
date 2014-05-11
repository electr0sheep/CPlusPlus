// I have decided to implement a universal control
//  scheme for the menus, and everything.
#pragma once
#ifndef _CONTROLS_
#define _CONTROLS_

#include <conio.h>

// BAD is what occurs when the user presses something we don't know what is
enum control {BAD, UP, DOWN, LEFT, RIGHT, ESCAPE, INTERACT, MENU};

// no other code should ever call _getch()
control gIn(){
	switch (_getch()){
	case '7':
	case 'q':
	case 'Q':
left:
		return LEFT;
	case '8':
	case 'w':
	case 'W':
up:
		return UP;
	case '9':
	case 'e':
	case 'E':
right:
		return RIGHT;
	case '2':
	case 's':
	case 'S':
down:
		return DOWN;
	case 'm':
	case 'M':
		return MENU;
	case 13:
		return INTERACT;
		// arrow keys are a bit tricky
		//  I have decided to commit programming suicide and
		//  use a goto statement, because I think that it will
		//  be more readable than having three nested switches
	case 224:
		switch(_getch()){
		case 72:
			goto up;
		case 75:
			goto left;
		case 77:
			goto right;
		case 80:
			goto down;
		}
		// Quit
	case 27:
		return ESCAPE;
	}
	return BAD;
}
#endif /* _CONTROLS_ */