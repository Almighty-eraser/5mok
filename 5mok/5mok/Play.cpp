#include "Play.h"

void Play::START(void)
{
	system("mode con cols=80 lines=30 | title 5mok");
	nickname = main_UI->AskNickNameRetAV();
	while (1)
	{
		int answer;
		answer = main_UI->Mainmenu();

		switch (answer)
		{
		case 1:
			SinglePptp();
			break;
		case 2:
			if (main_TCP->StartTCPclnt() != 1)
				break;
			while (1)
			{
				answer = main_UI->MakeRoomOrNot();
				if (answer == 3)
				{
					main_TCP->End();
					break;
				}
				else if (answer <= 0 || answer > 3)
					continue;
				else if (answer == 1)
				{
					main_TCP->SendChar(_IMMA_MAKE_ROOM_);
					MakingRoom();
				}
				else if (answer == 2)
				{
					main_TCP->SendChar(_IMMA_JOIN_ROOM_);
					JoiningRoom();
				}
				break;
			}
			break;
		case 3:
			break;
		default:
			continue;
			break;
		}
			if (answer == 3)
				break;
			main_UI->PressAnyKey();
			getchar();
			main_UI->Clear();
	}
	if (nickname != NULL)
		delete[] nickname;
	main_UI->PressAnyKey();
}
	 
int Play::MakeBoard(void)//return size of board
{
	if (board != NULL)
		RemoveBoard();
	int board_size_list[4] = {8, 9, 10, 11};
	int whichboard;
	while (1)
	{
		whichboard = main_UI->AskWhichBoard();
		if (whichboard < 1 || whichboard > 4)
			continue;

		int _board_size = board_size_list[whichboard - 1] * board_size_list[whichboard - 1];
		board = new char[_board_size];
		memset(board, 0, _board_size * sizeof(char));
		break;
	}

	return board_size_list[whichboard - 1];
}

