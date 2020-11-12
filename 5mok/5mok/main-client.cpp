#include "Play.h"
#include "TCP.h"
#include "UI.h"

int main()
{
	UI ui;
	TCP tcp;
	Play main(ui, tcp);
	main.START();
	return 0;
}