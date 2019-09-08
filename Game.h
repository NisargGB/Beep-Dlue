#pragma once
#include "pch.h"
#include "Board.h"
#include "Move.h"
#include <vector>
#include <string>

using namespace std;

class Game
{
public:
	int n, m;
	Board board;
	void start();
	Game(int nin, int min);
	void makeMove(Move move, Board *boardIn);
	string moveToString(Move move);
	Move stringToMove(pair<int, int> src, pair<int, int> tgt, char nature, bool polarity);
	pair<Move,int> alphaBetaMinimax(Board b, int depth, bool isMaximizing, int alpha, int beta);
};