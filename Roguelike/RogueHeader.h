// Contains data for the Player class, which is getting increasingly complex, and clutters my main
// Make sure the <string> has been included
#pragma once
#ifndef _ROGUEHEADER_
#define _ROGUEHEADER_

#include <string>
#include "Map.h"
#include "Screen.h"
#include <iostream>
#include <fstream>

using namespace std;

enum direction {NORTH, EAST, SOUTH, WEST};

// our lovely player class
class Player{
	direction face;
	string name;
	string clas;
	string gender;
	int xPos, yPos;
	int prevX, prevY;
	char icon;
	int strength;
	int intel;
	int dex;
	int hp;
	int mp;
	int level;
	int xp, toNextLevel;
	color characterColor;
public:
	Player();
	Player(string);
	void setName(string);
	void setClass(string);
	void setGender(string);
	void setPos(int, int);
	void setIcon(char);
	void setStrength(int);
	void setInt(int);
	void setDex(int);
	void setHP(int);
	void setMP(int);
	void setFace(direction);
	void setLevel(int);
	void setXP(int);
	void setNextLevel(int);
	void draw();
	string getName();
	string getClass();
	string getGender();
	int getXPos();
	int getYPos();
	char getIcon();
	int getStrength();
	int getInt();
	int getDex();
	int getHP();
	int getMP();
	direction getFace();
	int getLevel();
	int getXP();
	int getNextLevel();
	void interact(Map&);
	void move(int, int, Map&);
	void printCharacterInfo();
	void save(int);
	int load();
	void addXP(int);
	void levelUp();
};

Player::Player(){
	face = SOUTH;
	prevX = 4;
	prevY = 4;
	xPos = 4;
	yPos = 4;
	icon = 31;
	level = 1;
	xp = 0;
	toNextLevel = 200;
	characterColor = YELLOW;
}

Player::Player(string nName){
	name = nName;
	face = SOUTH;
	prevX = 4;
	prevY = 4;
	xPos = 4;
	yPos = 4;
	icon = 31;
	level = 1;
	xp = 0;
	toNextLevel = 200;
	characterColor = YELLOW;
}

// called when player wants to move
void Player::move(int x, int y, Map& m){
	if (x != xPos || y != yPos){
		setPos(x, y);
		draw();
		m.drawTile(prevX, prevY);
	}
}

// draws the location of the player
void Player::draw(){
		screen.setTextColor(BLACK, WHITE);
		screen.setPos(xPos, yPos);
		cout << icon;
}

// changes the direction the player is facing
void Player::setFace(direction n){
	switch (n){
	case NORTH:
		icon = 30;
		break;
	case SOUTH:
		icon = 31;
		break;
	case EAST:
		icon = 16;
		break;
	case WEST:
		icon = 17;
		break;
	}
	face = n;
	draw();
}

// this one, and any other function who's functionality
//  should be painfully obvious will not be afforded an explanation
direction Player::getFace(){
	return face;
}

void Player::setName(string n){
	name = n;
}

void Player::setClass(string n){
	clas = n;
}

void Player::setGender(string n){
	gender = n;
}

// this is actually what changes the position of the player
void Player::setPos(int x, int y){
	prevX = xPos;
	prevY = yPos;
	xPos = x;
	yPos = y;
}

void Player::setIcon(char c){
	icon = c;
}

void Player::setStrength(int s){
	strength = s;
}

void Player::setInt(int i){
	intel = i;
}

void Player::setDex(int d){
	dex = d;
}

void Player::setHP(int h){
	hp = h;
}

void Player::setMP(int m){
	mp = m;
}

string Player::getName(){
	return name;
}

string Player::getClass(){
	return clas;
}

string Player::getGender(){
	return gender;
}

int Player::getXPos(){
	return xPos;
}

int Player::getYPos(){
	return yPos;
}

char Player::getIcon(){
	return icon;
}

int Player::getStrength(){
	return strength;
}

int Player::getInt(){
	return intel;
}

int Player::getDex(){
	return dex;
}

int Player::getHP(){
	return hp;
}

