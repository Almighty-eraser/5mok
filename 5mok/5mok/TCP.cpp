#include "TCP.h" 

void ErrorHandling(const char* message)
{
	std::cout << '\n' << message << '\n';
	exit(-1);
}

int TCP::StartTCPclnt(void)
{
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
		ErrorHandling("Cannot startup TCP protocol");

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == SOCKET_ERROR)
		ErrorHandling("Cannot create socket");

	sockaddr_in Sockaddr{};

	Sockaddr.sin_family = AF_INET;
	Sockaddr.sin_addr.S_un.S_addr = htonl(atoi(IP_Address));
	Sockaddr.sin_port = htons(SERVER_PORT);

	if (connect(sock, reinterpret_cast<sockaddr*>(&Sockaddr), sizeof(Sockaddr)) == SOCKET_ERROR)
	{
		puts("\nCannot connect to server\n");
		return -1;
	}
	puts("\nConnected to Server\n");
	return 1;
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

char TCP::Receive(void)
{
	char pos;
	if (recv(sock, &pos, sizeof(pos), 0) == SOCKET_ERROR)
		ErrorHandling("Cannot receive any data");
	return pos;
}

void TCP::End(void)
{
	closesocket(sock);
}