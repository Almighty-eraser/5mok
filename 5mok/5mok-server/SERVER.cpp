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

		TempClnt = serv_TCP->WaitForClnt();

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
		else
		{
			serv_TCP->SendChar(TempClnt, -1);
			continue;
		}
			

		
	}
	
	if (TempClnt != 0)
		serv_TCP->End(TempClnt);

}

void SERVER::MakingRoom(void)
{
	SOCKET Clnt = serv_makingroom.WaitForClnt();
	rooms.push_back(Clnt);
	
	char* title;
	title = serv_makingroom.ReceiveStringRN(Clnt, BUFSIZE_OF_TITLE);
	titles.push_back(title);

	serv_makingroom.SendChar(Clnt, 1);

	serv_makingroom.End(Clnt);
}

void SERVER::ShowRooms(void)
{
	SOCKET Clnt = serv_showrooms.WaitForClnt();
	if (RoomFlag == false)
	{
		serv_showrooms.SendChar(Clnt, -1);
		return;
	}
	
	serv_showrooms.SendChar(Clnt, 1);
	serv_showrooms.SendChar(Clnt, rooms.size());

	for (int i = 0; i < rooms.size(); i++)
	{
		serv_showrooms.SendString(Clnt, titles[i], BUFSIZE_OF_TITLE);
	}

	serv_showrooms.End(Clnt);
}

void SERVER::ChoosingRoom(void)
{
	
	SOCKET Clnt = serv_choosingroom.WaitForClnt();

	char number = serv_choosingroom.Receive(Clnt);
	if(rooms[number] != NULL)
	{
		serv_makingroom.SendChar(rooms[number], 1);
		Play(rooms[number], Clnt);
	}
	else
		serv_choosingroom.SendChar(Clnt, -1);

	serv_choosingroom.End(Clnt);
}

void SERVER::Play(SOCKET black_clnt, SOCKET white_clnt)
{
	char pos[2];

	while (1)
	{
		//receive coordinates from black
		pos[0] = serv_makingroom.Receive(black_clnt);
		if (pos[0] == WINNER_IS_WHITE)
			break;
		pos[1] = serv_makingroom.Receive(black_clnt);

		serv_choosingroom.SendPosOfStone(white_clnt, pos[0] - 1, pos[1] - 1);


		//receive coordinates from white
		pos[0] = serv_makingroom.Receive(white_clnt);
		if (pos[0] == WINNER_IS_BLACK)
			break;
		pos[1] = serv_makingroom.Receive(white_clnt);

		serv_choosingroom.SendPosOfStone(black_clnt, pos[0] - 1, pos[1] - 1);

	}
}

void SERVER::EndServer(void)
{
	if (kbhit())
		if (int input = _getch() == 27)
		{
			exit(0);
		}
		else
			return;
	else
		return;
}
