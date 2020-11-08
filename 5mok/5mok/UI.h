#pragma once
#include "BASE.h"
class UI
{
public:
	void SetColor(int forground, int background);
	void PrintStone(int color);
	void ColorOneStone(char* board, int height, int x, int y);
	void ResultMessage(int result);
	void Mainmenu(void);
	void MakeRoomOrNot(void);
	void gotoxy(short x, short y);
	void Clear(void);
	int AskWhichBoard(void);
	int* AskCoordinatesRN(void);
	void PrintBoard(char* board, int length);
};

