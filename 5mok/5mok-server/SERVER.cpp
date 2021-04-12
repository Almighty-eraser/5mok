#include "SERVER.h"
#include "TCP_SERVER.h"

void SERVER::Run(void)
{
	g_serv_TCP->StartTCPserver(SERVER_PORT);

	std::thread t0{ &SERVER::EndServer, this };
	t0.detach();

	while (!g_End_Server)
	{
		SOCKET TempClnt = g_serv_TCP->AcceptClnt();
		char receive = g_serv_TCP->Receive(TempClnt);
		std::thread t{functions.at(receive), this, TempClnt};
		t.detach();
	}

	g_serv_TCP->EndTCPserver();
	return;
}

void SERVER::MakingRoom(SOCKET Clnt)
{
	g_socks.push_back(Clnt);
	
	char* room_name;
	room_name = g_serv_TCP->ReceiveStringRetAV(Clnt, BUFSIZE_OF_TITLE);
	g_room_names.push_back(room_name);

	//No need to end or delete room_name or socket
}

void SERVER::SendRoomList(SOCKET Clnt)
{	
	int size = g_socks.size();
	if (g_serv_TCP->SendInt(Clnt, size) == 0)
		return;

	for (int i = 0; i < g_room_names.size(); i++)
	{
		g_serv_TCP->SendString(Clnt, g_room_names[i], BUFSIZE_OF_TITLE);
	}

	g_serv_TCP->End(Clnt);
}

void SERVER::ChoosingRoom(SOCKET Clnt)
{
	while (1)
	{
		char number = g_serv_TCP->Receive(Clnt);
		number -= 1;
		if (g_socks[number] != NULL)
		{
			if (g_serv_TCP->SendChar(g_socks[number], 1) == 0)
				return;
			if (g_serv_TCP->SendChar(Clnt, 1) == 0)
				return;
			Play(g_socks[number], Clnt);
			break;
		}
		else
		{
			if (g_serv_TCP->SendChar(Clnt, -1) == 0)
				return;
			continue;
		}
	}

	//No need to end clnt sockets
}

void SERVER::DeletingRoom(SOCKET Clnt)
{
	char* string;
	bool success = false;
	string = g_serv_TCP->ReceiveStringRetAV(Clnt, BUFSIZE_OF_TITLE);
	success = Remove_room(string);
	delete[] string;
	
	if (success)
		g_serv_TCP->SendChar(Clnt, -1);
	else
		g_serv_TCP->SendChar(Clnt, 1);
	g_serv_TCP->End(Clnt);
	return;
}

void SERVER::Play(SOCKET black_clnt, SOCKET white_clnt)
{
	
	char pos[2];

	while (1)
	{
		//receive coordinates from black
		pos[0] = g_serv_TCP->Receive(black_clnt);
		if (pos[0] == WINNER_IS_WHITE)
		{
			Print_Time();
			std::cout << "White wins";
			break;
		}
		pos[1] = g_serv_TCP->Receive(black_clnt);
		Print_Time();
		std::cout << "black : " + pos[0] + ' ' + pos[1];

		g_serv_TCP->SendPosOfStone(white_clnt, pos[0], pos[1]);


		//receive coordinates from white
		pos[0] = g_serv_TCP->Receive(white_clnt);
		if (pos[0] == WINNER_IS_BLACK)
		{
			Print_Time();
			std::cout << "Black wins";
			break;
		}
		pos[1] = g_serv_TCP->Receive(white_clnt);
		Print_Time();
		std::cout << "white : " + pos[0] + ' ' + pos[1];


		g_serv_TCP->SendPosOfStone(black_clnt, pos[0], pos[1]);

	}

	g_serv_TCP->End(black_clnt);
	g_serv_TCP->End(white_clnt);
	return;
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
	g_End_Server = true;
	Clean_rooms();
	return;
}

