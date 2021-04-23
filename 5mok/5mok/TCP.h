#pragma once
#include "BASE.h"
#include <conio.h>

#define WIN32_LEAN_AND_MEAN

#pragma comment (lib, "ws2_32.lib")

class TCP
{
public:
	int StartTCPclnt(void);
	void SendPosOfStone(char x, char y);
	int SendChar(char decision);
	int SendString(char* string, int size);
	char Receive(void);
	int Receive_Non_Blocking(char* receive);
	char* ReceiveStringRetAV(int size);
	bool Change_IOMode(int mode);
	void End(void);
private:
	SOCKET sock;
};



