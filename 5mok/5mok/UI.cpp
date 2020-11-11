#include "UI.h"
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
		SetColor(black, yellow);
		cout << '+';
		break;
	}
}

void UI::ColorOneStone(char* board, int height, int x, int y)
{//First coordinates of Board : (5, 5)
	int PosOfStone = x + y * height;
	char stone = board[PosOfStone];
	SetColor(white, green);
	gotoxy(x + 5, y + 5);
	cout << stone;
}

void UI::ResultMessageForMulti(int result)
{
	gotoxy(0, 20);
	if (result == 0)
	{
		cout <<
			"бс     бс       бс     бсбсбс      бс           бс\n"
		<<	"бс    бс      бс        бс        бс бс       бс\n"
		<<	"бс   бс     бс        бс        бс    бс    бс\n"
		<<	"бс  бс    бс        бс         бс      бс  бс\n"
		<<	"бс бс  бс         бс         бс         бсбс\n"
		<<	"бс  бс       бс бс бс      бс            бс\n";
	}
	else
	{
		cout <<
			"     бс                  бс            бсбсбсбсбс   бсбсбсбсбсбсбсбс\n"
	<<      "    бс               бс    бс      бс                      бс\n"
	<<      "   бс              бс      бс     бс                     бс\n"
	<<      "  бс              бс       бс      бсбсбсбсбс          бс\n"
	<<      " бс               бс     бс                 бс      бс\n"
	<<      "бс                бс   бс                бс       бс\n"
	<<      "бсбсбсбсбсбс        бсбс       бсбсбсбсбс       бс";
	}
}

void UI::ResultMessageForSingle(int result)
{
	gotoxy(0, 20);
	if (result == BLACK)
		cout << "Black win";
	else 
		cout << "White win";
}

void UI::gotoxy(short x, short y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void UI::Clear(void)
{
	system("cls");
}

int UI::AskWhichBoard(void)
{
	int answer;
	cout << "Which board are you going to use?"
		<< "\n\n1. 8x8\n2. 9x9\n3. 10x10\n4. 11x11\n\nInput : ";
	cin >> answer;
	return answer;
}

int* UI::AskCoordinatesRN(void)//return new
{
	int* pos = new int[2];
	cout << "\n\nPlace : ";
	cin >> pos[0] >> pos[1];
	return pos;
}

void UI::PrintBoard(char* board, int length)
{
	Clear();
	gotoxy(5, 5);

	for (int i = 0; i <= length; i++)
		cout << i;
	cout << '\n';

	for (int i = 0; i < length; i++)
	{
		cout << i + 1;
		for (int j = 0; j < length; j++)
		{
			PrintStone(board[i * length + length]);
		}
		cout << '\n';
	}
}