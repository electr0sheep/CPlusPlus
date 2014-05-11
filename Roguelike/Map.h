// we are going to need to use iostream
#pragma once
#ifndef _MAP_
#define _MAP_

#include <iostream>
#include <fstream>
#include "Screen.h"

using namespace std;

// not sure if these two defines do anything
//#define MAX_WIDTH 40
//#define MAX_HEIGHT 25

enum tile {ROCKFLOOR, WALL, CLOSEDDOOR, OPENDOOR, GRASS, TREE, EMPTYSPACE, MONSTER};

// sturct used for specific tiles
struct Tile{
	char character;
	color colorCode;
	bool passable;
};

// list of usable tiles
// first param is what will be displayed on the screen
// second param will be the color
// third param will indicate if it is passable or not
// comment at the end is how it will look in the array
Tile tileIndex[] = {
	{'.', WHITE, true},		//0
	{'|', DARKGRAY, false},	//1
	{'_', BROWN, false},	//2
	{'/', BROWN, true},		//3
	{'!', GREEN, true},		//4
	{'T', GREEN, false},	//5
	{' ', WHITE, false},	//6
	{'#', YELLOW, false}	//7
};

// the Map object
class Map{
	int height, width;
	int **mapArray;
	char current;
public:
	// constructor
	Map();
	// deconstructor
	~Map();
	void load(char);
	void save();
	void change(int);
	void drawMap();
	bool IsPassable(int, int);
	int loadTile(int, int);
	void setTile(int, int, int);
	void drawTile(int, int);
	int getCurrent();
};

Map::Map(){
	mapArray = new int*[2];
	for (int i = 0; i < 2; i++)
		mapArray[i] = new int[2];
}

Map::~Map(){
	for (int i = 0; i < height; i++)
		delete [] mapArray[i];
	delete [] mapArray;
}

// As more maps are added, using a switch in-code will probably not be best
// function loads a map into the map object. Right now, there is only one map
/*PROBABLY NEED TO CHECK TO MAKE SURE THE SPECIFIED MAP ACTUALLY EXISTS*/
void Map::load(char index){
	current = index;
	// first clear the possibly used memory
	for (int i = 0; i < height; i++)
		delete [] mapArray[i];
	delete [] mapArray;

	string fileName = "Map";
	fileName += index;
	fileName += ".map";
	
	ifstream inputFile;
	inputFile.open(fileName);

	inputFile >> width >> height;

	mapArray = new int*[height];
	for (int i = 0; i < height; i++)
		mapArray[i] = new int[width];

	for (int a = 0; a < height; a++){
		for (int b = 0; b < width; b++){
			inputFile >> mapArray[a][b];
		}
	}
}

// function used to save the map
void Map::save(){
	string fileName = "Map";
	fileName += current;
	fileName += ".map";

	ofstream outputFile;
	outputFile.open(fileName);
	outputFile << width << ' ' << height;
	for (int a = 0; a < height; a++){
		outputFile << endl;
		for (int b = 0; b < width; b++)
			outputFile << mapArray[a][b] << ' ';
	}
}

// function used to save the current map and load another one
void Map::change(int x){
	(*this).save();
	(*this).load(x);
}

// function to output the map to the screen
void Map::drawMap(){
	for (int y = 0; y < height; y++){
		screen.setPos(0, y);
		for (int x = 0; x < width; x++){
			// Draw the tile
			drawTile(x,y);
		}
	}
}

// function used to determine if the tile in question is passable
bool Map::IsPassable (int X, int Y){
	// Before we do anything, make darn sure that the coordinates are valid
	if (X < 0 || X >= width || Y < 0 || Y >= height)
		return false;

	// Store the value of the tile specified
	int tileValue = mapArray[Y][X];

	// Return true if it's passable
	return tileIndex[tileValue].passable;
}

