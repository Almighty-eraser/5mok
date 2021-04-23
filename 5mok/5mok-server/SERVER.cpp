#include "SERVER.h"
#include "TCP_SERVER.h"


TCP_SERVER* g_serv_TCP;
std::vector<std::thread*> g_threads;
std::vector<SOCKET> g_room_owners;
std::vector<char*> g_room_names;
std::mutex g_rooms_mutex;

SERVER::SERVER(TCP_SERVER* tcp)
{
	g_serv_TCP = tcp;
	Print_Time();
	std::cout << "SERVER class started";
	functions.push_back(&SERVER::MakingRoom);
	functions.push_back(&SERVER::SendRoomList);
	functions.push_back(&SERVER::ChoosingRoom);
	functions.push_back(&SERVER::DeletingRoom);
};

SERVER::~SERVER() 
{
	Print_Time();
	std::cout << "SERVER class closed";
}

void SERVER::Run(void)
{
	g_serv_TCP->StartTCPserver(SERVER_PORT);

	std::thread t0( &SERVER::Commands, this );

	while (1)
	{
		SOCKET TempClnt = g_serv_TCP->AcceptClnt();
		g_serv_TCP->Add_Clnt(TempClnt);
		char receive = g_serv_TCP->Receive(TempClnt);
		std::thread *t1 = new std::thread( functions.at(receive), this, TempClnt );
		g_threads.push_back(t1);
	}
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
	int size = Get_Rooms_Size();
	if (g_serv_TCP->SendInt(Clnt, size) == 0)
		return;
	g_rooms_mutex.lock();
	for (int i = 0; i < size; i++)
	{
		g_serv_TCP->SendString(Clnt, g_room_names[i], BUFSIZE_OF_ROOM_NAME);
	}
	g_rooms_mutex.unlock();

	g_serv_TCP->Remove_Clnt(Clnt);
}

void SERVER::ChoosingRoom(SOCKET Clnt)
{
	char* room_name = g_serv_TCP->ReceiveStringRetAV(Clnt, BUFSIZE_OF_ROOM_NAME);
	int index = Find_Room(room_name);
	if (index >= 0)
	{
		if (g_serv_TCP->SendChar(g_room_owners[index], 1) == 0)
			return;
		if (g_serv_TCP->SendChar(Clnt, 1) == 0)
			return;
		Play(g_room_owners[index], Clnt);
	}
	else
	{
		if (g_serv_TCP->SendChar(Clnt, -1) == 0)
			return;
	}

	g_serv_TCP->Remove_Clnt(Clnt);

	delete[] room_name;

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
	g_serv_TCP->Remove_Clnt(Clnt);
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

	g_serv_TCP->Remove_Clnt(black_clnt);
	g_serv_TCP->Remove_Clnt(white_clnt);
	return;
}

void SERVER::Commands(void)
{
	char command[30] = {0};
	while (1)
	{
		scanf_s("%s", command, (unsigned int)sizeof(command));
		getchar();
		if (!strcmp(command, "exit"))
			break;
		else if (!strcmp(command, "clients"))
		{
			if (!g_serv_TCP->Print_Clnts_Info())
			{
				std::cout << "\n\nNo clients are active\n\n";
			}
		}
		else if (!strcmp(command, "rooms"))
		{
			if (!Print_Rooms())
			{
				std::cout << "\n\nNo rooms are active\n\n";
			}
		}
		else if (!strcmp(command, "terminate_clients"))
		{
			Clean_rooms();
			g_serv_TCP->Clean_Clnts();
			std::cout << "\n\nclients terminated\n\n";
		}
		else if (!strcmp(command, "terminate_client"))
		{
			int index = 0;
			scanf_s("%d", &index);
			getchar();
			if (g_serv_TCP->Remove_Clnt(g_serv_TCP->Get_Clnt_SOCKET(index - 1)) == false)
				std::cout << "\n\nFailed to terminate client\n\n";
			else
				std::cout << "\n\nterminated\n\n";
		}
	}
	for (int i = 0; i < g_threads.size(); i++)
	{
		if (g_threads[i]->joinable())
			g_threads[i]->join();
		delete g_threads[i];
	}
	Clean_rooms();
	g_serv_TCP->EndTCPserver();
	exit(0);
}

void SERVER::Add_room(SOCKET Clnt, char* room_name)
{
	g_rooms_mutex.lock();
	g_room_owners.push_back(Clnt);
	g_room_names.push_back(room_name);
	g_rooms_mutex.unlock();
}

bool SERVER::Remove_room(char* room_name)
{
	int index = -1;
	g_rooms_mutex.lock();
	if (g_room_owners.empty())
	{
		g_rooms_mutex.unlock();
		return false;
	}
	for (int i = 0; i < g_room_names.size(); i++)
		if (!strcmp(g_room_names[i], room_name))
		{
			index = i;
			break;
		}
	if (index != -1)
	{
		char* delete_room_name =  g_room_names[index];
		g_room_names.erase(g_room_names.begin() + index);
		delete delete_room_name;
		SOCKET sock = g_room_owners[index];
		g_room_owners.erase(g_room_owners.begin() + index);
		g_serv_TCP->Remove_Clnt(sock);
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
	if (g_room_owners.empty())
	{
		g_rooms_mutex.unlock();
		return;
	}
	while(g_room_names.empty() != 1)
	{
		char* room_name = g_room_names[0];
		g_room_names.erase(g_room_names.begin());
		delete room_name;
	}
	while (g_room_owners.empty() != 1)
		g_room_owners.erase(g_room_owners.begin());
	//Job to end sockets is what TCP_SERVER class does
	g_rooms_mutex.unlock();
	return;
}

int SERVER::Get_Rooms_Size(void)
{
	int size;
	g_rooms_mutex.lock();
	size = g_room_owners.size();
	g_rooms_mutex.unlock();
	return size;
}

bool SERVER::Print_Rooms(void)
{
	g_rooms_mutex.lock();
	if (g_room_owners.empty())
	{
		g_rooms_mutex.unlock();
		return false;
	}
	for (int i = 0; i < g_room_owners.size(); i++)
		std::cout << i + 1 << " SOCKET : " << g_room_owners[i] << " room : " << g_room_names[i]
			<< '\n';
	puts("\n");
	g_rooms_mutex.unlock();
	return true;
}

int SERVER::Find_Room(char* room_name)
{
	int index = -1;
	g_rooms_mutex.lock();
	for(int i = 0; i < g_room_names.size(); i++)
		if (!strcmp(g_room_names[i], room_name))
		{
			index = i;
			break;
		}
	g_rooms_mutex.unlock();
	return index;
}
