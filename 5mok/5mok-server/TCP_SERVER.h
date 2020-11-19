#pragma once
#include "BASE.h"

#define WIN32_LEAN_AND_MEAN

#pragma comment (lib, "ws2_32.lib")

class TCP_SERVER
{
public:
	void StartTCPserver(int PORT);
	SOCKET WaitForClnt(void);
	void SendPosOfStone(char x, char y);
	void SendChar(char decision);
	char Receive(void);
	void End(void);
private:
	SOCKET sock;
	sockaddr_in Sockaddr{};
};



