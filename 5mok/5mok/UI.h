#pragma once
#include "BASE.h"
#include <iostream>
#include <Windows.h>
#include <vector>
using namespace std;

class UI
{
public:
	void SetColor(int forground, int background);
	void PrintStoneOfBoard(int stone);
	void PrintRoom_names(vector<char*> room_names, vector<char> room_board_size, vector<char*> room_nicknames);
	void ColorOneStone(char* board, int height, int x, int y);
	void ColorFiveStones(char* board, int height, 
		int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5);
	void ResultMessageForMulti(int result);
	void ResultMessageForSingle(int result);
	void gotoxy(short x, short y);
	void Clear(void);
	void PrintBoard(char* board, int length);
	void PressAnyKey(void);

	int Mainmenu(void);
	char* AskNickNameRetAV(void);
	int MakeRoomOrNot(void);
	bool AcceptTheOpponent(char* opponent_nickname);
	int AskWhichBoard(void);
	int AskWhichRoom(vector<char*> room_names, vector<char> room_board_size, vector<char*> room_nicknames);
	int* AskCoordinatesRetAV(void);
	char* AskRoom_nameRetAV(void);
};

