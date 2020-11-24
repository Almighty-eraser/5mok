#include "SERVER.h"

void SERVER::Run(void)
{
	SOCKET TempClnt;
	char receive;

	serv_TCP->StartTCPserver();

	while (1)
	{

		TempClnt = serv_TCP->WaitForClnt();

		receive = serv_TCP->Receive(TempClnt);
		serv_TCP->End(TempClnt);

		if (receive == _IMMA_MAKE_ROOM_)
		{
			std::thread t1(&SERVER::MakingRoom, this, TempClnt);
			t1.detach();
		}
		else if (receive == _IMMA_JOIN_ROOM_)
		{
			std::thread t2(&SERVER::ShowRooms, this, TempClnt);
			t2.detach();
		}
		else if (receive == _IMMA_CHOOSE_ROOM_)
		{
			std::thread t3(&SERVER::ChoosingRoomAndPlay, this, TempClnt);
			t3.detach();
		}
		else
			continue;
	}
	
	if (TempClnt != 0)
		serv_TCP->End(TempClnt);

}

void SERVER::MakingRoom(SOCKET Clnt)
{
	serv_makingroom->StartTCPserver(MAKING_ROOM_PORT);
	rooms.push_back(Clnt);
	serv_makingroom->SendChar(Clnt, 1);
	
	char* title;
	title = serv_makingroom->ReceiveStringRN(Clnt, BUFSIZE_OF_TITLE);
	titles.push_back(title);

	serv_makingroom->SendChar(Clnt, 1);
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
}

void SERVER::ChoosingRoomAndPlay(SOCKET Clnt)
{

}
