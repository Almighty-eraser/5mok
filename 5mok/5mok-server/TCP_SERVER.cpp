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
			<< GetLastError() << "\n\n";
	}
	else
	{
		Print_Time();
		std::cout << "\nsent decision : " << data
			<< "\nTo : " << Clnt << "\n\n";
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
			<< GetLastError() << "\n\n";
	}
	else
	{
		Print_Time();
		std::cout << "\nsent decision : " << data
			<< "\nTo : " << Clnt << "\n\n";
	}

	return ErrorOrNot;
}

void TCP_SERVER::SendPosOfStone(SOCKET Clnt, char x, char y)
{
	if (send(Clnt, &x, sizeof(char), 0) == SOCKET_ERROR)
	{
		Print_Time();
		std::cout << "Cannot send x : " << (int)x << "\nTo : "
			<< Clnt << "\nError : " << GetLastError() << "\n\n";
	}
	else
	{
		Print_Time();
		std::cout << "Cannot send x : " << (int)x << "\nTo : "
			<< Clnt << "\n\n";
	}
	if (send(Clnt, &y, sizeof(char), 0) == SOCKET_ERROR)
	{
		Print_Time();
		std::cout << "Cannot send y : " << (int)y << "\nTo : "
			<< Clnt << "\nError : " << GetLastError() << "\n\n";
	}
	else
	{
		Print_Time();
		std::cout << "Cannot send y : " << (int)y << "\nTo : "
			<< Clnt << "\n\n";
	}
}

void TCP_SERVER::SendString(SOCKET Clnt, char* string, int size)
{
	if (send(Clnt, string, size, 0) == SOCKET_ERROR)
	{
		Print_Time();
		std::cout << "Cannot send string : " << string << "\nTo : "
			<< Clnt << "\nError : " << GetLastError() << "\n\n";
	}
	else
	{
		Print_Time();
		std::cout << "Cannot send string : " << string << "\nTo : "
			<< Clnt << "\n\n";
	}
}

char TCP_SERVER::Receive(SOCKET Clnt)
{
	char ch;
	if (recv(Clnt, &ch, sizeof(ch), 0) == SOCKET_ERROR)
	{
		Print_Time();
		std::cout << "Cannot receive : " << ch << "\nFrom : "
			<< Clnt << "\nError : " << GetLastError() << "\n\n";
	}
	else
	{
		Print_Time();
		std::cout << "Received : " << ch << "\nFrom : " << Clnt << "\n\n";
	}
	return ch;
}

char* TCP_SERVER::ReceiveStringRetAV(SOCKET Clnt, int size)//return allocated variable
{
	char* string = new char[size];
	if (recv(Clnt, string, size, 0) == SOCKET_ERROR)
	{
		Print_Time();
		std::cout << "Cannot receive string " << "\nFrom : "
			<< Clnt << "\nError : " << GetLastError() << "\n\n";
	}
	Print_Time();
	std::cout << "Received : " << string << "\nFrom : " << Clnt << "\n\n";

	return string;
}

void TCP_SERVER::End(SOCKET SOCK)
{
	Print_Time();
	std::cout << "End socket : " << SOCK << "\n\n";
	closesocket(SOCK);
}

void TCP_SERVER::Add_clnt(SOCKET Clnt, SOCKADDR_IN* Clntinfo)
{
	Clnts_mutex.lock();
	Clnts.push_back(Clnt);
	ClntsInfo.push_back(Clntinfo);
	Clnts_mutex.unlock();
}

void TCP_SERVER::Remove_clnt(SOCKET Clnt)
{
	if (ClntsInfo.empty() && Clnts.empty())
		return;
	int index = -1;
	Clnts_mutex.lock();
	for(int i = 0; i < Clnts.size(); i++)
		if (Clnts[i] == Clnt)
		{
			index = i;
			break;
		}
	if (index != -1)
	{
		delete ClntsInfo[index];
		ClntsInfo[index] = NULL;
		this->End(Clnts[index]);
		Clnts[index] = NULL;

		for (int j = index; j < ClntsInfo.size() - 1; j++)
			ClntsInfo[j] = ClntsInfo[j + 1];
		ClntsInfo.pop_back();

		for (int j = index; j < Clnts.size() - 1; j++)
			Clnts[j] = Clnts[j + 1];
		Clnts.pop_back();
	}
	Clnts_mutex.unlock();
	return;
}

void TCP_SERVER::Clean_clnts(void)
{
	if (ClntsInfo.empty() && Clnts.empty())
		return;
	Clnts_mutex.lock();
	for (int i = 0; i < ClntsInfo.size(); i++)
		if (ClntsInfo[i] != NULL)
			delete  ClntsInfo[i];
	for (int i = 0; i < Clnts.size(); i++)
		if (Clnts[i] != NULL)
			this->End(Clnts[i]);
	Clnts_mutex.unlock();
}

int TCP_SERVER::Get_Clnts_Size(void)
{
	int size;
	Clnts_mutex.lock();
	size = Clnts.size();
	Clnts_mutex.unlock();
	return size;
}

bool TCP_SERVER::Print_Clnts_Infos(void)
{
	if (Clnts.empty())
		return false;
	Clnts_mutex.lock();
	for (int i = 0; i < Clnts.size(); i++)
		std::cout << i + 1 << " SOCKET : " << Clnts[i] << " ip : " << ClntsInfo[i]->sin_addr.S_un.S_addr 
			<< '\n';
	puts("\n");
	Clnts_mutex.unlock();
	return true;
}