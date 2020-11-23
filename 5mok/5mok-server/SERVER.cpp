#include "SERVER.h"

void SERVER::Run(void)
{
	SOCKET TempClnt;
	char receive;
	bool RoomFlag(false);

	serv_TCP->StartTCPserver();

	while (1)
	{

		TempClnt = serv_TCP->WaitForClnt();

		receive = serv_TCP->Receive(TempClnt);

		if (receive == _IMMA_MAKE_ROOM_)
		{
			std::thread t1(&SERVER::MakingRoom, this, TempClnt);
			t1.detach();
			break;
		}
		else if (receive == _IMMA_JOIN_ROOM_)
		{
			std::thread t2(&SERVER::ShowRooms, this, TempClnt);
			t2.detach();
			break;
		}
		else if (receive == _IMMA_CHOOSE_ROOM_)
		{
			std::thread t3(&SERVER::ChoosingRoomAndPlay, this, TempClnt);
			t3.detach();
			break;
		}
		else
			continue;
	}
	if (TempClnt != 0)
		serv_TCP->End(TempClnt);

}

void SERVER::MakingRoom(SOCKET Clnt)
{
	
}

void SERVER::ShowRooms(SOCKET Clnt)
{

}

void SERVER::ChoosingRoomAndPlay(SOCKET Clnt)
{

}
