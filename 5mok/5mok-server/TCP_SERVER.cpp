#include "TCP_SERVER.h" 

void ErrorHandling(const char* message)
{
	std::cout << '\n' << message << '\n';
	exit(1);
}

void TCP_SERVER::StartTCPserver(int port)
{
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
		ErrorHandling("Cannot startup TCP protocol");

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == SOCKET_ERROR)
		ErrorHandling("Cannot create socket");


	Sockaddr.sin_family = AF_INET;
	Sockaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	Sockaddr.sin_port = htons(port);

	if (bind(sock, reinterpret_cast<sockaddr*>(&Sockaddr), sizeof(Sockaddr)) == SOCKET_ERROR)
		ErrorHandling("Cannot bind socket");

	if (listen(sock, SOMAXCONN) == SOCKET_ERROR)
		ErrorHandling("listen error");
}

SOCKET TCP_SERVER::AcceptClnt(void)
{
	SOCKET Clnt;
	
	int size_of_addr = sizeof(Sockaddr);
	if (Clnt = accept(sock, reinterpret_cast<sockaddr*>(&Sockaddr), &size_of_addr) == SOCKET_ERROR)
		ErrorHandling("accept error");
	char socket[8];
	_itoa_s(Clnt, socket, 10);
	Log("accepted clnt : ");
	Log(socket);
	
	return Clnt;
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
	char pos;
	if (recv(Clnt, &pos, sizeof(pos), 0) == SOCKET_ERROR)
		ErrorHandling("Cannot receive any data");
	Log("Received : " + pos);
	char socket[8];
	_itoa_s(Clnt, socket, 10);
	Log("From :");
	Log(socket);
	return pos;
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