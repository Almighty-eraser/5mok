#include "SERVER.h"

void SERVER::Run(void)
{
	serv_TCP->StartTCPserver(SERVER_PORT);


}
