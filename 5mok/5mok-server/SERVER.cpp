#include "SERVER.h"
#include "TCP_SERVER.h"


TCP_SERVER* g_serv_TCP;
std::vector<std::thread*> g_threads;
std::vector<SOCKET> g_room_owners;
std::vector<char*> g_room_nicknames;
std::vector<char*> g_room_names;
std::vector<int> g_room_board_sizes;
std::mutex g_rooms_mutex;

SERVER::SERVER(TCP_SERVER* tcp)
{
	g_serv_TCP = tcp;
	Print_Time();
	std::cout << "SERVER class started\t\t(Type \"help\" to see commands)";
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
		char receive;
		g_serv_TCP->Receive(TempClnt, &receive);
		std::thread *t1 = new std::thread( functions.at(receive), this, TempClnt );
		g_threads.push_back(t1);
	}
	return;
}

void SERVER::MakingRoom(SOCKET Clnt)
{
	char* room_name = g_serv_TCP->ReceiveStringRetAV(Clnt, BUFSIZE_OF_ROOM_NAME);
	int* board_size = g_serv_TCP->ReceiveIntRetAV(Clnt);
	char* nickname = g_serv_TCP->ReceiveStringRetAV(Clnt, BUFSIZE_OF_NICKNAME);
	Add_room(Clnt, room_name, *board_size, nickname);
	delete board_size;

	if (g_serv_TCP->SendChar(Clnt, 1) != 1)
		return;

	//No need to end or delete room_name or socket
}

