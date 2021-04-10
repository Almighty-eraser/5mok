#pragma once
#include "server-BASE.h"
#include <conio.h>

#define WIN32_LEAN_AND_MEAN

#pragma comment (lib, "ws2_32.lib")

class TCP_SERVER
{
public:
	void StartTCPserver(int port);
	void EndTCPserver(void) 
	{ 
		Print_Time();
		std::cout << "Server socket closing.."; 
		WSACleanup(); 
		this->End(sock); 
	};
	SOCKET AcceptClnt(void);
	void SendPosOfStone(SOCKET Clnt, char x, char y);
	int SendChar(SOCKET Clnt, char decision);
	int SendInt(SOCKET Clnt, int data);
	void SendString(SOCKET Clnt, char* string, int size);
	char Receive(SOCKET Clnt);
	char* ReceiveStringRetAV(SOCKET Clnt, int size);
	void End(SOCKET Clnt);
private:
	SOCKET sock;
	SOCKADDR_IN serv_addr {0};
};



