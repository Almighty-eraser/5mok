#pragma once
#include "BASE.h"
#include <iostream>
#include <Windows.h>
#include <vector>
using namespace std;

class UI
{
public:
	void PrintString(const char* string);
	void SetColor(int forground, int background);
	void PrintStoneOfBoard(int stone);
	void ColorOneStone(char* board, int height, int x, int y);
	void ColorFiveStones(char* board, int height, 
		int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5);
	void ResultMessageForMulti(int result);
	void ResultMessageForSingle(int result);
	void gotoxy(short x, short y);
	void Clear(void);
	void PrintBoard(char* board, int length);
	int AskWhichRoom(vector<char*> titles);
	void PressAnyKey(void);

	int Mainmenu(void);
	int MakeRoomOrNot(void);
	int AskWhichBoard(void);
	int* AskCoordinatesRetAV(void);
	char* AskTitleRetAV(void);
};

