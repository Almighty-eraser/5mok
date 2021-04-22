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
		std::cout << "Waiting for clients to end..\n\n";
		Clean_Clnts();
		std::cout << "Server socket closing..\n\n"; 
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

	void Add_Clnt(SOCKET Clnt, SOCKADDR_IN* Clntinfo);
	void Remove_Clnt(SOCKET Clnt);
	void Clean_Clnts(void);
	int Get_Clnts_Size(void);
	SOCKADDR_IN* Get_Recent_ClntInfo(void);
	bool Print_Clnts_Infos(void);
private:
	SOCKET sock;
	SOCKADDR_IN serv_addr {0};
	SOCKADDR_IN clnt_addr {0};


	std::vector<SOCKET> Clnts;
	std::vector<SOCKADDR_IN*> ClntsInfo;
	std::mutex Clnts_mutex;
};



