#include "UI.h"
#include <iostream>

using namespace std;

void UI::Clear(void)
{
	system("cls");
}

void UI::PrintBoard(char* board, int row, int height)
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < row; j++)
			cout << board[i * row + j];
}