// Beep_Dlue.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Game.h"
#include <iostream>
#include "Time.h"

int main()
{
	srand(time(0));
	Game game(8, 8);
	game.start();
}