#include "TCP.h"
#include "BASE.h"  

void TCP::StartTCPclnt()
{
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
		Error("Cannot startup TCP protocol");

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == SOCKET_ERROR)
		Error("Cannot create socket");

	sockaddr_in Sockaddr{};

	if (bind(sock, reinterpret_cast<sockaddr*>(&Sockaddr), sizeof(Sockaddr)) == SOCKET_ERROR)
		Error("Cannot bind socket");
}

void TCP::Send(int row, int height)
{

}

int TCP::Receive(void)
{

}

void TCP::End()
{

}