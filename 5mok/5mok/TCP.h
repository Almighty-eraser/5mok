#pragma once
#include "BASE.h"

#define WIN32_LEAN_AND_MEAN

#pragma comment (lib, "ws2_32.lib")

class TCP
{
public:
	void StartTCPclnt(SOCKET sock, int PORT);
	void SendPosOfStone(SOCKET sock, char x, char y);
	void SendChar(SOCKET sock, char decision);
	char Receive(SOCKET sock);
	void End(SOCKET sock);
};



