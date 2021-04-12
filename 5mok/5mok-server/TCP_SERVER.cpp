#include "TCP_SERVER.h" 

void ErrorHandling(const char* message)
{
	std::cout << '\n' << message << '\n'
		<< "\nPress any key to exit...";
	_getch();
	_getch();
	exit(1);
}

void TCP_SERVER::StartTCPserver(int port)
{
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
		ErrorHandling("Cannot startup TCP protocol");

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == SOCKET_ERROR)
		ErrorHandling("Cannot create socket");

	memset(&serv_addr, 0, sizeof(SOCKADDR_IN));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);

	if (bind(sock, reinterpret_cast<SOCKADDR*>(&serv_addr), sizeof(serv_addr)) == SOCKET_ERROR)
		ErrorHandling("Cannot bind socket");

	if (listen(sock, SOMAXCONN) == SOCKET_ERROR)
		ErrorHandling("listen error");
}

SOCKET TCP_SERVER::AcceptClnt(void)
{
	SOCKADDR_IN clnt_addr{0};
	
	int size_of_addr = sizeof(clnt_addr);
	return accept(sock, reinterpret_cast<SOCKADDR*>(&clnt_addr), &size_of_addr);
}

int TCP_SERVER::SendChar(SOCKET Clnt, char data)
{
	int ErrorOrNot;
	if (ErrorOrNot = send(Clnt, &data, sizeof(char), 0) == SOCKET_ERROR)
	{
		Print_Time();
		std::cout << "\nCannot send decision : " << data
			<< "\nTo : " << Clnt << '\n' << "Error : "
			<< GetLastError();
	}
	else
	{
		Print_Time();
		std::cout << "\nsent decision : " << data
			<< "\nTo : " << Clnt;
	}
	
	return ErrorOrNot;
}

int TCP_SERVER::SendInt(SOCKET Clnt, int data)
{
	int ErrorOrNot;
	if (ErrorOrNot = send(Clnt, (const char*)&data, sizeof(int), 0) == SOCKET_ERROR)
	{
		Print_Time();
		std::cout << "\nCannot send decision : " << data
			<< "\nTo : " << Clnt << '\n' << "Error : "
			<< GetLastError();
	}
	else
	{
		Print_Time();
		std::cout << "\nsent decision : " << data
			<< "\nTo : " << Clnt;
	}

	return ErrorOrNot;
}


void TCP_SERVER::SendPosOfStone(SOCKET Clnt, char x, char y)
{
	if (send(Clnt, &x, sizeof(char), 0) == SOCKET_ERROR)
	{
		Print_Time();
		std::cout << "Cannot send x : " << (int)x << "\nTo : "
			<< Clnt << "\nError : " << GetLastError();
	}
	else
	{
		Print_Time();
		std::cout << "Cannot send x : " << (int)x << "\nTo : "
			<< Clnt;
	}
	if (send(Clnt, &y, sizeof(char), 0) == SOCKET_ERROR)
	{
		Print_Time();
		std::cout << "Cannot send y : " << (int)y << "\nTo : "
			<< Clnt << "\nError : " << GetLastError();
	}
	else
	{
		Print_Time();
		std::cout << "Cannot send y : " << (int)y << "\nTo : "
			<< Clnt;
	}
}

void TCP_SERVER::SendString(SOCKET Clnt, char* string, int size)
{
	if (send(Clnt, string, size, 0) == SOCKET_ERROR)
	{
		Print_Time();
		std::cout << "Cannot send string : " << string << "\nTo : "
			<< Clnt << "\nError : " << GetLastError();
	}
	else
	{
		Print_Time();
		std::cout << "Cannot send string : " << string << "\nTo : "
			<< Clnt;
	}
}

char TCP_SERVER::Receive(SOCKET Clnt)
{
	char ch;
	if (recv(Clnt, &ch, sizeof(ch), 0) == SOCKET_ERROR)
	{
		Print_Time();
		std::cout << "Cannot receive : " << ch << "\nFrom : "
			<< Clnt << "\nError : " << GetLastError();
	}
	else
	{
		Print_Time();
		std::cout << "Received : " << ch << "\nFrom : " << Clnt;
	}
	return ch;
}

char* TCP_SERVER::ReceiveStringRetAV(SOCKET Clnt, int size)//return allocated variable
{
	char* string = new char[size];
	if(recv(Clnt, string, size, 0) == SOCKET_ERROR)
		ErrorHandling("Cannot receive any data");
	Print_Time();
	std::cout << "Received : " << string << "\nFrom : " << Clnt;

	return string;
}

void TCP_SERVER::End(SOCKET SOCK)
{
	Print_Time();
	std::cout << "End socket : " << SOCK;
	closesocket(SOCK);
}