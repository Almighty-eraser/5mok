#include "TCP_SERVER.h" 

void ErrorHandling(const char* message)
{
	std::cout << '\n' << message << '\n'
		<< "\nPress any key to exit... (Error : " << GetLastError() << ')';
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
	SOCKADDR_IN* clnt_addr = new SOCKADDR_IN;
	ClntsInfo.push_back(clnt_addr);
	int size_of_addr = sizeof(SOCKADDR_IN);
	memset(clnt_addr, 0, size_of_addr);
	return accept(sock, reinterpret_cast<SOCKADDR*>(clnt_addr), &size_of_addr);
}

int TCP_SERVER::SendChar(SOCKET Clnt, char data)
{
	int byte = 0;
	while (byte += send(Clnt, &data, sizeof(char), 0) > SOCKET_ERROR)
	{
		if (byte >= sizeof(char))
			break;
	}
	if (byte <= SOCKET_ERROR)
	{
		Print_Time();
		std::cout << "\nCannot send decision : " << data
			<< "\nTo : " << Clnt << '\n' << "Error : "
			<< GetLastError() << "\n\n";
	}
	else
	{
		Print_Time();
		std::cout << "\nsent decision : " << data
			<< "\nTo : " << Clnt << "\n\n";
	}
	
	return byte;
}

int TCP_SERVER::SendInt(SOCKET Clnt, int data)
{
	int byte = 0;
	while (byte += send(Clnt, (const char*)&data, sizeof(int), 0) > SOCKET_ERROR)
	{
		if (byte >= sizeof(int))
			break;
	}
	if (byte <= SOCKET_ERROR)
	{
		Print_Time();
		std::cout << "\nCannot send decision : " << data
			<< "\nTo : " << Clnt << '\n' << "Error : "
			<< GetLastError() << "\n\n";
	}
	else
	{
		Print_Time();
		std::cout << "\nsent decision : " << data
			<< "\nTo : " << Clnt << "\n\n";
	}

	return byte;
}

int TCP_SERVER::SendPosOfStone(SOCKET Clnt, char x, char y)
{
	int byte = 0;
	byte += this->SendChar(Clnt, x);
	byte += this->SendChar(Clnt, y);
	return byte;
}

int TCP_SERVER::SendString(SOCKET Clnt, char* string, int size)
{
	int byte = 0;
	while (byte += send(Clnt, string, size * sizeof(char), 0) > SOCKET_ERROR)
	{
		if (byte >= BUFSIZE_OF_ROOM_NAME)
			break;
	}
	if (byte <= SOCKET_ERROR)
	{
		Print_Time();
		std::cout << "Cannot send string : " << string << "\nTo : "
			<< Clnt << " Error : " << GetLastError() << "\n\n";
	}
	else 
	{
		Print_Time();
		std::cout << "sent string : " << string << "\nTo : "
			<< Clnt << "\n\n";
	}
	return byte;
}

int TCP_SERVER::Receive(SOCKET Clnt, char* receive)
{
	int byte = 0;
	while (byte += recv(Clnt, receive, sizeof(char), 0) > SOCKET_ERROR)
	{
		if (byte >= sizeof(char))
			break;
	}
	if (byte <= SOCKET_ERROR)
	{
		Print_Time();
		std::cout << "Cannot receive : " << (int)*receive << "\nFrom : "
			<< Clnt << "\nError : " << GetLastError() << "\n\n";
	}
	else
	{
		Print_Time();
		std::cout << "Received : " << (int)*receive << "\nFrom : " << Clnt << "\n\n";
	}
	return byte;
}

char* TCP_SERVER::ReceiveStringRetAV(SOCKET Clnt, int size)//return allocated variable
{
	char* string = new char[size];
	int byte = 0;
	while (byte += recv(Clnt, string, size, 0) > SOCKET_ERROR)
	{
		if (byte >= size)
			break;
	}
	if (byte <= SOCKET_ERROR)
	{
		Print_Time();
		std::cout << "Cannot receive string " << "\nFrom : "
			<< Clnt << "\nError : " << GetLastError() << "\n\n";
	}
	else
	{
		Print_Time();
		std::cout << "Received : " << string << "\nFrom : " << Clnt << "\n\n";
	}
	return string;
}

void TCP_SERVER::End(SOCKET SOCK)
{
	Print_Time();
	std::cout << "End socket : " << SOCK << "\n\n";
	closesocket(SOCK);
}

void TCP_SERVER::Add_Clnt(SOCKET Clnt)
{
	Clnts.push_back(Clnt);
}

bool TCP_SERVER::Remove_Clnt(SOCKET Clnt)
{
	if (ClntsInfo.empty() && Clnts.empty())
		return false;
	int index = -1;
	for(int i = 0; i < Clnts.size(); i++)
		if (Clnts[i] == Clnt)
		{
			index = i;
			break;
		}
	if (index != -1)
	{
		SOCKADDR_IN* sockaddr = ClntsInfo[index];
		ClntsInfo.erase(ClntsInfo.begin() + index);
		delete sockaddr;
		SOCKET sock = Clnts[index];
		Clnts.erase(Clnts.begin() + index);
		this->End(sock);
		return true;
	}
	return false;
}

void TCP_SERVER::Clean_Clnts(void)
{
	if (ClntsInfo.empty() && Clnts.empty())
		return;
	while(ClntsInfo.empty() != 1)
	{
		SOCKADDR_IN* sockaddr = ClntsInfo[0];
		ClntsInfo.erase(ClntsInfo.begin());
		delete sockaddr;
	}
	while(Clnts.empty() != 1)
	{
		SOCKET sock = Clnts[0];
		Clnts.erase(Clnts.begin());
		this->End(sock);
	}
}

int TCP_SERVER::Get_Clnts_Size(void)
{
	int size;
	size = Clnts.size();
	return size;
}

SOCKET TCP_SERVER::Get_Clnt_SOCKET(int index)
{
	if (Clnts.empty())
		return -1;
	return Clnts[index];
}

bool TCP_SERVER::Print_Clnts_Info(void)
{
	if (Clnts.empty())
		return false;
	for (int i = 0; i < Clnts.size(); i++)
	{
		char Buffer[10];
		std::cout << i + 1 << " SOCKET : " << Clnts[i] << " ip : " << inet_ntoa(ClntsInfo[i]->sin_addr)
			<< '\n';
	}
	puts("\n");
	return true;
}