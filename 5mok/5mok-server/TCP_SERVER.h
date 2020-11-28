#pragma once
#include "BASE.h"

#define WIN32_LEAN_AND_MEAN

#pragma comment (lib, "ws2_32.lib")

class TCP_SERVER
{
public:
	void StartTCPserver(int port);
	SOCKET AcceptClnt(void);
	void SendPosOfStone(SOCKET Clnt, char x, char y);
	int SendChar(SOCKET Clnt, char decision);
	void SendString(SOCKET Clnt, char* string, int size);
	char Receive(SOCKET Clnt);
	char* ReceiveStringRetAV(SOCKET Clnt, int size);
	void End(SOCKET Clnt);
private:
	SOCKET sock;
	sockaddr_in Sockaddr{0};
};



