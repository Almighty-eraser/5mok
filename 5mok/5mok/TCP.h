#pragma once
#include <WinSock2.h>

class TCP
{
public:
	void StartTCPclnt();
	void Send(int row, int height);
	int Receive(void);
	void End(void);
private:
	SOCKET sock;
};

