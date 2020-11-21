#include "SERVER.h"

void SERVER::Run(void)
{
	SOCKET BlackClnt, WhiteClnt, TempClnt;
	char receive;
	bool RoomFlag(false);

	serv_TCP->StartTCPserver(MAIN_SERVER_PORT);

	while (1)
	{
		TempClnt = 0;
		BlackClnt = 0;
		WhiteClnt = 0;

		TempClnt = serv_TCP->WaitForClnt();

		receive = serv_TCP->Receive(TempClnt);

		if (receive == _IMMA_JOIN_ROOM_)
		{
			WhiteClnt = TempClnt;

			if (RoomFlag == false)
			{
				serv_TCP->SendChar(WhiteClnt, -1);
				continue;
			}
			else if (RoomFlag == true)
			{
				
			}
		}
		else if (receive == _IMMA_MAKE_ROOM_)
		{
			BlackClnt = TempClnt;

			RoomFlag = true;
		}
		else
			continue;
	}

}

void SERVER::ShowRooms(SOCKET Clnt)
{

}
