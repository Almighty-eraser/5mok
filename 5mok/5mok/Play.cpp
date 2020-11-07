#include "BASE.h"

void Play::START(void)
{
	while (1)
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
			continue;
			break;
		}
	}
}

int Play::MakeBoard(void)//return size of board
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

	int height = MakeBoard();

	while (1)
	{
		main_UI->PrintBoard(board, height);
		int* pos;
		
		//black first
		pos = main_UI->AskACoordinationRN();
		board[(pos[1] - 1) * height + pos[0] - 1] == BLACK;
		main_UI->PrintBoard(board, height);
		delete[] pos;

		if(WhoseWinner(height) == )

		//white second
		pos = main_UI->AskACoordinationRN();
		board[(pos[1] - 1) * height + pos[0] - 1] == WHITE;
		delete[] pos;
	}
}

void Play::MultiP(void)
{

}

int Play::WhoseWinner(int stone, int height)
{
	int standard = stone;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < height; j++)
		{
			int PresentPos = i * height + j;
			int FirstMemOfLine = i * height;
			//check only x coordinate
			if (PresentPos - 4 >= FirstMemOfLine)
			{
				if (board[PresentPos] == standard)
					if (board[PresentPos - 1] == standard)//check left first
						if (board[PresentPos - 2] == standard)
							if (board[PresentPos - 3] == standard)									if (board[PresentPos - 4] == standard)
								return standard;
			}
			if (PresentPos - 3 >= FirstMemOfLine)
			{
				if (board[PresentPos] == standard)
					if (board[PresentPos - 1] == standard)
						if (board[PresentPos - 2] == standard)
							if (board[PresentPos - 3] == standard)
								if (board[PresentPos + 1] == standard)
									return standard;
			}
			if (PresentPos - 2 >= FirstMemOfLine)
			{
				if (board[PresentPos] == standard)
					if (board[PresentPos - 1] == standard)
						if (board[PresentPos - 2] == standard)
							if (board[PresentPos + 1] == standard)
								if (board[PresentPos + 2] == standard)
									return standard;
			}
			if (PresentPos - 1 >= FirstMemOfLine)
			{
				if (board[PresentPos] == standard)
					if (board[PresentPos - 1] == standard)
						if (board[PresentPos + 1] == standard)
							if (board[PresentPos + 2] == standard)
								if (board[PresentPos + 3] == standard)
									return standard;
			}
			if (PresentPos >= FirstMemOfLine)
			{
				if (board[PresentPos] == standard)
					if (board[PresentPos + 1] == standard)
						if (board[PresentPos + 2] == standard)
							if (board[PresentPos + 3] == standard)
								if (board[PresentPos + 4] == standard)
									return standard;
			}

			int LastMemOfLine = i * height + height - 1;

			//check only y coordinate

			if (PresentPos - height < 0)//if it's on the first line of the board
			{
				if (board[PresentPos] == standard)
					if (board[PresentPos + height] == standard)
						if (board[PresentPos + height * 2] == standard)
							if (board[PresentPos + height * 3] == standard)
								if (board[PresentPos + height * 4] == standard)
									return standard;
			}
			if (PresentPos - height * 2 < 0)
			{
			}
			if (PresentPos - height * 3 < 0)
			{

			}
			if (PresentPos - height * 4 < 0)
			{

			}

			//check diagonal lines





		}
}