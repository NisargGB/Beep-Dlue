#pragma once
#include "Board.h"
#include "Move.h"
#include <vector>
#include <string>

using namespace std;

class Game
{
public:
	int n, m, time_limit;
	int searchDepth;
	Board board;
	void start();
	Game();
	Game(int nin, int min, bool debug);
	pair<bool, bool> makeMove(Move move, Board *boardIn);
	string moveToString(Move move);
	Move stringToMove(pair<int, int> src, pair<int, int> tgt, char nature, bool polarity);
	// pair<vector<Move>,int> alphaBetaMinimax(Board b, int depth, bool isMaximizing, int alpha, int beta, vector<Move> selectedMoves);
	pair<vector<Move>,int> maxValue(Board b, int depth, int alpha, int beta, vector<Move> selectedMoves);
	pair<vector<Move>,int> minValue(Board b, int depth, int alpha, int beta, vector<Move> selectedMoves);
	void explainMoves(vector<Move> trail, int score, Board boardConfig);
};