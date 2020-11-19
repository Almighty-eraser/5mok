#pragma once
#include "BASE.h"

#define WIN32_LEAN_AND_MEAN

#pragma comment (lib, "ws2_32.lib")

class TCP
{
public:
	void StartTCPclnt(int PORT);
	void SendPosOfStone(char x, char y);
	void SendChar(char decision);
	char Receive(void);
	void End(void);
private:
	SOCKET sock;
};



