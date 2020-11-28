#include <iostream>
#include "SERVER.h"
#include "TCP_SERVER.h"

int main()
{
	TCP_SERVER tcp;
	SERVER MAIN(&tcp);
	MAIN.Run();

	return 0;
}