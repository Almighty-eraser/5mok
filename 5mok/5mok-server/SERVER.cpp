#include "SERVER.h"
#include "TCP_SERVER.h"

void SERVER::Run(void)
{
	serv_TCP->StartTCPserver(SERVER_PORT);

	std::thread t0{ &SERVER::EndServer, this };
	t0.detach();

	while (!End_Server)
	{
		SOCKET TempClnt = serv_TCP->AcceptClnt();
		char receive = serv_TCP->Receive(TempClnt);
		std::thread{functions.at(receive), this, TempClnt};
	}

	serv_TCP->EndTCPserver();
}

void SERVER::MakingRoom(SOCKET Clnt)
{
	rooms.push_back(Clnt);
	
	char* title;
	title = serv_TCP->ReceiveStringRetAV(Clnt, BUFSIZE_OF_TITLE);
	titles.push_back(title);

	if (!titles.empty())
		RoomFlag = true;

	serv_TCP->SendChar(Clnt, 1);

	//No need to end or delete title or socket
}

void SERVER::SendRoomList(SOCKET Clnt)
{
	if (RoomFlag == false)
	{
		serv_TCP->SendChar(Clnt, -1);
		return;
	}
	
	serv_TCP->SendChar(Clnt, 1);
	char size = rooms.size();
	serv_TCP->SendChar(Clnt, size);

	for (int i = 0; i < rooms.size(); i++)
	{
		serv_TCP->SendString(Clnt, titles[i], BUFSIZE_OF_TITLE);
	}

	serv_TCP->End(Clnt);
	delete &Clnt;
}

void SERVER::ChoosingRoom(SOCKET Clnt)
{
	while (1)
	{
		char number = serv_TCP->Receive(Clnt);
		number -= 1;
		if (rooms[number] != NULL)
		{
			serv_TCP->SendChar(rooms[number], 1);
			serv_TCP->SendChar(Clnt, 1);
			Play(rooms[number], Clnt);
			break;
		}
		else
		{
			serv_TCP->SendChar(Clnt, -1);
			continue;
		}
	}

	//No need to end clnt sockets
}

void SERVER::DeletingRoom(SOCKET Clnt)
{
	char* string;
	int position = 0;
	while (1)
	{
		string = serv_TCP->ReceiveStringRetAV(Clnt, BUFSIZE_OF_TITLE);

		for (int i = 1; i < titles.size(); i++)
		{
			if (!strcmp(titles[i], string))
			{
				position = i;
				for (int j = i; j < titles.size() - 1; j++)
					titles[j] = titles[j + 1];
				titles.pop_back();

				for (int i = position; i < rooms.size() - 1; i++)
					rooms[i] = rooms[i + 1];
				rooms.pop_back();
				break;
			}
			else
				position = -1;
		}

		if (position == -1)
		{
			serv_TCP->SendChar(Clnt, -1);
			delete[] string;
			continue;
		}
		else
		{
			serv_TCP->SendChar(Clnt, 1);
			break;
		}
			
	}

	if (titles.empty())
		RoomFlag = false;

	delete[] string;
}

void SERVER::Play(SOCKET black_clnt, SOCKET white_clnt)
{
	
	char pos[2];

	while (1)
	{
		//receive coordinates from black
		pos[0] = serv_TCP->Receive(black_clnt);
		if (pos[0] == WINNER_IS_WHITE)
		{
			Log("White wins");
			break;
		}
		pos[1] = serv_TCP->Receive(black_clnt);
		Log("black : " + pos[0] + ' ' + pos[1]);

		serv_TCP->SendPosOfStone(white_clnt, pos[0], pos[1]);


		//receive coordinates from white
		pos[0] = serv_TCP->Receive(white_clnt);
		if (pos[0] == WINNER_IS_BLACK)
		{
			Log("Black wins");
			break;
		}
		pos[1] = serv_TCP->Receive(white_clnt);
		Log("white : " + pos[0] + ' ' + pos[1]);


		serv_TCP->SendPosOfStone(black_clnt, pos[0], pos[1]);

	}

	serv_TCP->End(black_clnt);
	serv_TCP->End(white_clnt);
}

void SERVER::EndServer(void)
{
	char command[10] = {0};
	while (1)
	{
		scanf_s("%s", command, sizeof(command));
		if (!strcmp(command, "exit"))
			break;
	}
	End_Server = true;
	for (int i = 0; i < room_names.size(); i++)
		if (room_names[i] != NULL)
			delete room_names[i];
	for (int i = 0; i < socks.size(); i++)
		if (socks[i] != NULL)
		{
			serv_TCP->End(*socks[i]);
			delete socks[i];
		}
	serv_TCP->EndTCPserver();
	exit(0);
}

