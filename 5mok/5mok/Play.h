#pragma once
#include "BASE.h"
class Play
{
public:
	Play(UI& ui) { main_UI = &ui; };
	void START(void);
	int MakeBoard(void);
	void RemoveBoard(void);
	void SinglePptp(void);
	void MultiP(void);

private:
	UI* main_UI;
	char* board;
};

