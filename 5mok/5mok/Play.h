#pragma once
#include "UI.h"
#include "TCP.h"
#include <mutex>

class UI;

class TCP;

class Play
{
public:
	Play(UI& ui, TCP& tcp) 
	{ 
		main_UI = &ui; 
		main_TCP = &tcp; 
		board = NULL; 
		board_size = 0; 
		nickname = NULL; 
	};
	~Play() { 
		while (room_names.empty() != 1)
		{
			char* room_name = room_names[0];
			room_names.erase(room_names.begin());
			delete[] room_name;
		}
		while (room_nicknames.empty() != 1)
		{
			char* nickname = room_nicknames[0];
			room_nicknames.erase(room_nicknames.begin());
			delete[] nickname;
		}
	};
	void START(void);
	int MakeBoard(void);
	void MakeBoardForMulti(int _board_size);
	void RemoveBoard(void);
	void SinglePptp(void);//singleplay person to person
	void MakingRoom(void);
	void DeletingRoom(char* room_name);
	void JoiningRoom(void);
	void MultiP(int whichside);//multiplay
	int WhoseWinner(int stone, int height);

private:
	UI* main_UI;
	TCP* main_TCP;

	char* board;
	char board_size;
	char* nickname;
	vector<char*> room_names;
	vector<char> room_board_sizes;
	vector<char*> room_nicknames;
};

