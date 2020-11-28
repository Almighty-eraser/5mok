#pragma once

#include <conio.h>
#include <thread>
#include "TCP_SERVER.h"

void Log(const char* stringmessage);

class SERVER
{
public:
	SERVER(TCP_SERVER* tcp) { serv_TCP = tcp; Log("SERVER class started"); };
	~SERVER() {
		for (int i = 0; i < titles.size(); i++)
			if(titles[i] != NULL)
				delete titles[i];
		for (int i = 0; i < rooms.size(); i++)
			if (rooms[i] != NULL)
				serv_TCP->End(rooms[i]);
		Log("SERVER class closed");
	}

	void Run(void);
	void MakingRoom(SOCKET Clnt);
	void SendRoomList(SOCKET Clnt);
	void ChoosingRoom(SOCKET Clnt);
	void DeletingRoom(SOCKET Clnt);
	void Play(SOCKET black_clnt, SOCKET white_clnt);

	void EndServer(bool* RunServer);
private:
	TCP_SERVER* serv_TCP;
	std::vector<SOCKET> rooms;
	std::vector<char*> titles;
	bool RoomFlag = false;
};

