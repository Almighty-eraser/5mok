#include "TCP.h" 

void ErrorHandling(const char* message)
{
	std::cout << '\n' << message << '\n';
	exit(-1);
}

void TCP::StartTCPclnt(void)
{
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
		ErrorHandling("Cannot startup TCP protocol");

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == SOCKET_ERROR)
		ErrorHandling("Cannot create socket");

	sockaddr_in Sockaddr{};

	if (bind(sock, reinterpret_cast<sockaddr*>(&Sockaddr), sizeof(Sockaddr)) == SOCKET_ERROR)
		ErrorHandling("Cannot bind socket");
}

void TCP::SendChar(char decision)
{
	if (send(sock, &decision, sizeof(decision), 0) == SOCKET_ERROR)
		ErrorHandling("Cannot send first data");
}

void TCP::SendPosOfStone(char x, char y)
{
	if (send(sock, &x, sizeof(char), 0) == SOCKET_ERROR)
		ErrorHandling("Cannot send any data");
	if (send(sock, &y, sizeof(char), 0) == SOCKET_ERROR)
		ErrorHandling("Cannot send any data");
}

int TCP::Receive(void)
{
	char pos;
	if (recv(sock, &pos, sizeof(pos), 0) == SOCKET_ERROR)
		ErrorHandling("Cannot receive any data");
	return (int)pos;
}

void TCP::End()
{
	closesocket(sock);
}