#include "UI.h"
//бс бр

void UI::Mainmenu(void)
{
	cout
		<< "\t\t5MOK\t\t\tblack : ";
	SetColor(black, brown);
	cout << '0';
	SetColor(white, black);
	cout << "white: ";
	SetColor(white, brown);
	cout << '0';
	SetColor(white, black);
	cout
		<< "\n\n\t1. Singleplay(vs human)\n"
		<< "\t2. Singleplay(vs com)\n"
		<< "\t3. Multiplay\n"
		<< "\t4. Exit\n"
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

void UI::PrintStoneOfBoard(int stone)
{
	switch (stone)
	{
	case BLACK://black
		SetColor(black, brown);
		cout << '0';
		break;
	case WHITE://white
		SetColor(white, brown);
		cout << '0';
		break;
	case EMPTY:
		SetColor(black, brown);
		cout << '+';
		break;
	default:
		break;
	}
	SetColor(white, black);
}

void UI::ColorOneStone(int* board, int height, int x, int y)
{//First coordinates of Board : (5, 5)
	int PosOfStone = x + y * height;
	int stone = board[PosOfStone];
	gotoxy(x + 6, y + 6);
	switch (stone)
	{
	case BLACK://black
		SetColor(black, green);
		cout << '0';
		break;
	case WHITE://white
		SetColor(white, green);
		cout << '0';
		break;
	default:
		break;
	}
	SetColor(white, black);
}

void UI::ColorFiveStones(int* board, int height, 
	int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5)
{
	ColorOneStone(board, height, x1, y1);
	ColorOneStone(board, height, x2, y2);
	ColorOneStone(board, height, x3, y3);
	ColorOneStone(board, height, x4, y4);
	ColorOneStone(board, height, x5, y5);
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
	cout << "\n\n";
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
	scanf_s("%d", &answer);
	return answer;
}

int* UI::AskCoordinatesRN(void)//return new
{
	int* pos = new int[2];
	cout << "\n\nPlace : ";
	scanf_s("%d %d", pos, pos + 1);
	return pos;
}

void UI::PrintBoard(int* board, int length)
{
	gotoxy(5, 5);

	for (int i = 0; i <= length; i++)
		cout << i;
	cout << '\n';

	for (int i = 0; i < length; i++)
	{
		cout << "     ";
		if (i + 1 >= 10)
			cout << '\b';
		cout << i + 1;
		for (int j = 0; j < length; j++)
		{
			PrintStoneOfBoard(board[i * length + j]);
		}
		cout << i + 1;
		cout << '\n';
	}

	cout << "     ";
	for (int i = 0; i <= length; i++)
		cout << i;
	cout << '\n';
}

void UI::PressAnyKey(void)
{
	cout << "\n\nPress any key to proceed...";
	getchar();
	cout << "\n\n";
}