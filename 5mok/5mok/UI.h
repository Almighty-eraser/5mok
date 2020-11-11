#pragma once
#include "BASE.h"
class UI
{
public:
	void SetColor(int forground, int background);
	void PrintStoneOfBoard(int stone);
	void ColorOneStone(int* board, int height, int x, int y);
	void ResultMessageForMulti(int result);
	void ResultMessageForSingle(int result);
	void Mainmenu(void);
	void MakeRoomOrNot(void);
	void gotoxy(short x, short y);
	void Clear(void);
	int AskWhichBoard(void);
	int* AskCoordinatesRN(void);
	void PrintBoard(int* board, int length);
};

