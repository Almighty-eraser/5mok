#include "Play.h"

void Play::START(void)
{
	while (1)
	{
		int answer;
		main_UI->Mainmenu();
		scanf_s("%d", &answer);

		switch (answer)
		{
		case 1:
			SinglePptp();
			break;
		case 3:
			while (1)
			{
				main_UI->MakeRoomOrNot();
				scanf_s("%d", &answer);
				main_TCP->StartTCPclnt();
				if (answer == 1)
					MakeRoomOrNot(_IMMA_MAKE_ROOM_);
				else if (answer == 2)
					MakeRoomOrNot(_IMMA_JOIN_ROOM_);
				else
					continue;
			}

			break;
		case 4: 
			break;
		default:
			continue;
			break;
		}
		if (answer == 4)
			break;
	}

	main_UI->PressAnyKey();
}

int Play::MakeBoard(void)//return size of board
{
	int sizesofBoard[4] = {8, 9, 10, 11};
	int whichboard;
	while (1)
	{
		whichboard = main_UI->AskWhichBoard();
		if (whichboard < 1 || whichboard > 4)
			continue;

		int sizeofBoard = sizesofBoard[whichboard - 1] * sizesofBoard[whichboard - 1];
		board = new int[sizeofBoard];
		memset(board, 0, sizeofBoard * sizeof(int));
		break;
	}

	return sizesofBoard[whichboard - 1];
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
		main_UI->Clear();
		main_UI->PrintBoard(board, height);
		int* pos;
		
		//black first
		while (1) 
		{
			pos = new int[2];
			pos[0] = -1;
			while (*pos > height || *pos < 0 || *(pos + 1) > height || *(pos + 1) < 0)
			{
				delete[] pos;
				pos = main_UI->AskCoordinatesRN();
			}
			if (board[(pos[1] - 1) * height + pos[0] - 1] != EMPTY)
			{
				delete[] pos;
				continue;
			}
			board[(pos[1] - 1) * height + pos[0] - 1] = BLACK;
			break;
		}
		main_UI->Clear();
		main_UI->PrintBoard(board, height);
		delete[] pos;

		if (WhoseWinner(BLACK, height) == BLACK)
		{
			main_UI->ResultMessageForSingle(BLACK);
			break;
		}

		//white second
		while (1) 
		{
			pos = new int[2];
			pos[0] = -1;
			while (*pos > height || *pos < 0 || *(pos + 1) > height || *(pos + 1) < 0)
			{
				delete[] pos;
				pos = main_UI->AskCoordinatesRN();
			}
			if (board[(pos[1] - 1) * height + pos[0] - 1] != EMPTY)
			{
				delete[] pos;
				continue;
			}
			board[(pos[1] - 1) * height + pos[0] - 1] = WHITE;
			break;
		}
		main_UI->Clear();
		main_UI->PrintBoard(board, height);
		delete[] pos;

		if (WhoseWinner(WHITE, height) == WHITE)
		{
			main_UI->ResultMessageForSingle(WHITE);
			break;
		}
	}
}

