// Beep_Dlue.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Game.h"
#include <iostream>
#include "Time.h"

int main()
{
	srand(time(0));
	#ifdef DEBUG
		Game dgame(8, 8, true);
		dgame.start();
		return 0;
	#endif
	Game game;
	game.start();
}