#include "Play.h"

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
		pos = main_UI->AskCoordinatesRN();
		board[(pos[1] - 1) * height + pos[0] - 1] = BLACK;
		main_UI->PrintBoard(board, height);
		delete[] pos;

		if (WhoseWinner(BLACK, height) == BLACK)
		{
			
		}

		//white second
		pos = main_UI->AskCoordinatesRN();
		board[(pos[1] - 1) * height + pos[0] - 1] = WHITE;
		delete[] pos;

		if (WhoseWinner(WHITE, height) == WHITE)
		{

		}
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
			//check horizontal lines
			int PresentPos = i * height + j;
			int FirstIndexOfLine = i * height;
			int LastIndexOfLine = i * height + height - 1;

			if (board[PresentPos] == standard)//check stones towards left first
				if (board[PresentPos - 1] == standard && PresentPos - 1 >= FirstIndexOfLine)
					if (board[PresentPos - 2] == standard && PresentPos - 2 >= FirstIndexOfLine)
						if (board[PresentPos - 3] == standard && PresentPos - 3 >= FirstIndexOfLine)
							if (board[PresentPos - 4] == standard && PresentPos - 4 >= FirstIndexOfLine)
								return standard;

			if (board[PresentPos + 1] == standard && PresentPos + 1 <= LastIndexOfLine)
				if (board[PresentPos] == standard)
					if (board[PresentPos - 1] == standard && PresentPos - 1 >= FirstIndexOfLine)
						if (board[PresentPos - 2] == standard && PresentPos - 2 >= FirstIndexOfLine)
							if (board[PresentPos - 3] == standard && PresentPos - 3 >= FirstIndexOfLine)
								return standard;

			if (board[PresentPos + 2] == standard && PresentPos + 2 <= LastIndexOfLine)
				if (board[PresentPos + 1] == standard && PresentPos + 1 <= LastIndexOfLine)
					if (board[PresentPos] == standard)
						if (board[PresentPos - 1] == standard && PresentPos - 1 >= FirstIndexOfLine)
							if (board[PresentPos - 2] == standard && PresentPos - 2 >= FirstIndexOfLine)
								return standard;

			if (board[PresentPos + 3] == standard && PresentPos + 3 <= LastIndexOfLine)
				if (board[PresentPos + 2] == standard && PresentPos + 2 <= LastIndexOfLine)
					if (board[PresentPos + 1] == standard && PresentPos + 1 <= LastIndexOfLine)
						if (board[PresentPos] == standard)
							if (board[PresentPos - 1] == standard && PresentPos - 1 >= FirstIndexOfLine)
								return standard;

			if (board[PresentPos + 4] == standard && PresentPos + 4 <= LastIndexOfLine)
				if (board[PresentPos + 3] == standard && PresentPos + 3 <= LastIndexOfLine)
					if (board[PresentPos + 2] == standard && PresentPos + 2 <= LastIndexOfLine)
						if (board[PresentPos + 1] == standard && PresentPos + 1 <= LastIndexOfLine)
							if (board[PresentPos] == standard)
								return standard;

			//check vertical lines
			int FirstIndexOfTheBoard = 0;
			int LastIndexOfTheBoard = height * height - 1;

			if(board[PresentPos] == standard)//check stones towards upward first
				if(board[PresentPos - height] == standard && PresentPos - height <= FirstIndexOfTheBoard)
					if(board[PresentPos - height * 2] == standard && PresentPos - height * 2 )
			
			//check diagonal lines





		}
	return -1;
}