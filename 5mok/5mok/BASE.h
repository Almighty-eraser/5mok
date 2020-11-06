#pragma once

#ifndef _COLOR_LIST_
#define _COLOR_LIST_

enum {
	black,
	blue,
	green,
	cyan,
	red,
	purple,
	brown,
	lightgray,
	darkgray,
	lightblue,
	lightgreen,
	lightcyan,
	lightred,
	lightpurple,
	yellow,
	white
};

#endif

#ifndef _STONES_
#define _STONES_

#define EMPTY 0
#define BLACK 1
#define WHITE 2

#endif

#ifndef _MAKE_ROOM_OR_NOT_
#define _MAKE_ROOM_OR_NOT_

#define _IMMA_MAKE_ROOM_ 0
#define _IMMA_JOIN_ROOM_ 1

#endif

//1. 8x8 2. 9x9 3. 10x10 4. 11x11
#ifndef _SIZE_OF_BOARD_
#define _SIZE_OF_BOARD_

#define _8x8 1
#define _9x9 2
#define _10x10 3
#define _11x11 4

#endif

#include <iostream>
#include <Windows.h>
#include "Play.h"
#include "UI.h"
#include "TCP.h"

using namespace std;

void Error(const char* message)
{
	cout << '\n' << message << '\n';
	exit(-1);
}