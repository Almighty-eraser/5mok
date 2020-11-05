#include "UI.h"
#include "BASE.h"
#include <iostream>
#include <Windows.h>
//�� ��
using namespace std;

void UI::Mainmenu(void)
{
	cout
		<< "\t\t5MOK\n\n"
		<< "\t1. Singleplay(vs human)\n"
		<< "\t2. Singleplay(vs com)\n"
		<< "\tMultiplay\n"
		<< "Input : ";
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
		cout << '��';
		break;
	case WHITE:
		SetColor(white, black);
		cout << '��';
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