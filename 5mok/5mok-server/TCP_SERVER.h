#pragma once
#include "BASE.h"

#define WIN32_LEAN_AND_MEAN

#pragma comment (lib, "ws2_32.lib")

class TCP_SERVER
{
public:
	void StartTCPserver(int PORT);
	SOCKET WaitForClnt(void);
	void SendPosOfStone(SOCKET Clnt, char x, char y);
	void SendChar(SOCKET Clnt, char decision);
	char Receive(SOCKET Clnt);
	void End(SOCKET SOCK);
private:
	SOCKET sock;
	sockaddr_in Sockaddr{};
};