// draws a specific tile. It is called when the entire map
// is drawn, and when a player moves
void Map::drawTile (int x, int y){
	screen.setPos(x,y);
	int type = mapArray[y][x];
	screen.setTextColor(BLACK, tileIndex[type].colorCode);
	cout << tileIndex[type].character;
}

// function used to return the value of a tile
// used when a player attempts to move
int Map::loadTile (int x, int y){
	return mapArray[y][x];
}

// changes what a tile is, i.e. when a player opens a door
void Map::setTile(int x, int y, int tile){
	mapArray[y][x] = tile;
}

// returns the map that is currently loaded
int Map::getCurrent(){
	return current;
}
/* **************LIST OF ASCII CHARACTERS****************
0)
1) ☺
2) ☻
3) ♥
4) ♦
5) ♣
6) ♠
11) ♂
12) ♀
14) ♫
15) ☼
16) ►
17) ◄
18) ↕
19) ‼
20) ¶
21) §
22) ▬
23) ↨
24) ↑
25) ↓
26) →
27) ←
28) ∟
29) ↔
30) ▲
31) ▼
33) !
34) "
35) #
36) $
37) %
38) &
39) '
40) (
41) )
42) *
43) +
44) ,
45) -
46) .
47) /
48) 0
49) 1
50) 2
51) 3
52) 4
53) 5
54) 6
55) 7
56) 8
57) 9
58) :
59) ;
60) <
61) =
62) >
63) ?
64) @
65) A
66) B
67) C
68) D
69) E
70) F
71) G
72) H
73) I
74) J
75) K
76) L
77) M
78) N
79) O
80) P
81) Q
82) R
83) S
84) T
85) U
86) V
87) W
88) X
89) Y
90) Z
91) [
92) \
93) ]
94) ^
95) _
96) `
97) a
98) b
99) c
100) d
101) e
102) f
103) g
104) h
105) i
106) j
107) k
108) l
109) m
110) n
111) o
112) p
113) q
114) r
115) s
116) t
117) u
118) v
119) w
120) x
121) y
122) z
123) {
124) |
125) }
126) ~
127) ⌂
128) Ç
129) ü
130) é
131) â
132) ä
133) à
134) å
135) ç
136) ê
137) ë
138) è
139) ï
140) î
141) ì
142) Ä
143) Å
144) É
145) æ
146) Æ
147) ô
148) ö
149) ò
150) û
151) ù
152) ÿ
153) Ö
154) Ü
155) ¢
156) £
157) ¥
158) ₧
159) ƒ
160) á
161) í
162) ó
163) ú
164) ñ
165) Ñ
166) ª
167) º
168) ¿
169) ⌐
170) ¬
171) ½
172) ¼
173) ¡
174) «
175) »
176) ░
177) ▒
178) ▓
179) │
180) ┤
181) ╡
182) ╢
183) ╖
184) ╕
185) ╣
186) ║
187) ╗
188) ╝
189) ╜
190) ╛
191) ┐
192) └
193) ┴
194) ┬
195) ├
196) ─
197) ┼
198) ╞
199) ╟
200) ╚
201) ╔
202) ╩
203) ╦
204) ╠
205) ═
206) ╬
207) ╧
208) ╨
209) ╤
210) ╥
211) ╙
212) ╘
213) ╒
214) ╓
215) ╫
216) ╪
217) ┘
218) ┌
219) █
220) ▄
221) ▌
222) ▐
223) ▀
224) α
225) ß
226) Γ
227) π
228) Σ
229) σ
230) µ
231) τ
232) Φ
233) Θ
234) Ω
235) δ
236) ∞
237) φ
238) ε
239) ∩
240) ≡
241) ±
242) ≥
243) ≤
244) ⌠
245) ⌡
246) ÷
247) ≈
248) °
249) ∙
250) ·
251) √
252) ⁿ
253) ²
254) ■
Press any key to continue . . .
*/
#endif /* _MAP_ */