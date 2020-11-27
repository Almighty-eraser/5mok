#pragma once

#include <iostream>
#include <conio.h>
#include <thread>
#include "BASE.h"
#include "TCP_SERVER.h"

class SERVER
{
public:
	SERVER(TCP_SERVER& tcp) { serv_TCP = &tcp; LeaveLog("SERVER class started"); };
	~SERVER() {
		for (int i = 0; i < titles.size(); i++)
			if(titles[i] != NULL)
				delete titles[i];
		for (int i = 0; i < rooms.size(); i++)
			if (rooms[i] != NULL)
				serv_TCP->End(rooms[i]);
		LeaveLog("SERVER class closed");
	}

	void Run(void);
	void MakingRoom(SOCKET Clnt);
	void ShowRooms(SOCKET Clnt);
	void ChoosingRoom(SOCKET Clnt);
	void DeletingRoom(SOCKET Clnt);
	void Play(SOCKET black_clnt, SOCKET white_clnt);

	void EndServer(void);
private:
	TCP_SERVER* serv_TCP;
	std::vector<SOCKET> rooms;
	std::vector<char*> titles;
	bool RoomFlag = false;
};

