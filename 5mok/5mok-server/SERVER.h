#pragma once

#include <conio.h>
#include "TCP_SERVER.h"

TCP_SERVER* g_serv_TCP;

std::vector<SOCKET> g_socks;
std::vector<char*> g_room_names;
bool g_End_Server = false;

std::mutex g_rooms_mutex;

class SERVER
{
public:
	SERVER(TCP_SERVER* tcp)
	{ 
		g_serv_TCP = tcp; 
		Print_Time(); 
		std::cout << "SERVER class started"; 
		functions.push_back(&SERVER::MakingRoom);
		functions.push_back(&SERVER::SendRoomList);
		functions.push_back(&SERVER::ChoosingRoom);
		functions.push_back(&SERVER::DeletingRoom);
	};
	~SERVER() {
		
		g_serv_TCP->EndTCPserver();
		Print_Time();
		std::cout << "SERVER class closed";
	}

	void Run(void);
	void MakingRoom(SOCKET Clnt);
	void SendRoomList(SOCKET Clnt);
	void ChoosingRoom(SOCKET Clnt);
	void DeletingRoom(SOCKET Clnt);
	void Play(SOCKET black_clnt, SOCKET white_clnt);
	void EndServer(void);

	void Add_room(SOCKET Clnt, char* room_name)
	{
		g_rooms_mutex.lock();
		g_socks.push_back(Clnt);
		g_room_names.push_back(room_name);
		g_rooms_mutex.unlock();
	}
	bool Remove_room(char* room_name)
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
	void Clean_rooms(void)
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
private:
	std::vector<void (SERVER::*)(SOCKET)> functions;
};