void Play::MakeBoardForMulti(int _board_size)
{
	if (board != NULL)
		RemoveBoard();
	long __board_size = _board_size * _board_size;
	board = new char[__board_size];
	memset(board, 0, __board_size * sizeof(char));
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
		do
		{
			pos = main_UI->AskCoordinatesRetAV();
			if (*pos > height || *pos < 0 || *(pos + 1) > height || *(pos + 1) < 0)
			{
				delete[] pos;
				continue;
			}
			if (board[(pos[1] - 1) * height + pos[0] - 1] != EMPTY)
			{
				delete[] pos;
				continue;
			}
			break;
		} while (1);
		board[(pos[1] - 1) * height + pos[0] - 1] = BLACK;

		main_UI->Clear();
		main_UI->PrintBoard(board, height);
		delete[] pos;

		if (WhoseWinner(BLACK, height) == BLACK)
		{
			main_UI->ResultMessageForSingle(BLACK);
			break;
		}

		//white second
		do
		{
			pos = main_UI->AskCoordinatesRetAV();
			if (*pos > height || *pos < 0 || *(pos + 1) > height || *(pos + 1) < 0)
			{
				delete[] pos;
				continue;
			}
			if (board[(pos[1] - 1) * height + pos[0] - 1] != EMPTY)
			{
				delete[] pos;
				continue;
			}
			break;
		} while (1);
		board[(pos[1] - 1) * height + pos[0] - 1] = WHITE;
	
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

void Play::MakingRoom(void)
{
	char* room_name;
	room_name = main_UI->AskRoom_nameRetAV();
	char _board_size;
	_board_size = main_UI->AskWhichBoard();

	if (main_TCP->SendString(room_name, BUFSIZE_OF_ROOM_NAME) != BUFSIZE_OF_ROOM_NAME)
	{
		delete[] room_name;
		return;
	}

	if (main_TCP->SendChar(_board_size) != 1)
	{
		delete[] room_name;
		return;
	}

	if (main_TCP->SendString(nickname, BUFSIZE_OF_ROOM_NAME) != BUFSIZE_OF_NICKNAME)
	{
		delete[] room_name;
		return;
	}

	char receive;
	if (main_TCP->Receive(&receive) != 1)
	{
		delete[] room_name;
		return;
	}
	if (receive == 1)
		cout << "\nRoom created successfully\nWait until any player joins\n";
	else
	{
		cout << "\nFailed to create a room\n";
		delete[] room_name;
		return;
	}

	receive = 0;
	int state = 0;
	main_TCP->Change_IOMode(NON_BLOCKING);

	std::cout << "\n\nTo delete the room you created, press 1\n";

	while (1)
	{
		Sleep(500);
		state = main_TCP->Receive_Non_Blocking(&receive);

		if (state > 0 && receive == 1)
		{
			char message = 0;
			char* opponent_nickname = main_TCP->ReceiveStringRetAV(BUFSIZE_OF_NICKNAME);
			if (main_UI->AcceptTheOpponent(opponent_nickname))
			{
				message = 1;
				if (main_TCP->SendChar(message) != 1)
					break;
				main_TCP->Change_IOMode(BLOCKING);
				board_size = (int)_board_size + 7;
				MultiP(_IMMA_MAKE_ROOM_);
				delete[] opponent_nickname;
				break;
			}
			else
			{
				message = -1;
				if (main_TCP->SendChar(message) != 1)
					break;
			}
		}

		if (_kbhit())
		{
			char a = _getch();
			if (a == '1')
			{
				DeletingRoom(room_name);
				break;
			}
		}
	}

	main_TCP->End();
	delete[] room_name;
	return;
}

void Play::DeletingRoom(char* room_name)
{
	TCP* Delete = new TCP;

	Delete->StartTCPclnt();
	Delete->SendChar(_IMMA_DELETE_ROOM_);
	std::cout << "\nDeleting your Room...\n";

	Delete->SendString(room_name, BUFSIZE_OF_ROOM_NAME);
	char receive;
	if (Delete->Receive(&receive) < 0)
	{
		Delete->End();
		delete Delete;
		return;
	}
	if (receive == 1)
	{
		std::cout << "\nSuccessfully deleted the Room\n";
	}
	else
	{
		std::cout << "\nFailed to delete the Room\n";
	}
	Delete->End();
	delete Delete;
	return;
}

void Play::JoiningRoom(void)
{
	int* RoomCount = main_TCP->ReceiveIntRetAV();

	if (*RoomCount <= 0)
	{
		puts("\nThere's no room available\n");
		main_TCP->End();
		delete RoomCount;
		return;
	}


	while (room_names.empty() != 1)
	{
		char* room_name = room_names[0];
		room_names.erase(room_names.begin());
		delete[] room_name;
	}

	while (room_nicknames.empty() != 1)
	{
		char* nickname = room_nicknames[0];
		room_nicknames.erase(room_nicknames.begin());
		delete[] nickname;
	}

	while (room_board_sizes.empty() != 1)
	{
		room_board_sizes.erase(room_board_sizes.begin());
	}
	
	for (int i = 0; i < *RoomCount; i++)
	{
		char* room_name = main_TCP->ReceiveStringRetAV(BUFSIZE_OF_ROOM_NAME);
		room_names.push_back(room_name);
		char _board_size = 0;
		if (main_TCP->Receive(&_board_size) != 1)
		{
			delete RoomCount;
			return;
		}
		room_board_sizes.push_back(_board_size);
		char* nickname = main_TCP->ReceiveStringRetAV(BUFSIZE_OF_NICKNAME);
		room_nicknames.push_back(nickname);
	}
	delete RoomCount;
	main_TCP->End();

	while (1)
	{
		int ChosenRoomNum;
		ChosenRoomNum = main_UI->AskWhichRoom(room_names, room_board_sizes, room_nicknames);

		if (ChosenRoomNum == 0)
			break;

		if (main_TCP->StartTCPclnt() == -1)
			break;

		if (main_TCP->SendChar(_IMMA_CHOOSE_ROOM_) != sizeof(char))
			break;

		ChosenRoomNum--;

		if (main_TCP->SendString(room_names[ChosenRoomNum], BUFSIZE_OF_ROOM_NAME) != BUFSIZE_OF_ROOM_NAME)
			break;

		if (main_TCP->SendString(nickname, BUFSIZE_OF_NICKNAME) != BUFSIZE_OF_NICKNAME)
			break;

		char receive;
		std::cout << "\nWaiting for the host to accept the battle...\n\n";
		if (main_TCP->Receive(&receive) != 1)
			break;
		if (receive == 1)
		{
			board_size = room_board_sizes[ChosenRoomNum] + 7;
			std::cout << "\nJoining Room...\n\n";
			MultiP(_IMMA_JOIN_ROOM_);
		}
		else if(receive == -1)
		{
			std::cout << "\nThe room does not exist\n";
			continue;
		}
		else if (receive == -2)
		{
			std::cout << "\nGot rejected by room creater\n";
			break;
		}

		break;
	}
}

void Play::MultiP(int whichside)
{
	int height = (int)board_size;
	MakeBoardForMulti(height);
	char message = 0;
	if (whichside == _IMMA_MAKE_ROOM_)
	{
		while (1)//BLACK sends coodinates first
		{
			main_UI->Clear();
			main_UI->PrintBoard(board, height);
			int* pos;
			char* ch_pos;

			//black
			do
			{
				pos = main_UI->AskCoordinatesRetAV();
				if (*pos > height || *pos < 0 || *(pos + 1) > height || *(pos + 1) < 0)
				{
					delete[] pos;
					continue;
				}
				if (board[(pos[1] - 1) * height + pos[0] - 1] != EMPTY)
				{
					delete[] pos;
					continue;
				}
				break;
			} while (1);
			board[(pos[1] - 1) * height + pos[0] - 1] = BLACK;
		
			if (main_TCP->SendPosOfStone(pos[0] - 1, pos[1] - 1) != 2)
				break;

			main_UI->Clear();
			main_UI->PrintBoard(board, height);
			delete[] pos;

			if (WhoseWinner(BLACK, height) == BLACK)
			{
				main_UI->ResultMessageForMulti(WIN);
				message = WINNER_IS_BLACK;
				if (main_TCP->SendChar(message) != 1)
					break;
				break;
			}
			else
			{
				message = 1;
				if (main_TCP->SendChar(message) != 1)
					break;
			}

			//white
			ch_pos = new char[2];
			if (main_TCP->Receive(&ch_pos[0]) != 1)
				break;
			if (main_TCP->Receive(&ch_pos[1]) != 1)
				break;
			board[(int)ch_pos[1] * height + (int)ch_pos[0]] = WHITE;
			
			main_UI->Clear();
			main_UI->PrintBoard(board, height);
			delete[] ch_pos;

			if (WhoseWinner(WHITE, height) == WHITE)
			{
				main_UI->ResultMessageForMulti(LOST);
				break;
			}
		}
	}
	else
	{
		while (1)//WHITE receives coordinates first
		{
			main_UI->Clear();
			main_UI->PrintBoard(board, height);
			char* ch_pos;
			int* pos;

			//black
			ch_pos = new char[2];
			if (main_TCP->Receive(&ch_pos[0]) != 1)
				break;
			if (main_TCP->Receive(&ch_pos[1]) != 1)
				break;
			board[(int)ch_pos[1] * height + (int)ch_pos[0]] = BLACK;
			main_UI->Clear();
			main_UI->PrintBoard(board, height);
			delete[] ch_pos;

			if (WhoseWinner(BLACK, height) == BLACK)
			{
				main_UI->ResultMessageForMulti(LOST);
				break;
			}

			//white
			do
			{
				pos = main_UI->AskCoordinatesRetAV();
				if (*pos > height || *pos < 0 || *(pos + 1) > height || *(pos + 1) < 0)
				{
					delete[] pos;
					continue;
				}
				if (board[(pos[1] - 1) * height + pos[0] - 1] != EMPTY)
				{
					delete[] pos;
					continue;
				}
				break;
			} while (1);
			board[(pos[1] - 1) * height + pos[0] - 1] = WHITE;

			if(main_TCP->SendPosOfStone(pos[0] - 1, pos[1] - 1) != 2)
				break;

			main_UI->Clear();
			main_UI->PrintBoard(board, height);
			delete[] pos;

			if (WhoseWinner(WHITE, height) == WHITE)
			{
				main_UI->ResultMessageForMulti(WIN);
				message = WINNER_IS_WHITE;
				if (main_TCP->SendChar(message) != 1)
					break;
				break;
			}
			else
			{
				message = 1;
				if (main_TCP->SendChar(message) != 1)
					break;
			}
		}
	}
	main_TCP->End();
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