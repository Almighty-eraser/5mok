#pragma once
#include "UI.h"
#include "TCP.h"

class UI;

class TCP;

class Play
{
public:
	Play(UI& ui, TCP& tcp) { main_UI = &ui; main_TCP = &tcp; board = nullptr; };
	void START(void);
	int MakeBoard(void);
	void RemoveBoard(void);
	void SinglePptp(void);//singleplay person to person
	void MakeRoomOrNot(char decision);
	void MultiP(void);//multiplay
	int WhoseWinner(int stone, int height);

private:
	UI* main_UI;
	TCP* main_TCP;
	int* board;
};

