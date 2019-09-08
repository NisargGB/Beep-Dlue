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
		

		//Making our move
		vector<Move> list = board.validMoves();			//List of all possible moves
		
		cerr << "Searching for optimal move\n";
		pair<Move, int> optimalStep = alphaBetaMinimax(board, 1, true, -100, 100);		//The optimal move bu alpha-bta search
		cerr << "Optimal --------------------------------- Value: " << optimalStep.second << " Move: " << moveToString(optimalStep.first) << endl;

		makeMove(optimalStep.first, &board);		//Make the optimal move
		cout << moveToString(optimalStep.first) << endl;
		board.printBoard();


		//Opponent is making the move now
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
	{
		cerr << "Utility value: " << b.utilityScore() << endl;
		return {Move({-1,-1}, {-1,-1}, 'x', true), b.utilityScore()};
	}

	if(depth == 0)
	{
		cerr << "End point " << endl;
		b.printBoard();
		cerr << "Heuristic value: " << b.heuristicScore() << endl << endl;
		return {Move({-1,-1}, {-1,-1}, 'x', true), b.heuristicScore()};
	}

	if(isMaximizing)
	{
		// cerr << "MAX node ----------- " << depth << endl << endl;
		int maxScore = -100;
		vector<Move> legalMoves = b.validMoves();
		int optimalMoveIndex = -1;
		for(int i=0 ; i<legalMoves.size() ; i++)
		{
			Board tempBoard = board;
			makeMove(legalMoves[i], &tempBoard);
			// cerr << "Testing move: " << moveToString(m) << endl;
			// tempBoard.printBoard();
			pair<Move,int> tempScore = alphaBetaMinimax(tempBoard, depth-1, false, alpha, beta);
			maxScore = max(maxScore, tempScore.second);
			if(maxScore == tempScore.second)
				optimalMoveIndex = i;
			alpha = max(alpha, maxScore);
			if(alpha >= beta)
				break;
		}

		return {legalMoves[optimalMoveIndex], maxScore};
	}
	else
	{
		// cerr << "MIN at : " << depth << endl;
		int minScore = 100;								//TODO set the max value bound
		vector<Move> legalMoves = b.validMoves();
		int optimalMoveIndex;
		for(int i=0 ; i<legalMoves.size() ; i++)
		{
			Board tempBoard = board;
			makeMove(legalMoves[i], &tempBoard);
			pair<Move,int> tempScore = alphaBetaMinimax(tempBoard, depth-1, true, alpha, beta);
			minScore = min(minScore, tempScore.second);
			if(minScore == tempScore.second)
				optimalMoveIndex = i;
			beta = min(beta, minScore);
			if(alpha >= beta)
				break;
		}
		return {legalMoves[optimalMoveIndex], minScore};
	}
}