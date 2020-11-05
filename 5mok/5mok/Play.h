#pragma once
#include "UI.h"
class Play
{
public:
	Play(UI& ui) { main_UI = &ui; };
	void START(void);
	void SinglePptp(void);
	void MultiP(void);

private:
	UI* main_UI;
};