void SERVER::SendRoomList(SOCKET Clnt)
{	
	int size = Get_Rooms_Size();
	if (g_serv_TCP->SendInt(Clnt, size) != sizeof(int))
		return;
	g_rooms_mutex.lock();
	for (int i = 0; i < size; i++)
	{
		if(g_serv_TCP->SendString(Clnt, g_room_names[i], BUFSIZE_OF_ROOM_NAME) != BUFSIZE_OF_ROOM_NAME)
		{
			g_rooms_mutex.unlock();
			return;
		}
		if (g_serv_TCP->SendInt(Clnt, g_room_board_sizes[i]) != sizeof(int))
		{
			g_rooms_mutex.unlock();
			return;
		}
		if (g_serv_TCP->SendString(Clnt, g_room_nicknames[i], BUFSIZE_OF_NICKNAME) != BUFSIZE_OF_NICKNAME)
		{
			g_rooms_mutex.unlock();
			return;
		}
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
		if (g_serv_TCP->SendChar(g_room_owners[index], 1) != 1)
		{
			g_serv_TCP->Remove_Clnt(Clnt);
			delete[] room_name;
			return;
		}
		char* clnt_nickname = g_serv_TCP->ReceiveStringRetAV(Clnt, BUFSIZE_OF_NICKNAME);
		if (g_serv_TCP->SendString(g_room_owners[index], clnt_nickname, BUFSIZE_OF_NICKNAME) != BUFSIZE_OF_NICKNAME)
		{
			g_serv_TCP->Remove_Clnt(Clnt);
			delete[] room_name;
			return;
		}
		char receive = -1;
		if(g_serv_TCP->Receive(g_room_owners[index], &receive) != 1)
		{
			g_serv_TCP->Remove_Clnt(Clnt);
			delete[] room_name;
			return;
		}
		if (receive == 1)
		{
			if (g_serv_TCP->SendChar(Clnt, 1) != 1)
			{
				g_serv_TCP->Remove_Clnt(Clnt);
				delete[] room_name;
				return;
			}
			Play(g_room_owners[index], Clnt);
			Remove_room(room_name);
		}
		else
		{
			if (g_serv_TCP->SendChar(Clnt, -2) != 1)
			{
				g_serv_TCP->Remove_Clnt(Clnt);
				delete[] room_name;
				return;
			}
			g_serv_TCP->Remove_Clnt(Clnt);
		}
	}
	else
	{
		if (g_serv_TCP->SendChar(Clnt, -1) == 0)
			return;
		g_serv_TCP->Remove_Clnt(Clnt);
	}

	delete[] room_name;
	//No need to remove room_owner sock
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
	char message = 0;

	while (1)
	{
		//receive coordinates from black
		if (g_serv_TCP->Receive(black_clnt, &pos[0]) != 1)
			break;
		if (g_serv_TCP->Receive(black_clnt, &pos[1]) != 1)
			break;
		Print_Time();
		std::cout << "black : " + pos[0] + ' ' + pos[1];

		if (g_serv_TCP->Receive(black_clnt, &message) != 1)
			break;

		if (g_serv_TCP->SendPosOfStone(white_clnt, pos[0], pos[1]) != 2)
			break;

		if (message == WINNER_IS_BLACK)
		{
			Print_Time();
			std::cout << "Black wins";
			break;
		}


		//receive coordinates from white
		if (g_serv_TCP->Receive(white_clnt, &pos[0]) != 1)
			break;
		if (g_serv_TCP->Receive(white_clnt, &pos[1]) != 1)
			break;
		Print_Time();
		std::cout << "white : " + pos[0] + ' ' + pos[1];

		if (g_serv_TCP->Receive(white_clnt, &message) != 1)
			break;
		
		if(g_serv_TCP->SendPosOfStone(black_clnt, pos[0], pos[1]) != 2)
			break;

		if (message == WINNER_IS_WHITE)
		{
			Print_Time();
			std::cout << "White wins";
			break;
		}
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
		else if (!strcmp(command, "help"))
		{
			std::cout << "\n\n-commands-\n\nclients : view clients\n\n"
				<< "rooms : view available rooms\n\n"
				<< "terminate_clients : disconnect all clients\n\n"
				<< "terminate_rooms : remove all rooms\n\n"
				<< "terminate_client (number) : disconnect selected client\n\n"
				<< "terminate_room (number) : remove selected room\n\n"
				<< "exit : end this server\n\n";
		}
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
		else if (!strcmp(command, "terminate_rooms"))
		{
			Clean_rooms();
			std::cout << "\n\nrooms terminated\n\n";
		}
		else if (!strcmp(command, "terminate_room"))
		{
			int index = 0;
			scanf_s("%d", &index);
			getchar();
			char* delete_room_name = Get_Room_NameRetAV(index);
			if (Remove_room(delete_room_name) == false)
				std::cout << "\n\nFailed to terminate client\n\n";
			else
				std::cout << "\n\nterminated\n\n";
			delete[] delete_room_name;
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

void SERVER::Add_room(SOCKET Clnt, char* room_name, int board_size, char* nickname)
{
	g_rooms_mutex.lock();
	g_room_owners.push_back(Clnt);
	g_room_names.push_back(room_name);
	g_room_board_sizes.push_back(board_size);
	g_room_nicknames.push_back(nickname);
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
		g_room_board_sizes.erase(g_room_board_sizes.begin() + index);
		char* delete_nickname = g_room_nicknames[index];
		g_room_nicknames.erase(g_room_nicknames.begin() + index);
		delete[] delete_nickname;
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
	{
		SOCKET sock = g_room_owners[0];
		g_room_owners.erase(g_room_owners.begin());
		g_serv_TCP->Remove_Clnt(sock);
	}
	while (g_room_board_sizes.empty() != 1)
		g_room_board_sizes.erase(g_room_board_sizes.begin());
	while (g_room_nicknames.empty() != 1)
	{
		char* nickname = g_room_nicknames[0];
		g_room_nicknames.erase(g_room_nicknames.begin());
		delete nickname;
	}
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

char* SERVER::Get_Room_NameRetAV(int index)
{
	char* new_room_name = new char[BUFSIZE_OF_ROOM_NAME];
	g_rooms_mutex.lock();
	char* room_name = g_room_names[index];
	memcpy_s(new_room_name, sizeof(char) * BUFSIZE_OF_ROOM_NAME,
		room_name, sizeof(char) * BUFSIZE_OF_ROOM_NAME);
	g_rooms_mutex.unlock();
	return new_room_name;
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
			<< " board size : " << g_room_board_sizes[i] << 'x' << g_room_board_sizes[i] 
		<< " nickname : " << g_room_nicknames[i] << '\n';
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
