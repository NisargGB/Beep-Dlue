#include "pch.h"
#include "Board.h"
#include "Game.h"
#include "Move.h"
#include <vector>
#include <iostream>
#include <string>
#include <tuple>
using namespace std;


Game::Game(int nin, int min)
{
	n = nin;
	m = min;
	Board newBoard(n, m);
	// Board newBoard(n);
	board = newBoard;
}

void Game::start()
{
	int player, nin, min, time;
	cin >> player >> nin >> min >> time;
	int plays = 0;

	while (true)
	{
		pair<int,int> pieceCount = board.countEnemies();
		if(pieceCount.second <= 8 || pieceCount.first <= 8)
			searchDepth = 5;
		if(pieceCount.second <= 5 || pieceCount.first <= 5)
			searchDepth = 6;
		if(pieceCount.second <= 3 || pieceCount.first <= 3)
			searchDepth = 6;
		
		if(player == 1)
		{
			char s, nature;
			pair<int, int> src, tgt;

			//Making our move
			// vector<Move> list = board.validMoves(true);			//List of all possible moves
			
			cerr << "Searching for optimal move\n";
			pair<Move, int> optimalStep = alphaBetaMinimax(board, searchDepth, true, -100000, 100000);		//The optimal move from alpha-bta search
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
		else
		{
			char s, nature;
			pair<int, int> src, tgt;

			//Opponent is making the move now
			cin >> s >> src.first >> src.second >> nature >> tgt.first >> tgt.second;
			// cerr << "This was read: " << s << " " << src.first << " " << src.second << " " << nature << " " << tgt.first << " " << tgt.second << endl;
			Move move = stringToMove(src, tgt, nature, true);
			// cerr << "Opponent's move: " << move.from.first << " " << move.from.second << " to " << move.to.first << " " << move.to.second << " " << move.type << endl; 
			makeMove(move, &board);
			board.printBoard();

			//Making our move
			// vector<Move> list = board.validMoves(false);			//List of all possible moves
			
			cerr << "Searching for optimal move\n";
			pair<Move, int> optimalStep = alphaBetaMinimax(board, searchDepth, false, -100000, 100000);		//The optimal move from alpha-bta search
			cerr << "Optimal --------------------------------- Value: " << optimalStep.second << " Move: " << moveToString(optimalStep.first) << endl;

			makeMove(optimalStep.first, &board);		//Make the optimal move
			cout << moveToString(optimalStep.first) << endl;
			board.printBoard();
			
		}
		
		plays++;
	}
}

// Plays the move and returns if the move was lethal
pair<bool, bool> Game::makeMove(Move move, Board* boardIn)
{
	bool isLethalToGoal = false;
	bool isLethalToTownhall = false;

	char myChar;
	char myGoal;
	char myTownhall;
	if (move.polarity)
	{
		myChar = 'S';
		myGoal = 'G';
		myTownhall = 'T';
	}
	else
	{
		myChar = 'E';
		myGoal = 'T';
		myTownhall = 'G';
	}
	if (move.type == 'f')		//firing cannon
	{
		if(boardIn->config[move.to.first][move.to.second] == myGoal)
		{
			isLethalToGoal = true;	
		}
		if(boardIn->config[move.to.first][move.to.second] == myTownhall)
		{
			isLethalToTownhall = true;
		}
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
		if(boardIn->config[move.to.first][move.to.second] == myGoal)
			isLethalToGoal = true;
		if(boardIn->config[move.to.first][move.to.second] == myTownhall)
			isLethalToTownhall = true;
		boardIn->config[move.from.first][move.from.second] = '-';
		boardIn->config[move.to.first][move.to.second] = myChar;
	}

	return {isLethalToTownhall, isLethalToGoal};
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
	// cerr << "I recieved this board\n";
	// b.printBoard();
	if(b.isTerminal(isMaximizing))
	{
		// cerr << "Utility value: " << b.heuristicScore() << " at depth: " << depth << endl;
		return {Move({-1,-1}, {-1,-1}, 'x', true), b.heuristicScore()};
	}

	if(depth == 0)
	{
		// cerr << "End point " << endl;
		// b.printBoard();
		// cerr << "Heuristic value: " << b.heuristicScore() << endl << endl;
		return {Move({-1,-1}, {-1,-1}, 'x', true), b.heuristicScore()};
	}

	if(isMaximizing)
	{
		// cerr << "MAX node ----------- " << depth << endl << endl;
		int maxScore = -100000;
		vector<Move> legalMoves = b.validMoves(true);
		// int optimalMoveIndex = -1;
		vector<Move> optimalMoveList;
		for(int i=0 ; i<legalMoves.size() ; i++)
		{
			// cerr << "************************---------------************ Max Trying move: " << moveToString(legalMoves[i]) << endl;
			Board tempBoard = b;
			pair<bool,bool> lethals;
			lethals = (makeMove(legalMoves[i], &tempBoard));
			bool isLethalToTownhall = lethals.first;
			bool isLethalToGoal = lethals.second;

			// tempBoard.printBoard();
			// if(depth >= searchDepth-1 && isLethalToGoal == true)
			// {
			// 	cerr << "///////////////////////// GOAL IN DANGER /////////////////////\n";
			// 	// pair<Move,int> anticipation = alphaBetaMinimax(tempBoard, 1, false, alpha, beta);
			// 	// if(tempBoard.config[anticipation.first.to.first][anticipation.first.to.second] != 'T')
			// 		return {legalMoves[i], tempBoard.heuristicScore()};
			// }
			pair<Move,int> tempScore = alphaBetaMinimax(tempBoard, depth-1, false, alpha, beta);
			
			if(tempScore.second > maxScore)
				optimalMoveList = {};
			maxScore = max(maxScore, tempScore.second);
			if(maxScore == tempScore.second)
				optimalMoveList.push_back(legalMoves[i]);
			alpha = max(alpha, maxScore);
			if(alpha >= beta)
				break;
		}

		return {optimalMoveList[rand() % optimalMoveList.size()], maxScore};
	}
	else
	{
		// cerr << "MIN at : " << depth << endl;
		int minScore = 100000;								//TODO set the max value bound
		vector<Move> legalMoves = b.validMoves(false);
		vector<Move> optimalMoveList;
		for(int i=0 ; i<legalMoves.size() ; i++)
		{
			// cerr << "************************---------------************ Min Trying move: " << moveToString(legalMoves[i]) << endl;
			Board tempBoard = b;
			pair<bool,bool> lethals;
			lethals = (makeMove(legalMoves[i], &tempBoard));
			bool isLethalToTownhall = lethals.first;
			bool isLethalToGoal = lethals.second;
			//makeMove(legalMoves[i], &tempBoard);

			// cerr << "Move at depth: " << depth << " is "
			// if(depth >= searchDepth-1 && isLethalToGoal == true)
			// {
			// 	cerr << "///////////////////////// TOWNHALL IN DANGER /////////////////////\n";
			// 	// pair<Move,int> anticipation = alphaBetaMinimax(tempBoard, 1, true, alpha, beta);
			// 	// if(tempBoard.config[anticipation.first.to.first][anticipation.first.to.second] != 'G')
			// 		return {legalMoves[i], tempBoard.heuristicScore()};
			// }
			pair<Move,int> tempScore = alphaBetaMinimax(tempBoard, depth-1, true, alpha, beta);
			if(tempScore.second < minScore)
				optimalMoveList = {};
			minScore = min(minScore, tempScore.second);
			if(minScore == tempScore.second)
				optimalMoveList.push_back(legalMoves[i]);
			beta = min(beta, minScore);
			if(alpha >= beta)
				break;
		}

		// cerr << "Min node will play: " << moveToString(legalMoves[optimalMoveIndex]) << endl;
		return {optimalMoveList[rand() % optimalMoveList.size()], minScore};
	}
}