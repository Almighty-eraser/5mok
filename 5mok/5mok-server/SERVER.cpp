#include "SERVER.h"
#include "TCP_SERVER.h"


TCP_SERVER* g_serv_TCP;
std::vector<SOCKET> g_socks;
std::vector<char*> g_room_names;
bool g_End_Server;
std::mutex g_rooms_mutex;

SERVER::SERVER(TCP_SERVER* tcp)
{
	g_End_Server = false;
	g_serv_TCP = tcp;
	Print_Time();
	std::cout << "SERVER class started";
	functions.push_back(&SERVER::MakingRoom);
	functions.push_back(&SERVER::SendRoomList);
	functions.push_back(&SERVER::ChoosingRoom);
	functions.push_back(&SERVER::DeletingRoom);
};
SERVER::~SERVER() {

	g_serv_TCP->EndTCPserver();
	Print_Time();
	std::cout << "SERVER class closed";
}

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
	char* room_name;
	room_name = g_serv_TCP->ReceiveStringRetAV(Clnt, BUFSIZE_OF_ROOM_NAME);
	Add_room(Clnt, room_name);

	g_serv_TCP->SendChar(Clnt, 1);

	//No need to end or delete room_name or socket
}

void SERVER::SendRoomList(SOCKET Clnt)
{	
	int size = g_socks.size();
	if (g_serv_TCP->SendInt(Clnt, size) == 0)
		return;

	for (int i = 0; i < g_room_names.size(); i++)
	{
		g_serv_TCP->SendString(Clnt, g_room_names[i], BUFSIZE_OF_ROOM_NAME);
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
	string = g_serv_TCP->ReceiveStringRetAV(Clnt, BUFSIZE_OF_ROOM_NAME);
	success = Remove_room(string);
	delete[] string;
	
	if (success)
		g_serv_TCP->SendChar(Clnt, 1);
	else
		g_serv_TCP->SendChar(Clnt, -1);
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
		scanf_s("%s", command, (unsigned int)sizeof(command));
		if (!strcmp(command, "exit"))
			break;
	}
	g_End_Server = true;
	Clean_rooms();
	return;
}

void SERVER::Add_room(SOCKET Clnt, char* room_name)
{
	g_rooms_mutex.lock();
	g_socks.push_back(Clnt);
	g_room_names.push_back(room_name);
	g_rooms_mutex.unlock();
}

bool SERVER::Remove_room(char* room_name)
{
	g_rooms_mutex.lock();
	int index = -1;
	for (int i = 1; i < g_room_names.size(); i++)
		if (!strcmp(g_room_names[i], room_name))
		{
			index = i;
			break;
		}
	if (index != -1)
	{
		for (int j = index; j < g_room_names.size() - 1; j++)
			g_room_names[j] = g_room_names[j + 1];
		g_room_names.pop_back();

		for (int j = index; j < g_socks.size() - 1; j++)
			g_socks[j] = g_socks[j + 1];
		g_socks.pop_back();
	}
	g_rooms_mutex.unlock();
	if (index == -1)
		return false;
	else
		return true;
}

void SERVER::Clean_rooms(void)
{
	g_rooms_mutex.lock();
	for (int i = 0; i < g_room_names.size(); i++)
		if (g_room_names[i] != NULL)
			delete  g_room_names[i];
	for (int i = 0; i < g_socks.size(); i++)
		if (g_socks[i] != NULL)
			g_serv_TCP->End(g_socks[i]);
	g_rooms_mutex.unlock();
}