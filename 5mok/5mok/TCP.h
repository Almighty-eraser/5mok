#pragma once
#include "BASE.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#pragma comment (lib, "ws2_32.lib")

class TCP
{
public:
	void StartTCPclnt(void);
	void SendPosOfStone(char x, char y);
	void SendChar(char decision);
	int Receive(void);
	void End(void);
private:
	SOCKET sock;
};



