#pragma once
#include <WinSock2.h>

class TCP
{
public:
	void StartTCPclnt();
	void Send(char row, char height);
	void FirstSend(char decision);
	int Receive(void);
	void End(void);
private:
	SOCKET sock;
};

