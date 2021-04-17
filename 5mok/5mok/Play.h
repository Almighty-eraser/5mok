#pragma once
#include "UI.h"
#include "TCP.h"

class UI;

class TCP;

bool g_isThreadRunning = false;
bool g_isRoomDeleted = false;

class Play
{
public:
	Play(UI& ui, TCP& tcp) { main_UI = &ui; main_TCP = &tcp; board = nullptr; };
	~Play() { 
		if(!room_names.empty())
			for (int i = 0; i < room_names.size(); i++)
				if (room_names[i] != NULL)
					delete room_names[i];
			
	};
	void START(void);
	int MakeBoard(void);
	void RemoveBoard(void);
	void SinglePptp(void);//singleplay person to person
	void MakingRoom(void);
	void DeletingRoom(char* title);
	void JoiningRoom(void);
	int ReceiveRoomList(void);
	void MultiP(int whichside);//multiplay
	int WhoseWinner(int stone, int height);

private:
	UI* main_UI;
	TCP* main_TCP;

	char* board;
	vector<char*> room_names;
};