int Player::getMP(){
	return mp;
}

// player wants to interact with whatever is in front
void Player::interact(Map& m){
	int x = 0, y = 0;
	int tileType;

	switch(face){
	case NORTH:
		y = -1;
		break;
	case WEST:
		x = -1;
		break;
	case EAST:
		x = 1;
		break;
	case SOUTH:
		y = 1;
		break;
	}
	// for reference 
	enum tile {ROCKFLOOR, WALL, CLOSEDDOOR, OPENDOOR, GRASS, TREE, EMPTYSPACE, MONSTER};
	switch (tileType = m.loadTile(xPos + x, yPos + y)){
	case CLOSEDDOOR:
		m.setTile(xPos + x, yPos + y, OPENDOOR);
		m.drawTile(xPos + x, yPos + y);
		screen.setTextColor(BLACK, WHITE);
		screen.drawTextBox("Door opened");
		break;
	case OPENDOOR:
		m.setTile(xPos + x, yPos + y, CLOSEDDOOR);
		m.drawTile(xPos + x, yPos + y);
		screen.setTextColor(BLACK, WHITE);
		screen.drawTextBox("Door closed");
		break;
	case ROCKFLOOR:
		screen.setTextColor(BLACK, WHITE);
		screen.drawTextBox("Ahead of you looms...absolutely nothing");
		break;
	case WALL:
		screen.setTextColor(BLACK, WHITE);
		screen.drawTextBox("A solid wall blocks your path");
		break;
	case GRASS:
		screen.setTextColor(BLACK, WHITE);
		screen.drawTextBox("You see a field of grass lies before you");
		break;
	case TREE:
		screen.setTextColor(BLACK, WHITE);
		screen.drawTextBox("A large, sturdy looking tree blocks your path");
		break;
	case EMPTYSPACE:
		screen.setTextColor(BLACK, WHITE);
		screen.drawTextBox("The void...");
		break;
	case MONSTER:
		screen.setTextColor(BLACK, WHITE);
		screen.drawTextBox("Prepare for battle!");
		this->addXP(10);
		screen.drawBattleScreen();
	}
}

// this function fills the lower-right corner of the screen
void Player::printCharacterInfo(){
	screen.setTextColor(BLACK, WHITE);
	screen.setPos(56, 13);
	cout << "Name: " << name;
	screen.setPos(56, 14);
	cout << "Class: " << clas;
	screen.setPos(56, 15);
	cout << "Gender: " << gender;
	screen.setPos(56, 16);
	cout << "Level: " << level;
	screen.setPos(56,17);
	cout << "XP: " << xp << "/" << toNextLevel;
	screen.setPos(56, 23);
	cout << "Press 'm' for menu";
}

void Player::setXP(int n){
	xp = n;
}

void Player::setNextLevel(int n){
	toNextLevel = n;
}

int Player::getXP(){
	return xp;
}

int Player::getNextLevel(){
	return toNextLevel;
}

// mapID repesents the current map the player is on
// should be called p.save(CURRENT MAP);
void Player::save(int mapID){
	ofstream outputFile;
	outputFile.open("Save.sav");

	// save all character related info
	outputFile << name << endl
			   << clas << endl
			   << gender << endl
			   << xp << endl
			   << level << endl
			   << xPos << endl
			   << yPos << endl
			   << mapID << endl;
}

// returns the mapID of the map the player saved on
// should be called like MAPID = p.load();
int Player::load(){
	int mapID;
	ifstream inputFile;
	inputFile.open("Save.sav");

	if (inputFile){
		inputFile >> name >> clas >> gender >> xp >> level >> xPos >> yPos;
		inputFile >> mapID;
		return mapID;
	} else{
		screen.drawTextBox("No save file found");
		return -1;
	}
}

void Player::addXP(int x){
	xp += x;
	if (xp >= toNextLevel)
		this->levelUp();
	this->printCharacterInfo();
}

void Player::levelUp(){
	toNextLevel *= 2;
	level ++;
}
#endif /* _ROGUEHEADER */