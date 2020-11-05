#include "BASE.h"
//бс бр

void UI::Mainmenu(void)
{
	cout
		<< "\t\t5MOK\t\t\twhite : бр black : бс board : +\n\n"
		<< "\t1. Singleplay(vs human)\n"
		<< "\t2. Singleplay(vs com)\n"
		<< "\t3. Multiplay\n"
		<< "\tInput : ";
}

void UI::MakeRoomOrNot(void)
{
	cout
		<< "\n\n\t1. Make a room"
		<< "\n\t2. Join a room"
		<< "\n\tInput : ";
}

void UI::SetColor(int forground, int background)
{
	int code = forground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), code);
}

void UI::PrintStone(int color)
{
	switch (color)
	{
	case BLACK:
		SetColor(white, black);
		cout << 'бр';
		break;
	case WHITE:
		SetColor(white, black);
		cout << 'бс';
		break;
	default:
		break;
	}
}

void UI::gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void UI::Clear(void)
{
	system("cls");
}

void UI::PrintBoard(char* board, int row, int height)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < row; j++)
		{
			SetColor(black, yellow);
			cout << '+';
		}
		cout << '\n';
	}
}