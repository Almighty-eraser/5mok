#include "TCP_SERVER.h" 

void ErrorHandling(const char* message)
{
	std::cout << '\n' << message << '\n';
	exit(-1);
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
	SOCKET FacingWithClnt;
	
	int size_of_addr = sizeof(Sockaddr);
	if (FacingWithClnt = accept(sock, reinterpret_cast<sockaddr*>(&Sockaddr), &size_of_addr) == SOCKET_ERROR)
		ErrorHandling("accept error");
	LeaveLog("accepted clnt : " + FacingWithClnt);
	

	return FacingWithClnt;
}

int TCP_SERVER::SendChar(SOCKET Clnt, char decision)
{
	int ErrorOrNot;
	if (ErrorOrNot = send(Clnt, &decision, sizeof(decision), 0) == SOCKET_ERROR)
		LeaveLog("Cannot send decision : " + decision + ' ' + 't' + 'o' + ' ' + Clnt);
	else
		LeaveLog("sent decision : " + decision + ' ' + 't' + 'o' + ' ' + Clnt);
	
	return ErrorOrNot;
}


void TCP_SERVER::SendPosOfStone(SOCKET Clnt, char x, char y)
{
	if (send(Clnt, &x, sizeof(char), 0) == SOCKET_ERROR)
		LeaveLog("Cannot send coordinates" + ' ' + x + ' ' + Clnt);
	else
		LeaveLog("sent coordinates" + ' ' + x + ' ' + Clnt);
	if (send(Clnt, &y, sizeof(char), 0) == SOCKET_ERROR)
		LeaveLog("sent coordinates" + ' ' + y + ' ' + Clnt);
	else
		LeaveLog("sent coordinates" + ' ' + y + ' ' + Clnt);
}

void TCP_SERVER::SendString(SOCKET Clnt, char* string, int size)
{
	if (send(Clnt, string, size, 0) == SOCKET_ERROR)
	{
		LeaveLog("Cannot send string to : " + Clnt);
		LeaveLog(string);
	}
	else
	{
		LeaveLog("sent string to : " + Clnt);
		LeaveLog(string);
	}
}

char TCP_SERVER::Receive(SOCKET Clnt)
{
	char pos;
	if (recv(Clnt, &pos, sizeof(pos), 0) == SOCKET_ERROR)
		ErrorHandling("Cannot receive any data");
	return pos;
}

char* TCP_SERVER::ReceiveStringRN(SOCKET Clnt, int size)
{
	char* string = new char[size];
	if(recv(Clnt, string, size, 0) == SOCKET_ERROR)
		ErrorHandling("Cannot receive any data");
	return string;
}

void TCP_SERVER::End(SOCKET SOCK)
{
	closesocket(SOCK);
}