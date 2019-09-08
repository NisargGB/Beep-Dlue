#include "pch.h"
#include "Board.h"
#include "Game.h"
#include "Move.h"
#include <vector>
#include <iostream>
#include <string>

using namespace std;


Game::Game(int nin, int min)
{
	n = nin;
	m = min;
	Board newBoard(n, m);
	board = newBoard;
}

void Game::start()
{
	while (true)
	{
		char s, nature;
		pair<int, int> src, tgt;
		string input;
		getline(cin,input);
		cerr << input << endl;
		
		vector<Move> list = board.validMoves();
		int index = rand() % (list.size());
		makeMove(list[index], &board);
		board.printBoard();
		cout << moveToString(list[index]) << endl;

		cin >> s >> src.first >> src.second >> nature >> tgt.first >> tgt.second;
		// cerr << "This was read: " << s << " " << src.first << " " << src.second << " " << nature << " " << tgt.first << " " << tgt.second << endl;
		Move move = stringToMove(src, tgt, nature, false);
		// cerr << "Opponent's move: " << move.from.first << " " << move.from.second << " to " << move.to.first << " " << move.to.second << " " << move.type << endl; 
		makeMove(move, &board);
		board.printBoard();
	}
}

void Game::makeMove(Move move, Board* boardIn)
{
	char myChar;
	if (move.polarity)
		myChar = 'S';
	else
		myChar = 'E';

	if (move.type == 'f')		//firing cannon
	{
		boardIn->config[move.to.first][move.to.second] = '-';
	}
	else if (move.type == 'c')	//moving cannon
	{
		int directiony = move.to.first - move.from.first;
		int directionx = move.to.second - move.from.second;
		boardIn->config[move.from.first - directionx][move.from.second - directiony] = '-';
		boardIn->config[move.to.first + directionx][move.to.second + directiony] = myChar;
	}
	else						//moving soldier
	{
		boardIn->config[move.from.first][move.from.second] = '-';
		boardIn->config[move.to.first][move.to.second] = myChar;
	}
}





string Game::moveToString(Move move)
{
	string ans;
	if (move.type == 'f')
	{
		ans = ans + "S " + to_string(move.from.second) + " " + to_string(move.from.first) + " B " + to_string(move.to.second) + " " + to_string(move.to.first);
	}
	else
	{
		ans = ans + "S " + to_string(move.from.second) + " " + to_string(move.from.first) + " M " + to_string(move.to.second) + " " + to_string(move.to.first);
	}
	return ans;
}

Move Game::stringToMove(pair<int,int> src, pair<int,int> tgt, char nature, bool polarity)
{
	char c;
	if (nature == 'B') c = 'f'; else c = 'a';
	pair<int,int> from = {src.second, src.first};
	pair<int,int> to = {tgt.second, tgt.first};
	Move newMove(from, to, c, polarity);

	return newMove;
}



pair<Move,int> Game::alphaBetaMinimax(Board b, int depth, bool isMaximizing, int alpha, int beta)
{
	if(b.isTerminal())
		return {NULL, b.utilityScore()};

	if(depth == 0)
		return {NULL, b.heuristicScore()};

	if(isMaximizing)
	{
		int max = -1;
		vector<Move> legalMoves = b.validMoves();
		Move optimalMove = NULL;
		for(Move m : legalMoves)
		{
			Board tempBoard = board;
			makeMove(m, tempBoard);
			pair<Move,int> tempScore = alphaBetaMinimax(tempBoard, depth-1, false, alpha, beta);
			max = max(max, tempScore.second);
			if(max == tempScore.second)
				optimalMove = m;
			alpha = max(alpha, max);
			if(alpha >= beta)
				break;
		}
		return {optimalMove, max};
	}
	else
	{
		int min = 100;								//TODO set the max value bound
		vector<Move> legalMoves = b.validMoves();
		Move optimalMove = NULL;
		for(Move m : legalMoves)
		{
			Board tempBoard = board;
			makeMove(m, tempBoard);
			pair<Move,int> tempScore = alphaBetaMinimax(tempBoard, depth-1, true, alpha, beta);
			min = min(max, tempScore.second);
			if(min == tempScore.second)
				optimalMove = m;
			beta = min(beta, min);
			if(alpha >= beta)
				break;
		}
		return {optimalMove, min};
	}
}