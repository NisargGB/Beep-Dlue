#pragma once
#include "Move.h"
#include <vector>

using namespace std;

class Board
{
public:
	int n, m;
	vector<vector<char>> config;
	Board(int nin, int min);
	Board(int nin, int min, bool debug);
	Board();
	Board(vector<vector<char>> initBoard);
	vector<Move> validMoves(bool side);
	void reverseBoard();
	bool positionIsValid(pair<int, int> pos);
	bool checkEnemy(pair<int, int> targetPos);
	bool checkSoldier(pair<int, int> targetPos);
	bool checkTownhall(pair<int, int> targetPos);
	bool checkGoal(pair<int, int> targetPos);
	bool checkEmpty(pair<int,int> targetPos);
	bool checkSoldierOrTownhall(pair<int, int> targetPos);
	bool checkEnemyOrGoal(pair<int, int> targetPos);
	void printBoard();
	pair<int,int> countEnemies();
	bool isTerminal(bool side);
	int utilityScore();
	int heuristicScore();
	tuple<int, int, int, int, int, int, int, int, int, int, int> cannonsAndSoldierAttacks(bool side);
};