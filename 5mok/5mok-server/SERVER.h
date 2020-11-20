#pragma once

#include <iostream>
#include <thread>
#include "BASE.h"
#include "TCP_SERVER.h"

class SERVER
{
public:
	SERVER(TCP_SERVER& tcp) { serv_TCP = &tcp; };
	void Run(void);
	void ShowRooms(SOCKET Clnt);
private:
	TCP_SERVER* serv_TCP;
};

