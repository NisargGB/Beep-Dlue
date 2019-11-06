#pragma once
#include "Board.h"
#include "Move.h"
#include <vector>
#include <string>

using namespace std;

class Game
{
public:
	int n, m;
	int searchDepth = 3;
	Board board;
	void start();
	Game(int nin, int min);
	pair<bool, bool> makeMove(Move move, Board *boardIn);
	string moveToString(Move move);
	Move stringToMove(pair<int, int> src, pair<int, int> tgt, char nature, bool polarity);
	pair<vector<Move>,int> alphaBetaMinimax(Board b, int depth, bool isMaximizing, int alpha, int beta, vector<Move> selectedMoves);
	void explainMoves(vector<Move> trail, int score, Board boardConfig);
};