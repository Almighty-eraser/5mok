#pragma once

#include <conio.h>
#include "TCP_SERVER.h"

class SERVER
{
public:
	SERVER(TCP_SERVER* tcp);
	~SERVER();

	void Run(void);
	void MakingRoom(SOCKET Clnt);
	void SendRoomList(SOCKET Clnt);
	void ChoosingRoom(SOCKET Clnt);
	void DeletingRoom(SOCKET Clnt);
	void Play(SOCKET black_clnt, SOCKET white_clnt);
	void Commands(void);

	void Add_room(SOCKET Clnt, char* room_name, int board_size, char* nickname);
	bool Remove_room(char* room_name);
	void Clean_rooms(void);
	int Get_Rooms_Size(void);
	char* Get_Room_NameRetAV(int index);
	bool Print_Rooms(void);
	int Find_Room(char* room_name);
private:
	std::vector<void (SERVER::*)(SOCKET)> functions;
};

