#pragma once
class UI
{
public:
	void SetColor(int forground, int background);
	void PrintStone(int color);
	void Mainmenu(void);
	void MakeRoomOrNot(void);
	void gotoxy(int x, int y);
	void Clear(void);
	int AskWhichBoard(void);
	int* AskCoordinatesRN(void);
	void PrintBoard(char* board, int length);
};

