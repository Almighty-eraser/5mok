#pragma once

#include <iostream>
#include <conio.h>
#include <thread>
#include "BASE.h"
#include "TCP_SERVER.h"

class SERVER
{
public:
	SERVER(TCP_SERVER& tcp) { serv_TCP = &tcp; };
	~SERVER() {
		for (int i = 0; i < titles.size(); i++)
			delete titles[i];
		for (int i = 0; i < rooms.size(); i++)
			if (rooms[i] != NULL)
				serv_TCP->End(rooms[i]);
	}

	void Run(void);
	void MakingRoom(void);
	void ShowRooms(void);
	void ChoosingRoom(void);
	void Play(SOCKET black_clnt, SOCKET white_clnt);

	void EndServer(void);
private:
	TCP_SERVER* serv_TCP;
	TCP_SERVER serv_makingroom;
	TCP_SERVER serv_showrooms;
	TCP_SERVER serv_choosingroom;
	TCP_SERVER serv_play;
	std::vector<SOCKET> rooms;
	std::vector<char*> titles;
	bool RoomFlag = false;
};

