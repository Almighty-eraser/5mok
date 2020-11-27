#include "SERVER.h"

void SERVER::Run(void)
{
	SOCKET TempClnt;
	char receive;

	serv_TCP->StartTCPserver(SERVER_PORT);

	std::thread texit(&SERVER::EndServer, this);
	texit.detach();

	while (1)
	{

		TempClnt = serv_TCP->AcceptClnt();

		receive = serv_TCP->Receive(TempClnt);
		serv_TCP->End(TempClnt);

		if (receive == _IMMA_MAKE_ROOM_)
		{
			serv_TCP->SendChar(TempClnt, 1);
			std::thread t1(&SERVER::MakingRoom, this, TempClnt);
			t1.detach();
		}
		else if (receive == _IMMA_JOIN_ROOM_)
		{
			serv_TCP->SendChar(TempClnt, 1);
			std::thread t2(&SERVER::ShowRooms, this, TempClnt);
			t2.detach();
		}
		else if (receive == _IMMA_CHOOSE_ROOM_)
		{
			serv_TCP->SendChar(TempClnt, 1);
			std::thread t3(&SERVER::ChoosingRoom, this, TempClnt);
			t3.detach();
		}
		else if (receive == _IMMA_DELETE_ROOM_)
		{
			serv_TCP->SendChar(TempClnt, 1);
			std::thread t4(&SERVER::DeletingRoom, this, TempClnt);
			t4.detach();
		}
		else
		{
			serv_TCP->SendChar(TempClnt, -1);
			continue;
		}
	}
	
	if (TempClnt != 0)
		serv_TCP->End(TempClnt);

}

void SERVER::MakingRoom(SOCKET Clnt)
{
	rooms.push_back(Clnt);
	
	char* title;
	title = serv_TCP->ReceiveStringRN(Clnt, BUFSIZE_OF_TITLE);
	titles.push_back(title);

	serv_TCP->SendChar(Clnt, 1);
	delete[] title;
}

void SERVER::ShowRooms(SOCKET Clnt)
{
	if (RoomFlag == false)
	{
		serv_TCP->SendChar(Clnt, -1);
		return;
	}
	
	serv_TCP->SendChar(Clnt, 1);
	serv_TCP->SendChar(Clnt, rooms.size());

	for (int i = 0; i < rooms.size(); i++)
	{
		serv_TCP->SendString(Clnt, titles[i], BUFSIZE_OF_TITLE);
	}

	serv_TCP->End(Clnt);
}

void SERVER::ChoosingRoom(SOCKET Clnt)
{
	char number = serv_TCP->Receive(Clnt);
	number -= 1;
	if(rooms[number] != NULL)
	{
		serv_TCP->SendChar(rooms[number], 1);
		serv_TCP->SendChar(Clnt, 1);
		Play(rooms[number], Clnt);
	}
	else
		serv_TCP->SendChar(Clnt, -1);


}

void SERVER::DeletingRoom(SOCKET Clnt)
{
	char* string;
	string = serv_TCP->ReceiveStringRN(Clnt, BUFSIZE_OF_TITLE);
	int position;

	for (int i = 1; i < titles.size(); i++)
	{
		if (!strcmp(titles[i], string))
		{
			position = i;
			for (int j = i; j < titles.size() - 1; j++)
				titles[j] = titles[j + 1];
			titles.pop_back();
			break;
		}
	}
	
	for (int i = position; i < rooms.size() - 1; i++)
		rooms[i] = rooms[i + 1];
	rooms.pop_back();

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
			LeaveLog("White wins");
			break;
		}
		pos[1] = serv_TCP->Receive(black_clnt);
		LeaveLog("black : " + pos[0] + ' ' + pos[1]);

		serv_TCP->SendPosOfStone(white_clnt, pos[0] - 1, pos[1] - 1);


		//receive coordinates from white
		pos[0] = serv_TCP->Receive(white_clnt);
		if (pos[0] == WINNER_IS_BLACK)
		{
			LeaveLog("Black wins");
			break;
		}
		pos[1] = serv_TCP->Receive(white_clnt);
		LeaveLog("white : " + pos[0] + ' ' + pos[1]);


		serv_TCP->SendPosOfStone(black_clnt, pos[0] - 1, pos[1] - 1);

	}
}

void SERVER::EndServer(void)
{
	char command[10];
	while (1)
	{
		scanf_s("%s", command);
		if (!strcmp(command, "exit"))
			break;
	}
	exit(0);
}

