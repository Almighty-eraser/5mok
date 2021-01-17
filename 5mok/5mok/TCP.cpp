#include "TCP.h" 

void ErrorHandling(const char* message)
{
	std::cout << '\n' << message << '\n'
		<< "\nPress any key to exit...";
	_getch();
	exit(1);
}

int TCP::StartTCPclnt(void)
{
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
		ErrorHandling("Cannot startup TCP protocol");

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == SOCKET_ERROR)
		ErrorHandling("Cannot create socket");

	sockaddr_in Sockaddr{};

	Sockaddr.sin_family = AF_INET;
	Sockaddr.sin_addr.S_un.S_addr = inet_addr(IP_Address);
	Sockaddr.sin_port = htons(SERVER_PORT);

	if (connect(sock, reinterpret_cast<SOCKADDR*>(&Sockaddr), sizeof(Sockaddr)) == SOCKET_ERROR)
	{
		puts("\nCannot connect to server\n");
		return -1;
	}
	puts("\nConnected to Server\n");
	return 1;
}

int TCP::SendChar(char decision)
{
	int result;
	if (result = send(sock, &decision, sizeof(char), 0) == SOCKET_ERROR)
		puts("\nCannot send data\n");

	return result;
}

int TCP::SendString(char* string, int size)
{
	int result = send(sock, string, sizeof(char) * size, 0);
	if (result == SOCKET_ERROR)
		puts("\nCannot send string\n");

	return result;
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

char* TCP::ReceiveStringRetAV(int size)//return allocated variable
{
	char* string = new char[size];
	if (recv(sock, string, size, 0) == SOCKET_ERROR)
		ErrorHandling("Cannot receive any data");
	return string;
}

void TCP::End(void)
{
	WSACleanup();
	closesocket(sock);
}