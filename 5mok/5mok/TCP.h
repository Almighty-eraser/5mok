#pragma once
#include "BASE.h"

#define WIN32_LEAN_AND_MEAN

#pragma comment (lib, "ws2_32.lib")

class TCP
{
public:
	int StartTCPclnt(void);
	void SendPosOfStone(char x, char y);
	void SendChar(char decision);
	char Receive();
	void End();
private:
	SOCKET sock;
};



