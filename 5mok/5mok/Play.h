#pragma once
#include "BASE.h"

class UI;

class Play
{
public:
	Play(UI& ui) { main_UI = &ui; char* board = nullptr; };
	void START(void);
	int MakeBoard(void);
	void RemoveBoard(void);
	void SinglePptp(void);//singleplay person to person
	void MultiP(void);//multiplay
	int WhoseWinner(int stone, int height);

private:
	UI* main_UI;
	char* board;
};

