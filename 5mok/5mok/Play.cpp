#include "BASE.h"

void Play::START(void)
{
	int answer;
	main_UI->Mainmenu();
	cin >> answer;

	switch (answer)
	{
	case 1:
		SinglePptp();
		break;
	case 3:
		while (1)
		{
			main_UI->MakeRoomOrNot();
			cin >> answer;
			if (answer == 1)
			{
				
			}
			else if (answer == 2)
			{

			}
			else
				continue;
		}

		break;
	default:
		break;
	}
}

int Play::MakeBoard(void)
{
	int sizeofBoard[4] = {8, 9, 10, 11};
	int whichboard;
	while (1)
	{
		whichboard = main_UI->AskWhichBoard();
		if (whichboard < 1 || whichboard > 4)
			continue;

		board = new char[sizeofBoard[whichboard - 1] * sizeofBoard[whichboard - 1]];
		memset(board, 0, sizeofBoard[whichboard - 1] * sizeofBoard[whichboard - 1]);
		break;
	}

	return sizeofBoard[whichboard - 1];
}

void Play::RemoveBoard(void)
{
	delete[] board;
}

void Play::SinglePptp(void)
{
	main_UI->Clear();

	int Height = MakeBoard();

	while (1)
	{

	}
}

void Play::MultiP(void)
{

}