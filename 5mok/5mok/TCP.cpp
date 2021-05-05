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
	int byte = 0;
	while (byte += send(sock, &decision, sizeof(char), 0) > SOCKET_ERROR)
	{
		if (byte >= sizeof(char))
			break;
	}
	if(byte <= SOCKET_ERROR)
		puts("\nCannot send data\n");

	return byte;
}

int TCP::SendString(char* string, int size)
{
	int byte = 0;
	while (byte += send(sock, string, sizeof(char) * size, 0) > SOCKET_ERROR)
	{
		if (byte >= size)
			break;
	}
	if (byte <= SOCKET_ERROR)
		puts("\nCannot send string\n");

	return byte;
}

int TCP::SendPosOfStone(int x, int y)
{
	int byte = 0;
	char _x = (char)x;
	char _y = (char)y;
	byte += this->SendChar(_x);
	byte += this->SendChar(_y);
	return byte;
}

int TCP::Receive(char* receive)
{
	int byte = 0;
	while (byte += recv(sock, receive, sizeof(char), 0) > SOCKET_ERROR)
	{
		if (byte >= sizeof(char))
			break;
	}
	if (byte <= SOCKET_ERROR)
	{
		std::cout << "\nCannot send data\n";
	}
	return byte;
}

int* TCP::ReceiveIntRetAV(void)
{
	int* data = new int;
	int byte = 0;
	while (byte += recv(sock, (char*)data, sizeof(int), 0) > SOCKET_ERROR)
	{
		if (byte >= sizeof(int))
			break;
	}
	if(byte <= SOCKET_ERROR)
		ErrorHandling("Cannot receive any data");
	return data;
}

int TCP::Receive_Non_Blocking(char* receive)
{
	int byte;
	byte = recv(sock, receive, sizeof(char), 0);
	return byte;
}

char* TCP::ReceiveStringRetAV(int size)//return allocated variable
{
	char* string = new char[size];
	int byte = 0;
	while (byte += recv(sock, string, size, 0) > SOCKET_ERROR)
	{
		if (byte >= BUFSIZE_OF_ROOM_NAME)
			break;
	}
	if (byte <= SOCKET_ERROR)
		ErrorHandling("Cannot receive room_name");
	return string;
}

bool TCP::Change_IOMode(int mode)
{
	u_long Mode = mode;
	int Error;
	if (Error = ioctlsocket(sock, FIONBIO, &Mode) == SOCKET_ERROR)
		ErrorHandling("Cannot change the mode of io");
	if (Error == 0)
		return true;
	else
		return false;
}

void TCP::End(void)
{
	WSACleanup();
	closesocket(sock);
}