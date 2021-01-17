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

int TCP_SERVER::SendChar(SOCKET Clnt, char decision)
{
	char socket[8];
	_itoa_s(Clnt, socket, 10);
	int ErrorOrNot;
	if (ErrorOrNot = send(Clnt, &decision, sizeof(decision), 0) == SOCKET_ERROR)
	{
		Log("Cannot send decision : " + decision + '\0');
		Log("To :");
		Log(socket);
		std::cout << GetLastError();
	}
	else
	{
		Log("sent decision : " + decision + '\0');
		Log("To :");
		Log(socket);
	}
	
	return ErrorOrNot;
}


void TCP_SERVER::SendPosOfStone(SOCKET Clnt, char x, char y)
{
	char socket[8];
	_itoa_s(Clnt, socket, 10);
	if (send(Clnt, &x, sizeof(char), 0) == SOCKET_ERROR)
		ErrorHandling("Cannot send coordinates");
	else
	{
		Log("sent a coordinate : " + x);
		Log("To :");
		Log(socket);
	}
	if (send(Clnt, &y, sizeof(char), 0) == SOCKET_ERROR)
		ErrorHandling("Cannot send a coordinate");
	else
	{
		Log("sent a coordinate : " + y);
		Log("To :");
		Log(socket);
	}
}

void TCP_SERVER::SendString(SOCKET Clnt, char* string, int size)
{
	char socket[8];
	_itoa_s(Clnt, socket, 10);
	if (send(Clnt, string, size, 0) == SOCKET_ERROR)
	{
		Log("Cannot send string :");
		Log(string);
		Log("To :");
		Log(socket);
	}
	else
	{
		Log("sent string :");
		Log(string);
		Log("To :");
		Log(socket);
	}
}

char TCP_SERVER::Receive(SOCKET Clnt)
{
	char chara;
	if (recv(Clnt, &chara, sizeof(chara), 0) == SOCKET_ERROR)
	{
		std::cout << GetLastError();
		exit(1);
	}
	Log("Received : " + chara);
	char socket[8];
	_itoa_s(Clnt, socket, 10);
	Log("From :");
	Log(socket);
	return chara;
}

char* TCP_SERVER::ReceiveStringRetAV(SOCKET Clnt, int size)//return allocated variable
{
	char* string = new char[size];
	if(recv(Clnt, string, size, 0) == SOCKET_ERROR)
		ErrorHandling("Cannot receive any data");
	char socket[8];
	_itoa_s(Clnt, socket, 10);
	Log("Received :");
	Log(string);
	Log("From :");
	Log(socket);
	return string;
}

void TCP_SERVER::End(SOCKET SOCK)
{
	char socket[8];
	_itoa_s(SOCK, socket, 10);
	Log("Ending socket :");
	Log(socket);
	closesocket(SOCK);
}