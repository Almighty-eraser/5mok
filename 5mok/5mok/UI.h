#pragma once
class UI
{
public:
	void SetColor(int forground, int background);
	void PrintStone(int color);
	void Mainmenu(void);
	void gotoxy(int x, int y);
	void Clear(void);
	void PrintBoard(char* board, int row, int height);
};

