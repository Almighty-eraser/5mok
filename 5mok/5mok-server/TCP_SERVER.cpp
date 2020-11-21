#include "TCP_SERVER.h" 

void ErrorHandling(const char* message)
{
	std::cout << '\n' << message << '\n';
	exit(-1);
}

void TCP_SERVER::StartTCPserver(int PORT)
{
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
		ErrorHandling("Cannot startup TCP protocol");

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == SOCKET_ERROR)
		ErrorHandling("Cannot create socket");

	Sockaddr.sin_family = AF_INET;
	Sockaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	Sockaddr.sin_port = htons(PORT);

	if (bind(sock, reinterpret_cast<sockaddr*>(&Sockaddr), sizeof(Sockaddr)) == SOCKET_ERROR)
		ErrorHandling("Cannot bind socket");
}

SOCKET TCP_SERVER::WaitForClnt(void)
{
	SOCKET FacingWithClnt;

	if (listen(sock, 1) == SOCKET_ERROR)
		ErrorHandling("listen error");
	
	int size_of_addr = sizeof(Sockaddr);
	if (FacingWithClnt = accept(sock, reinterpret_cast<sockaddr*>(&Sockaddr), &size_of_addr) == SOCKET_ERROR)
		ErrorHandling("accept error");

	return FacingWithClnt;
}

void TCP_SERVER::SendChar(SOCKET Clnt, char decision)
{
	if (send(Clnt, &decision, sizeof(decision), 0) == SOCKET_ERROR)
		ErrorHandling("Cannot send first data");
}

void TCP_SERVER::SendPosOfStone(SOCKET Clnt, char x, char y)
{
	if (send(Clnt, &x, sizeof(char), 0) == SOCKET_ERROR)
		ErrorHandling("Cannot send any data");
	if (send(Clnt, &y, sizeof(char), 0) == SOCKET_ERROR)
		ErrorHandling("Cannot send any data");
}

char TCP_SERVER::Receive(SOCKET Clnt)
{
	char pos;
	if (recv(Clnt, &pos, sizeof(pos), 0) == SOCKET_ERROR)
		ErrorHandling("Cannot receive any data");
	return pos;
}

void TCP_SERVER::End(SOCKET SOCK)
{
	closesocket(SOCK);
}