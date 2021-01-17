#include "UI.h"
//бс бр

int UI::Mainmenu(void)
{
	int answer;
	cout
		<< "\t\t\n\n\n\t\t5MOK\t\t\tblack : ";
	SetColor(black, brown);
	cout << '0';
	SetColor(white, black);
	cout << "white: ";
	SetColor(white, brown);
	cout << '0';
	SetColor(white, black);
	cout
		<< "\n\n\t\t1. Singleplay(vs human)\n"
		<< "\t\t2. Multiplay\n"
		<< "\t\t3. Exit\n"
		<< "\t\tInput : ";
	scanf_s("%d", &answer);
	return answer;
}

int UI::MakeRoomOrNot(void)
{
	int answer;
	cout
		<< "\n\n\n\n\t\t1. Make a room"
		<< "\n\t\t2. Join a room"
		<< "\n\t\t3. Back to menu"
		<< "\n\t\tInput : ";
	scanf_s("%d", &answer);
	return answer;
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

void UI::ColorOneStone(char* board, int height, int x, int y)
{//First coordinates of Board : (5, 5)
	int PosOfStone = x + y * height;
	int stone = board[PosOfStone];
	gotoxy(x + FIRST_X + 1, y + FIRST_Y + 1);
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

void UI::ColorFiveStones(char* board, int height, 
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
	cout << "\n\n\n\t\tWhich board are you going to use?"
		<< "\n\n\t\t1. 8x8\n\t\t2. 9x9\n\t\t3. 10x10\n\t\t4. 11x11\n\n\t\tInput : ";
	scanf_s("%d", &answer);
	return answer;
}

int* UI::AskCoordinatesRetAV(void)//return allocated variable
{
	int* pos = new int[2];
	cout << "\n\nPlace : ";
	scanf_s("%d %d", pos, pos + 1);
	return pos;
}

char* UI::AskTitleRetAV(void)
{
	char* title = new char[BUFSIZE_OF_TITLE];
	cout << "\n\n\t\tInput title : ";
	scanf_s("%s", title, sizeof(title));

	return title;
}

void UI::PrintBoard(char* board, int length)
{
	gotoxy(FIRST_X, FIRST_Y);

	for (int i = 0; i <= length; i++)
		cout << i;
	cout << '\n';

	for (int i = 0; i < length; i++)
	{
		gotoxy(FIRST_X, FIRST_Y + 1 + i);
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

	gotoxy(FIRST_X, FIRST_Y + length + 1);
	for (int i = 0; i <= length; i++)
		cout << i;
	cout << '\n';
}

int UI::AskWhichRoom(vector<char*> titles)
{
	int answer;
	for (int i = 0; i < titles.size(); i++)
		cout << "\n\t\t" << i + 1 << ". " << titles[i];
	cout << "\t\t\t0. Back to Menu";
	cout << "\n\t\t  Input : ";
	scanf_s("%d", &answer);
	return answer;
}

void UI::PressAnyKey(void)
{
	cout << "\n\nPress any key to proceed...";
	getchar();
	cout << "\n\n";
}

void UI::PrintString(const char* string)
{
	for (int i = 0; string[i] != '\0'; i++)
		cout << string[i];
}