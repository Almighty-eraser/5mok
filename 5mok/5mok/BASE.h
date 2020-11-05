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

#include <iostream>
#include <Windows.h>
#include "Play.h"
#include "UI.h"

using namespace std;

void Error(const char* message)
{
	cout << '\n' << message << '\n';
	exit(-1);
}