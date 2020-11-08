/*#include "TCP.h" 

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

void TCP::FirstSend(char decision)
{
	if (send(sock, &decision, sizeof(decision), 0) == SOCKET_ERROR)
		Error("Cannot send first data");
}

void TCP::Send(char row, char height)
{
	if (send(sock, &row, sizeof(row), 0) == SOCKET_ERROR)
		Error("Cannot send any data");
	if (send(sock, &height, sizeof(height), 0) == SOCKET_ERROR)
		Error("Cannot send any data");
}

int TCP::Receive(void)
{
	char pos;
	if (recv(sock, &pos, sizeof(pos), 0) == SOCKET_ERROR)
		Error("Cannot receive any data");
	return (int)pos;
}

void TCP::End()
{
	closesocket(sock);
}*/