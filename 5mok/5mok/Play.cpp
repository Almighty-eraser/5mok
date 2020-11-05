#include "BASE.h"

void Play::START(void)
{
	int answer;
	main_UI->Mainmenu();
	cin >> answer;

	switch (answer)
	{
	case 1:
		SinglePptp();
		break;
	case 3:
		main_UI->MakeRoomOrNot();
		cin >> answer;
		if(answer == 1)

		break;
	default:
		break;
	}
}

void Play::SinglePptp(void)
{

}

void Play::MultiP(void)
{

}