void Play::MakeRoomOrNot(char decision)
{
	main_TCP->SendChar(decision); 

	while(main_TCP->Receive() != 1){}


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
							{
								main_UI->ColorFiveStones(board, height, j, i, j - 1, i, j - 2, i, j - 3, i, j - 4, i);
								return standard;
							}

			if (board[PresentPos + 1] == standard && PresentPos + 1 <= LastIndexOfLine)
				if (board[PresentPos] == standard)
					if (board[PresentPos - 1] == standard && PresentPos - 1 >= FirstIndexOfLine)
						if (board[PresentPos - 2] == standard && PresentPos - 2 >= FirstIndexOfLine)
							if (board[PresentPos - 3] == standard && PresentPos - 3 >= FirstIndexOfLine)
							{
								main_UI->ColorFiveStones(board, height, j, i, j - 1, i, j - 2, i, j - 3, i, j + 1, i);
								return standard;
							}

			if (board[PresentPos + 2] == standard && PresentPos + 2 <= LastIndexOfLine)
				if (board[PresentPos + 1] == standard && PresentPos + 1 <= LastIndexOfLine)
					if (board[PresentPos] == standard)
						if (board[PresentPos - 1] == standard && PresentPos - 1 >= FirstIndexOfLine)
							if (board[PresentPos - 2] == standard && PresentPos - 2 >= FirstIndexOfLine)
							{
								main_UI->ColorFiveStones(board, height, j, i, j - 1, i, j - 2, i, j + 1, i, j + 2, i);
								return standard;
							}

			if (board[PresentPos + 3] == standard && PresentPos + 3 <= LastIndexOfLine)
				if (board[PresentPos + 2] == standard && PresentPos + 2 <= LastIndexOfLine)
					if (board[PresentPos + 1] == standard && PresentPos + 1 <= LastIndexOfLine)
						if (board[PresentPos] == standard)
							if (board[PresentPos - 1] == standard && PresentPos - 1 >= FirstIndexOfLine)
							{
								main_UI->ColorFiveStones(board, height, j, i, j - 1, i, j + 1, i, j + 2, i, j + 3, i);
								return standard;
							}

			if (board[PresentPos + 4] == standard && PresentPos + 4 <= LastIndexOfLine)
				if (board[PresentPos + 3] == standard && PresentPos + 3 <= LastIndexOfLine)
					if (board[PresentPos + 2] == standard && PresentPos + 2 <= LastIndexOfLine)
						if (board[PresentPos + 1] == standard && PresentPos + 1 <= LastIndexOfLine)
							if (board[PresentPos] == standard)
							{
								main_UI->ColorFiveStones(board, height, j, i, j + 1, i, j + 2, i, j + 3, i, j + 4, i);
								return standard;
							}

			//check vertical lines
			int FirstIndexOfTheBoard = 0;
			int LastIndexOfTheBoard = height * height - 1;

			if (board[PresentPos] == standard)//check stones towards upward first
				if (board[PresentPos - height] == standard && PresentPos - height >= FirstIndexOfTheBoard)
					if (board[PresentPos - height * 2] == standard && PresentPos - height * 2 >= FirstIndexOfTheBoard)
						if (board[PresentPos - height * 3] == standard && PresentPos - height * 3 >= FirstIndexOfTheBoard)
							if (board[PresentPos - height * 4] == standard && PresentPos - height * 4 >= FirstIndexOfTheBoard)
							{
								main_UI->ColorFiveStones(board, height, j, i, j, i - 1, j, i - 2, j, i - 3, j, i - 4);
								return standard;
							}

			if (board[PresentPos + height] == standard && PresentPos + height <= LastIndexOfTheBoard)
				if (board[PresentPos] == standard)
					if (board[PresentPos - height] == standard && PresentPos - height >= FirstIndexOfTheBoard)
						if (board[PresentPos - height * 2] == standard && PresentPos - height * 2 >= FirstIndexOfTheBoard)
							if (board[PresentPos - height * 3] == standard && PresentPos - height * 3 >= FirstIndexOfTheBoard)
							{
								main_UI->ColorFiveStones(board, height, j, i, j, i - 1, j, i - 2, j, i - 3, j, i + 1);
								return standard;
							}

			if (board[PresentPos + height * 2] == standard && PresentPos + height * 2 <= LastIndexOfTheBoard)
				if (board[PresentPos + height] == standard && PresentPos + height <= LastIndexOfTheBoard)
					if (board[PresentPos] == standard)
						if (board[PresentPos - height] == standard && PresentPos - height >= FirstIndexOfTheBoard)
							if (board[PresentPos - height * 2] == standard && PresentPos - height * 2 >= FirstIndexOfTheBoard)
							{
								main_UI->ColorFiveStones(board, height, j, i, j, i - 1, j, i - 2, j, i + 2, j, i + 1);
								return standard;
							}

			if (board[PresentPos + height * 3] == standard && PresentPos + height * 3 <= LastIndexOfTheBoard)
				if (board[PresentPos + height * 2] == standard && PresentPos + height * 2 <= LastIndexOfTheBoard)
					if (board[PresentPos + height] == standard && PresentPos + height <= LastIndexOfTheBoard)
						if (board[PresentPos] == standard)
							if (board[PresentPos - height] == standard && PresentPos - height >= FirstIndexOfTheBoard)
							{
								main_UI->ColorFiveStones(board, height, j, i, j, i - 1, j, i + 3, j, i + 2, j, i + 1);
								return standard;
							}

			if (board[PresentPos + height * 4] == standard && PresentPos + height * 4 <= LastIndexOfTheBoard)
				if (board[PresentPos + height * 3] == standard && PresentPos + height * 3 <= LastIndexOfTheBoard)
					if (board[PresentPos + height * 2] == standard && PresentPos + height * 2 <= LastIndexOfTheBoard)
						if (board[PresentPos + height] == standard && PresentPos + height <= LastIndexOfTheBoard)
							if (board[PresentPos] == standard)
							{
								main_UI->ColorFiveStones(board, height, j, i, j, i + 4, j, i + 3, j, i + 2, j, i + 1);
								return standard;
							}

			//check diagonal lines
			int CheckPosGradient = height - 1;
			int CheckNegGradient = height + 1;

			//check postive gradient first
			if (board[PresentPos] == standard)
				if (board[PresentPos - CheckPosGradient] == standard && PresentPos - CheckPosGradient > FirstIndexOfLine - height)
					if (board[PresentPos - CheckPosGradient * 2] == standard && PresentPos - CheckPosGradient * 2 > FirstIndexOfLine - height * 2)
						if (board[PresentPos - CheckPosGradient * 3] == standard && PresentPos - CheckPosGradient * 3 > FirstIndexOfLine - height * 3)
							if (board[PresentPos - CheckPosGradient * 4] == standard && PresentPos - CheckPosGradient * 4 > FirstIndexOfLine - height * 4)
							{
								main_UI->ColorFiveStones(board, height, j, i, j + 1, i - 1, j + 2, i - 2, j + 3, i - 3, j + 4, i - 4);
								return standard;
							}

			if (board[PresentPos + CheckPosGradient] == standard && PresentPos + CheckPosGradient < LastIndexOfLine + height)
				if (board[PresentPos] == standard)
					if (board[PresentPos - CheckPosGradient] == standard && PresentPos - CheckPosGradient > FirstIndexOfLine - height)
						if (board[PresentPos - CheckPosGradient * 2] == standard && PresentPos - CheckPosGradient * 2 > FirstIndexOfLine - height * 2)
							if (board[PresentPos - CheckPosGradient * 3] == standard && PresentPos - CheckPosGradient * 3 > FirstIndexOfLine - height * 3)
							{
								main_UI->ColorFiveStones(board, height, j, i, j + 1, i - 1, j + 2, i - 2, j + 3, i - 3, j - 1, i + 1);
								return standard;
							}

			if (board[PresentPos + CheckPosGradient * 2] == standard && PresentPos + CheckPosGradient * 2 < LastIndexOfLine + height * 2)
				if (board[PresentPos + CheckPosGradient] == standard && PresentPos + CheckPosGradient < LastIndexOfLine + height)
					if (board[PresentPos] == standard)
						if (board[PresentPos - CheckPosGradient] == standard && PresentPos - CheckPosGradient > FirstIndexOfLine - height)
							if (board[PresentPos - CheckPosGradient * 2] == standard && PresentPos - CheckPosGradient * 2 > FirstIndexOfLine - height * 2)
							{
								main_UI->ColorFiveStones(board, height, j, i, j + 1, i - 1, j + 2, i - 2, j - 2, i + 2, j - 1, i + 1);
								return standard;
							}

			if (board[PresentPos + CheckPosGradient * 3] == standard && PresentPos + CheckPosGradient * 3 < LastIndexOfLine + height * 3)
				if (board[PresentPos + CheckPosGradient * 2] == standard && PresentPos + CheckPosGradient * 2 < LastIndexOfLine + height * 2)
					if (board[PresentPos + CheckPosGradient] == standard && PresentPos + CheckPosGradient < LastIndexOfLine + height)
						if (board[PresentPos] == standard)
							if (board[PresentPos - CheckPosGradient] == standard && PresentPos - CheckPosGradient > FirstIndexOfLine - height)
							{
								main_UI->ColorFiveStones(board, height, j, i, j + 1, i - 1, j - 3, i + 3, j - 2, i + 2, j - 1, i + 1);
								return standard;
							}

			if (board[PresentPos + CheckPosGradient * 4] == standard && PresentPos + CheckPosGradient * 4 < LastIndexOfLine + height * 4)
				if (board[PresentPos + CheckPosGradient * 3] == standard && PresentPos + CheckPosGradient * 3 < LastIndexOfLine + height * 3)
					if (board[PresentPos + CheckPosGradient * 2] == standard && PresentPos + CheckPosGradient * 2 < LastIndexOfLine + height * 2)
						if (board[PresentPos + CheckPosGradient] == standard && PresentPos + CheckPosGradient < LastIndexOfLine + height)
							if (board[PresentPos] == standard)
							{
								main_UI->ColorFiveStones(board, height, j, i, j - 4, i + 4, j - 3, i + 3, j - 2, i + 2, j - 1, i + 1);
								return standard;
							}

			//check negative gradient
			if (board[PresentPos] == standard)
				if (board[PresentPos - CheckNegGradient] == standard && PresentPos - CheckNegGradient > LastIndexOfLine - height * 2)
					if (board[PresentPos - CheckNegGradient * 2] == standard && PresentPos - CheckNegGradient * 2 > LastIndexOfLine - height * 3)
						if (board[PresentPos - CheckNegGradient * 3] == standard && PresentPos - CheckNegGradient * 3 > LastIndexOfLine - height * 4)
							if (board[PresentPos - CheckNegGradient * 4] == standard && PresentPos - CheckNegGradient * 4 > LastIndexOfLine - height * 5)
							{
								main_UI->ColorFiveStones(board, height, j, i, j - 1, i - 1, j - 2, i - 2, j - 3, i - 3, j - 4, i - 4);
								return standard;
							}

			if (board[PresentPos + CheckNegGradient] == standard && PresentPos + CheckNegGradient < FirstIndexOfLine + height * 2)
				if (board[PresentPos] == standard)
					if (board[PresentPos - CheckNegGradient] == standard && PresentPos - CheckNegGradient > LastIndexOfLine - height * 2)
						if (board[PresentPos - CheckNegGradient * 2] == standard && PresentPos - CheckNegGradient * 2 > LastIndexOfLine - height * 3)
							if (board[PresentPos - CheckNegGradient * 3] == standard && PresentPos - CheckNegGradient * 3 > LastIndexOfLine - height * 4)
							{
								main_UI->ColorFiveStones(board, height, j, i, j - 1, i - 1, j - 2, i - 2, j - 3, i - 3, j + 1, i + 1);
								return standard;
							}

			if (board[PresentPos + CheckNegGradient * 2] == standard && PresentPos + CheckNegGradient * 2 < FirstIndexOfLine + height * 3)
				if (board[PresentPos + CheckNegGradient] == standard && PresentPos + CheckNegGradient < FirstIndexOfLine + height * 2)
					if (board[PresentPos] == standard)
						if (board[PresentPos - CheckNegGradient] == standard && PresentPos - CheckNegGradient > LastIndexOfLine - height * 2)
							if (board[PresentPos - CheckNegGradient * 2] == standard && PresentPos - CheckNegGradient * 2 > LastIndexOfLine - height * 3)
							{
								main_UI->ColorFiveStones(board, height, j, i, j - 1, i - 1, j - 2, i - 2, j + 2, i + 2, j + 1, i + 1);
								return standard;
							}

			if (board[PresentPos + CheckNegGradient * 3] == standard && PresentPos + CheckNegGradient * 3 < FirstIndexOfLine + height * 4)
				if (board[PresentPos + CheckNegGradient * 2] == standard && PresentPos + CheckNegGradient * 2 < FirstIndexOfLine + height * 3)
					if (board[PresentPos + CheckNegGradient] == standard && PresentPos + CheckNegGradient < FirstIndexOfLine + height * 2)
						if (board[PresentPos] == standard)
							if (board[PresentPos - CheckNegGradient] == standard && PresentPos - CheckNegGradient > LastIndexOfLine - height * 2)
							{
								main_UI->ColorFiveStones(board, height, j, i, j - 1, i - 1, j + 3, i + 3, j + 2, i + 2, j + 1, i + 1);
								return standard;
							}

			if (board[PresentPos + CheckNegGradient * 4] == standard && PresentPos + CheckNegGradient * 4 < FirstIndexOfLine + height * 5)
				if (board[PresentPos + CheckNegGradient * 3] == standard && PresentPos + CheckNegGradient * 3 < FirstIndexOfLine + height * 4)
					if (board[PresentPos + CheckNegGradient * 2] == standard && PresentPos + CheckNegGradient * 2 < FirstIndexOfLine + height * 3)
						if (board[PresentPos + CheckNegGradient] == standard && PresentPos + CheckNegGradient < FirstIndexOfLine + height * 2)
							if (board[PresentPos] == standard)
							{
								main_UI->ColorFiveStones(board, height, j, i, j + 4, i + 4, j + 3, i + 3, j + 2, i + 2, j + 1, i + 1);
								return standard;
							}
		}
	return -1;
}