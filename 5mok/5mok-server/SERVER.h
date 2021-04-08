#pragma once

#include <conio.h>
#include "TCP_SERVER.h"

TCP_SERVER* serv_TCP;

std::vector<SOCKET> socks;
std::vector<char*> room_names;
bool End_Server = false;

std::mutex socket_mutex;
std::mutex room_names_mutex;

class SERVER
{
public:
	SERVER(TCP_SERVER* tcp)
	{ 
		serv_TCP = tcp; 
		Print_Time(); 
		std::cout << "SERVER class started"; 
		functions.push_back(&SERVER::MakingRoom);
		functions.push_back(&SERVER::SendRoomList);
		functions.push_back(&SERVER::ChoosingRoom);
		functions.push_back(&SERVER::DeletingRoom);
	};
	~SERVER() {
		
		serv_TCP->EndTCPserver();
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

	void add_sock(SOCKET* Clnt)
	{
		socket_mutex.lock();
		socks.push_back(Clnt);
		socket_mutex.unlock();
	}
	void add_room_name(char* room)
	{
		room_names_mutex.lock();
		room_names.push_back(room);
		room_names_mutex.unlock();
	}

private:
	std::vector<void (SERVER::*)(SOCKET)> functions;
};

