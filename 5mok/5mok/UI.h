#pragma once
#include "BASE.h"
class UI
{
public:
	void SetColor(int forground, int background);
	void PrintStoneOfBoard(int stone);
	void ColorOneStone(int* board, int height, int x, int y);
	void ColorFiveStones(int* board, int height, 
		int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5);
	void ResultMessageForMulti(int result);
	void ResultMessageForSingle(int result);
	void Mainmenu(void);
	void MakeRoomOrNot(void);
	void gotoxy(short x, short y);
	void Clear(void);
	int AskWhichBoard(void);
	int* AskCoordinatesRN(void);
	void PrintBoard(int* board, int length);
	void PressAnyKey(void);
};

