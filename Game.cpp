#include "Board.h"
#include "Game.h"
#include "Move.h"
#include <vector>
#include <iostream>
#include <string>
#include <tuple>
#include <time.h>
#include <chrono>
using namespace std;


Game::Game()
{
	
}

Game::Game(int nin, int min, bool debug)
{
	n = nin;
	m = min;
	// Board newBoard(n);
	if(debug){
		vector<vector<char>> initBoard(n);
		for(int i = 0; i < n; i++){
			vector<char> row(m);
			for(int j = 0; j < m; j++){
				cin >> row[j];				
			}
			initBoard[i] = row;
		}
		Board newBoard(initBoard);
		board = newBoard;
	}
	else{
		Board newBoard(n, m);
		board = newBoard;
	}
}

void Game::start()
{
	// initializing the game and its parameters
	int player, nin, min, timein;
	double time_left;
	cin >> player >> nin >> min >> timein;
	cerr << "n=" << nin << " m=" << min << endl;
	n = nin;
	m = min;
	time_limit = timein;
	time_left = time_limit;
	Board newBoard(n, m, true);
	board = newBoard;
	int plays = 0;

	while (true)
	{
		pair<int,int> pieceCount = board.countEnemies();
		if(player == 1)	searchDepth = 3; else searchDepth = 4;
		if(m != 10)
		{	
			if((pieceCount.second <= 8 || pieceCount.first <= 8) && (pieceCount.first - pieceCount.second <= 3))
			{
				if(player == 1)	searchDepth = 5; else searchDepth = 4;
			}
			if((pieceCount.second <= 5 || pieceCount.first <= 5) && (pieceCount.first - pieceCount.second <= 4))// 5 earlier
			{
				if(player == 1)	searchDepth = 5; else searchDepth = 6;
			}
			if((pieceCount.second <= 3 || pieceCount.first <= 3) && (pieceCount.first - pieceCount.second <= 4))
			{
				if(player == 1)	searchDepth = 5; else searchDepth = 6;
			}
		}
		else
		{
			if((pieceCount.second <= 10 || pieceCount.first <= 10) && (pieceCount.first - pieceCount.second <= 4))
			{
				if(player == 1)	searchDepth = 5; else searchDepth = 4;
			}
			if((pieceCount.second <= 5 || pieceCount.first <= 5) && (pieceCount.first - pieceCount.second <= 4))// 5 earlier
			{
				if(player == 1)	searchDepth = 5; else searchDepth = 6;
			}
			if((pieceCount.second <= 3 || pieceCount.first <= 3) && (pieceCount.first - pieceCount.second <= 4))
			{
				if(player == 1)	searchDepth = 5; else searchDepth = 6;
			}
		}
		if(player == 1)
		{
			char s, nature;
			pair<int, int> src, tgt;
			time_t start, end;

			// checking for approaching timeout
			if(time_left < 16)
			{
				searchDepth = 3;
			}

			//Making our move
			// cerr << "Searching for optimal move\n";
			
			time(&start);
			pair<vector<Move>, int> optimalStep = maxValue(board, searchDepth, -100000, 100000, {});		//The optimal move from alpha-bta search
			// cerr << "Optimal --------------------------------- Value: " << optimalStep.second << " Move: " << moveToString(optimalStep.first) << endl;

			explainMoves(optimalStep.first, optimalStep.second, board);
			makeMove(optimalStep.first[0], &board);		//Make the optimal move
			cout << moveToString(optimalStep.first[0]) << endl;			
			
			board.printBoard();
			time(&end);
			
			time_left -= double(end - start);

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
			time_t start, end;

			// checking for approaching timeout
			if(time_left < 45 && n==10 && m==10)
			{
				searchDepth = 4;
			}
			if(time_left < 16)
			{
				if(searchDepth==4) searchDepth = 2; else searchDepth = 4;
			}

			//Opponent is making the move now
			cin >> s >> src.first >> src.second >> nature >> tgt.first >> tgt.second;
			// cerr << "This was read: " << s << " " << src.first << " " << src.second << " " << nature << " " << tgt.first << " " << tgt.second << endl;
			Move move = stringToMove(src, tgt, nature, true);
			// cerr << "Opponent's move: " << move.from.first << " " << move.from.second << " to " << move.to.first << " " << move.to.second << " " << move.type << endl; 
			makeMove(move, &board);
			board.printBoard();

			//Making our move			
			// cerr << "Searching for optimal move\n";
			time(&start);
			pair<vector<Move>, int> optimalStep = minValue(board, searchDepth, -100000, 100000, {});		//The optimal move from alpha-bta search
			// cerr << "Optimal --------------------------------- Value: " << optimalStep.second << " Move: " << moveToString(optimalStep.first) << endl;

			explainMoves(optimalStep.first, optimalStep.second, board);
			makeMove(optimalStep.first[0], &board);		//Make the optimal move
			cout << moveToString(optimalStep.first[0]) << endl;
			
			board.printBoard();
			time(&end);
			time_left -= double(end - start);
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

pair<vector<Move>,int> Game::maxValue(Board b, int depth, int alpha, int beta, vector<Move> selectedMoves)
{
	if(b.isTerminal(true))
	{
		return {selectedMoves, b.heuristicScore()};
	}

	if(depth == 0)
	{
		return {selectedMoves, b.heuristicScore()};
	}
	int maxScore = -100000;
	vector<Move> legalMoves = b.validMoves(true);
	// int optimalMoveIndex = -1;
	vector<vector<Move>> optimalMoveList;
	for(int i=0 ; i<legalMoves.size() ; i++)
	{
		// cerr << "************************---------------************ Max Trying move: " << moveToString(legalMoves[i]) << endl;
		Board tempBoard = b;
		vector<Move> tempSelectedMoves = selectedMoves;	// the list to be passed on initialized
		pair<bool,bool> lethals;
		
		lethals = (makeMove(legalMoves[i], &tempBoard));
		tempSelectedMoves.push_back(legalMoves[i]);	// the list of selected moves updated and ready to be passed on
		
		// bool isLethalToTownhall = lethals.first;
		bool isLethalToGoal = lethals.second;

		if(depth >= searchDepth-1 && isLethalToGoal == true)
		{
			int goalsLeft = 0;
			for(int j = 0; j < m; j++)
			{
				if(tempBoard.config[0][j] == 'G')
				{
					goalsLeft++;
				}
			}
			if(goalsLeft == m/2 - 2){
				cerr << "///////////////////////// GOAL IN DANGER /////////////////////\n";
				return {{legalMoves[i]}, tempBoard.heuristicScore()};
			}
		}

		pair<vector<Move>,int> tempScore = minValue(tempBoard, depth-1, alpha, beta, tempSelectedMoves);
		
		if(tempScore.second > maxScore)
			optimalMoveList = {};
		maxScore = max(maxScore, tempScore.second);
		if(maxScore == tempScore.second)
			optimalMoveList.push_back(tempScore.first);
		alpha = max(alpha, maxScore);
		if(alpha > beta){
			return {optimalMoveList[rand() % optimalMoveList.size()], maxScore};
		}
	}
	// returning a randomly selected move from the list of optimal moves appended to the end of already selected moves
	return {optimalMoveList[rand() % optimalMoveList.size()], maxScore};	

}

pair<vector<Move>,int> Game::minValue(Board b, int depth, int alpha, int beta, vector<Move> selectedMoves)
{
	if(b.isTerminal(false))
	{
		return {selectedMoves, b.heuristicScore()};
	}

	if(depth == 0)
	{
		return {selectedMoves, b.heuristicScore()};
	}
	int minScore = 100000;
	vector<Move> legalMoves = b.validMoves(false);
	vector<vector<Move>> optimalMoveList;
	for(int i=0 ; i<legalMoves.size() ; i++)
	{
		// cerr << "************************---------------************ Min Trying move: " << moveToString(legalMoves[i]) << endl;
		Board tempBoard = b;
		vector<Move> tempSelectedMoves = selectedMoves;	// the list to be passed on initialized
		pair<bool,bool> lethals;
		
		lethals = (makeMove(legalMoves[i], &tempBoard));
		tempSelectedMoves.push_back(legalMoves[i]);	// the list of selected moves updated and ready to be passed on

		//bool isLethalToTownhall = lethals.first;
		bool isLethalToGoal = lethals.second;
		if(depth >= searchDepth-1 && isLethalToGoal == true)
		{
			int goalsLeft = 0;
			for(int j = 0; j < m; j++)
			{
				if(tempBoard.config[n-1][j] == 'T')
				{
					goalsLeft++;
				}
			}
			if(goalsLeft == m/2 - 2){
				cerr << "///////////////////////// TOWNHALL IN DANGER /////////////////////\n";
				return {{legalMoves[i]}, tempBoard.heuristicScore()};
			}
		}
		pair<vector<Move>,int> tempScore = maxValue(tempBoard, depth-1, alpha, beta, tempSelectedMoves);
		if(tempScore.second < minScore)
			optimalMoveList = {};
		minScore = min(minScore, tempScore.second);
		if(minScore == tempScore.second)
			optimalMoveList.push_back(tempScore.first);
		beta = min(beta, minScore);
		if(alpha > beta){
			return {optimalMoveList[rand() % optimalMoveList.size()], minScore};
		}
	}
	// returning a randomly selected move from the list of optimal moves appended to the end of already selected moves
	return {optimalMoveList[rand() % optimalMoveList.size()], minScore};
}


// pair<vector<Move>,int> Game::alphaBetaMinimax(Board b, int depth, bool isMaximizing, int alpha, int beta, vector<Move> selectedMoves)
// {
// 	// cerr << "I recieved this board\n";
// 	// b.printBoard();
// 	if(b.isTerminal(isMaximizing))
// 	{
// 		// cerr << "Utility value: " << b.heuristicScore() << " at depth: " << depth << endl;
// 		return {selectedMoves, b.heuristicScore()};
// 	}

// 	if(depth == 0)
// 	{
// 		// cerr << "End point " << endl;
// 		// b.printBoard();
// 		// cerr << "Heuristic value: " << b.heuristicScore() << endl << endl;
// 		return {selectedMoves, b.heuristicScore()};
// 	}

// 	if(isMaximizing)
// 	{
// 		// cerr << "MAX node ----------- " << depth << endl << endl;
// 		int maxScore = -100000;
// 		vector<Move> legalMoves = b.validMoves(true);
// 		// int optimalMoveIndex = -1;
// 		vector<vector<Move>> optimalMoveList;
// 		for(int i=0 ; i<legalMoves.size() ; i++)
// 		{
// 			// cerr << "************************---------------************ Max Trying move: " << moveToString(legalMoves[i]) << endl;
// 			Board tempBoard = b;
// 			vector<Move> tempSelectedMoves = selectedMoves;	// the list to be passed on initialized
// 			pair<bool,bool> lethals;
			
// 			lethals = (makeMove(legalMoves[i], &tempBoard));
// 			tempSelectedMoves.push_back(legalMoves[i]);	// the list of selected moves updated and ready to be passed on
			
// 			bool isLethalToTownhall = lethals.first;
// 			bool isLethalToGoal = lethals.second;

// 			// tempBoard.printBoard();
// 			// if(depth >= searchDepth-1 && isLethalToGoal == true)
// 			// {
// 			// 	cerr << "///////////////////////// GOAL IN DANGER /////////////////////\n";
// 			// 	// pair<Move,int> anticipation = alphaBetaMinimax(tempBoard, 1, false, alpha, beta);
// 			// 	// if(tempBoard.config[anticipation.first.to.first][anticipation.first.to.second] != 'T')
// 			// 		return {legalMoves[i], tempBoard.heuristicScore()};
// 			// }
// 			pair<vector<Move>,int> tempScore = alphaBetaMinimax(tempBoard, depth-1, false, alpha, beta, tempSelectedMoves);
			
// 			if(tempScore.second > maxScore)
// 				optimalMoveList = {};
// 			maxScore = max(maxScore, tempScore.second);
// 			if(maxScore == tempScore.second)
// 				optimalMoveList.push_back(tempScore.first);
// 			alpha = max(alpha, maxScore);
// 			if(alpha >= beta){
// 				return {optimalMoveList[rand() % optimalMoveList.size()], maxScore};
// 				break;
// 			}
// 		}

// 		// returning a randomly selected move from the list of optimal moves appended to the end of already selected moves
// 		return {optimalMoveList[rand() % optimalMoveList.size()], maxScore};
// 	}
// 	else
// 	{
// 		// cerr << "MIN at : " << depth << endl;
// 		int minScore = 100000;
// 		vector<Move> legalMoves = b.validMoves(false);
// 		vector<vector<Move>> optimalMoveList;
// 		for(int i=0 ; i<legalMoves.size() ; i++)
// 		{
// 			// cerr << "************************---------------************ Min Trying move: " << moveToString(legalMoves[i]) << endl;
// 			Board tempBoard = b;
// 			vector<Move> tempSelectedMoves = selectedMoves;	// the list to be passed on initialized
// 			pair<bool,bool> lethals;
			
// 			lethals = (makeMove(legalMoves[i], &tempBoard));
// 			tempSelectedMoves.push_back(legalMoves[i]);	// the list of selected moves updated and ready to be passed on

// 			bool isLethalToTownhall = lethals.first;
// 			bool isLethalToGoal = lethals.second;
// 			//makeMove(legalMoves[i], &tempBoard);

// 			// cerr << "Move at depth: " << depth << " is "
// 			// if(depth >= searchDepth-1 && isLethalToGoal == true)
// 			// {
// 			// 	cerr << "///////////////////////// TOWNHALL IN DANGER /////////////////////\n";
// 			// 	// pair<Move,int> anticipation = alphaBetaMinimax(tempBoard, 1, true, alpha, beta);
// 			// 	// if(tempBoard.config[anticipation.first.to.first][anticipation.first.to.second] != 'G')
// 			// 		return {legalMoves[i], tempBoard.heuristicScore()};
// 			// }
// 			pair<vector<Move>,int> tempScore = alphaBetaMinimax(tempBoard, depth-1, true, alpha, beta, tempSelectedMoves);
// 			if(tempScore.second < minScore)
// 				optimalMoveList = {};
// 			minScore = min(minScore, tempScore.second);
// 			if(minScore == tempScore.second)
// 				optimalMoveList.push_back(tempScore.first);
// 			beta = min(beta, minScore);
// 			if(alpha >= beta){
// 				return {optimalMoveList[rand() % optimalMoveList.size()], minScore};
// 				break;
// 			}
// 		}

// 		// cerr << "Min node will play: " << moveToString(legalMoves[optimalMoveIndex]) << endl;

// 		// returning a randomly selected move from the list of optimal moves appended to the end of already selected moves
// 		return {optimalMoveList[rand() % optimalMoveList.size()], minScore};
// 	}
// }


void Game::explainMoves(vector<Move> trail, int score, Board boardConfig)
{
	cerr << "The trail of moves predicted by bot is-\n";
	bool isOpponent = false;
	Board tempBoard = boardConfig;

	for(Move m : trail)
	{
		if(isOpponent)
			cerr << "Opponent : ";
		else
			cerr << "Beep-Dlue : ";
		cerr << moveToString(m) << '\n';
		makeMove(m, &tempBoard);
		tempBoard.printBoard();
		isOpponent = !isOpponent;
	}
	
	cerr << "The utility score predicted : " << score << "\n\n";
}
// #include "Board.h"
// #include "Game.h"
// #include "Move.h"
// #include <vector>
// #include <iostream>
// #include <string>
// #include <tuple>
// #include <time.h>
// using namespace std;


// Game::Game(int nin, int min)
// {
// 	n = nin;
// 	m = min;
// 	Board newBoard(n, m);
// 	// Board newBoard(n);
// 	board = newBoard;
// }

// Game::Game(int nin, int min, bool debug)
// {
// 	n = nin;
// 	m = min;
// 	// Board newBoard(n);
// 	if(debug){
// 		vector<vector<char>> initBoard(n);
// 		for(int i = 0; i < n; i++){
// 			vector<char> row(m);
// 			for(int j = 0; j < m; j++){
// 				cin >> row[j];				
// 			}
// 			initBoard[i] = row;
// 		}
// 		Board newBoard(initBoard);
// 		board = newBoard;
// 	}
// 	else{
// 		Board newBoard(n, m);
// 		board = newBoard;
// 	}
// }

// void Game::start()
// {
// 	int player, nin, min, time;
// 	cin >> player >> nin >> min >> time;
// 	cerr << "value of n is: " << nin << endl;
// 	int plays = 0;


// 	while (true)
// 	{
// 		pair<int,int> pieceCount = board.countEnemies();
// 		if(player == 1)	searchDepth = 3; else searchDepth = 3;
// 		if((pieceCount.second <= 8 || pieceCount.first <= 8) && (pieceCount.first - pieceCount.second <= 3))
// 		{
// 			if(player == 1)	searchDepth = 5; else searchDepth = 4;
// 		}
// 		if((pieceCount.second <= 5 || pieceCount.first <= 5) && (pieceCount.first - pieceCount.second <= 4))// 5 earlier
// 		{
// 			if(player == 1)	searchDepth = 5; else searchDepth = 6;
// 		}
// 		if((pieceCount.second <= 3 || pieceCount.first <= 3) && (pieceCount.first - pieceCount.second <= 4))
// 		{
// 			if(player == 1)	searchDepth = 5; else searchDepth = 6;
// 		}
		
// 		if(player == 1)
// 		{
// 			char s, nature;
// 			pair<int, int> src, tgt;

// 			//Making our move
// 			// vector<Move> list = board.validMoves(true);			//List of all possible moves
			
// 			// cerr << "Searching for optimal move\n";
// 			pair<vector<Move>, int> optimalStep = maxValue(board, searchDepth, -100000, 100000, {});		//The optimal move from alpha-bta search
// 			// cerr << "Optimal --------------------------------- Value: " << optimalStep.second << " Move: " << moveToString(optimalStep.first) << endl;

// 			explainMoves(optimalStep.first, optimalStep.second, board);
// 			makeMove(optimalStep.first[0], &board);		//Make the optimal move
// 			cout << moveToString(optimalStep.first[0]) << endl;
			
// 			board.printBoard();


// 			//Opponent is making the move now
// 			cin >> s >> src.first >> src.second >> nature >> tgt.first >> tgt.second;
// 			// cerr << "This was read: " << s << " " << src.first << " " << src.second << " " << nature << " " << tgt.first << " " << tgt.second << endl;
// 			Move move = stringToMove(src, tgt, nature, false);
// 			// cerr << "Opponent's move: " << move.from.first << " " << move.from.second << " to " << move.to.first << " " << move.to.second << " " << move.type << endl; 
// 			makeMove(move, &board);
// 			board.printBoard();
// 		}
// 		else
// 		{
// 			char s, nature;
// 			pair<int, int> src, tgt;

// 			//Opponent is making the move now
// 			cin >> s >> src.first >> src.second >> nature >> tgt.first >> tgt.second;
// 			// cerr << "This was read: " << s << " " << src.first << " " << src.second << " " << nature << " " << tgt.first << " " << tgt.second << endl;
// 			Move move = stringToMove(src, tgt, nature, true);
// 			// cerr << "Opponent's move: " << move.from.first << " " << move.from.second << " to " << move.to.first << " " << move.to.second << " " << move.type << endl; 
// 			makeMove(move, &board);
// 			board.printBoard();

// 			//Making our move
// 			// vector<Move> list = board.validMoves(false);			//List of all possible moves
			
// 			// cerr << "Searching for optimal move\n";
// 			pair<vector<Move>, int> optimalStep = minValue(board, searchDepth, -100000, 100000, {});		//The optimal move from alpha-bta search
// 			// cerr << "Optimal --------------------------------- Value: " << optimalStep.second << " Move: " << moveToString(optimalStep.first) << endl;

// 			explainMoves(optimalStep.first, optimalStep.second, board);
// 			makeMove(optimalStep.first[0], &board);		//Make the optimal move
// 			cout << moveToString(optimalStep.first[0]) << endl;
			
// 			board.printBoard();
			
// 		}
		
// 		plays++;
// 	}
// }

// // Plays the move and returns if the move was lethal
// pair<bool, bool> Game::makeMove(Move move, Board* boardIn)
// {
// 	bool isLethalToGoal = false;
// 	bool isLethalToTownhall = false;

// 	char myChar;
// 	char myGoal;
// 	char myTownhall;
// 	if (move.polarity)
// 	{
// 		myChar = 'S';
// 		myGoal = 'G';
// 		myTownhall = 'T';
// 	}
// 	else
// 	{
// 		myChar = 'E';
// 		myGoal = 'T';
// 		myTownhall = 'G';
// 	}
// 	if (move.type == 'f')		//firing cannon
// 	{
// 		if(boardIn->config[move.to.first][move.to.second] == myGoal)
// 		{
// 			isLethalToGoal = true;	
// 		}
// 		if(boardIn->config[move.to.first][move.to.second] == myTownhall)
// 		{
// 			isLethalToTownhall = true;
// 		}
// 		boardIn->config[move.to.first][move.to.second] = '-';
// 	}
// 	else if (move.type == 'c')	//moving cannon
// 	{
// 		int directiony = move.to.first - move.from.first;
// 		int directionx = move.to.second - move.from.second;
// 		boardIn->config[move.from.first - directionx][move.from.second - directiony] = '-';
// 		boardIn->config[move.to.first + directionx][move.to.second + directiony] = myChar;
// 	}
// 	else						//moving soldier
// 	{
// 		if(boardIn->config[move.to.first][move.to.second] == myGoal)
// 			isLethalToGoal = true;
// 		if(boardIn->config[move.to.first][move.to.second] == myTownhall)
// 			isLethalToTownhall = true;
// 		boardIn->config[move.from.first][move.from.second] = '-';
// 		boardIn->config[move.to.first][move.to.second] = myChar;
// 	}

// 	return {isLethalToTownhall, isLethalToGoal};
// }



// string Game::moveToString(Move move)
// {
// 	string ans;
// 	if (move.type == 'f')
// 	{
// 		ans = ans + "S " + to_string(move.from.second) + " " + to_string(move.from.first) + " B " + to_string(move.to.second) + " " + to_string(move.to.first);
// 	}
// 	else
// 	{
// 		ans = ans + "S " + to_string(move.from.second) + " " + to_string(move.from.first) + " M " + to_string(move.to.second) + " " + to_string(move.to.first);
// 	}
// 	return ans;
// }

// Move Game::stringToMove(pair<int,int> src, pair<int,int> tgt, char nature, bool polarity)
// {
// 	char c;
// 	if (nature == 'B') c = 'f'; else c = 'a';
// 	pair<int,int> from = {src.second, src.first};
// 	pair<int,int> to = {tgt.second, tgt.first};
// 	Move newMove(from, to, c, polarity);

// 	return newMove;
// }

// pair<vector<Move>,int> Game::maxValue(Board b, int depth, int alpha, int beta, vector<Move> selectedMoves)
// {
// 	if(b.isTerminal(true))
// 	{
// 		return {selectedMoves, b.heuristicScore()};
// 	}

// 	if(depth == 0)
// 	{
// 		return {selectedMoves, b.heuristicScore()};
// 	}
// 		int maxScore = -100000;
// 		vector<Move> legalMoves = b.validMoves(true);
// 		// int optimalMoveIndex = -1;
// 		vector<vector<Move>> optimalMoveList;
// 		for(int i=0 ; i<legalMoves.size() ; i++)
// 		{
// 			// cerr << "************************---------------************ Max Trying move: " << moveToString(legalMoves[i]) << endl;
// 			Board tempBoard = b;
// 			vector<Move> tempSelectedMoves = selectedMoves;	// the list to be passed on initialized
// 			pair<bool,bool> lethals;
			
// 			lethals = (makeMove(legalMoves[i], &tempBoard));
// 			tempSelectedMoves.push_back(legalMoves[i]);	// the list of selected moves updated and ready to be passed on
			
// 			bool isLethalToTownhall = lethals.first;
// 			bool isLethalToGoal = lethals.second;

// 			pair<vector<Move>,int> tempScore = minValue(tempBoard, depth-1, alpha, beta, tempSelectedMoves);
			
// 			if(tempScore.second > maxScore)
// 				optimalMoveList = {};
// 			maxScore = max(maxScore, tempScore.second);
// 			if(maxScore == tempScore.second)
// 				optimalMoveList.push_back(tempScore.first);
// 			alpha = max(alpha, maxScore);
// 			if(alpha > beta){
// 				return {optimalMoveList[rand() % optimalMoveList.size()], maxScore};
// 			}
// 		}
// 		// returning a randomly selected move from the list of optimal moves appended to the end of already selected moves
// 		return {optimalMoveList[rand() % optimalMoveList.size()], maxScore};	

// }

// pair<vector<Move>,int> Game::minValue(Board b, int depth, int alpha, int beta, vector<Move> selectedMoves)
// {
// 	if(b.isTerminal(false))
// 	{
// 		return {selectedMoves, b.heuristicScore()};
// 	}

// 	if(depth == 0)
// 	{
// 		return {selectedMoves, b.heuristicScore()};
// 	}
// 	int minScore = 100000;
// 	vector<Move> legalMoves = b.validMoves(false);
// 	vector<vector<Move>> optimalMoveList;
// 	for(int i=0 ; i<legalMoves.size() ; i++)
// 	{
// 		// cerr << "************************---------------************ Min Trying move: " << moveToString(legalMoves[i]) << endl;
// 		Board tempBoard = b;
// 		vector<Move> tempSelectedMoves = selectedMoves;	// the list to be passed on initialized
// 		pair<bool,bool> lethals;
		
// 		lethals = (makeMove(legalMoves[i], &tempBoard));
// 		tempSelectedMoves.push_back(legalMoves[i]);	// the list of selected moves updated and ready to be passed on

// 		bool isLethalToTownhall = lethals.first;
// 		bool isLethalToGoal = lethals.second;
		
// 		pair<vector<Move>,int> tempScore = maxValue(tempBoard, depth-1, alpha, beta, tempSelectedMoves);
// 		if(tempScore.second < minScore)
// 			optimalMoveList = {};
// 		minScore = min(minScore, tempScore.second);
// 		if(minScore == tempScore.second)
// 			optimalMoveList.push_back(tempScore.first);
// 		beta = min(beta, minScore);
// 		if(alpha > beta){
// 			return {optimalMoveList[rand() % optimalMoveList.size()], minScore};
// 		}
// 	}
// 	// returning a randomly selected move from the list of optimal moves appended to the end of already selected moves
// 	return {optimalMoveList[rand() % optimalMoveList.size()], minScore};
// }


// // pair<vector<Move>,int> Game::alphaBetaMinimax(Board b, int depth, bool isMaximizing, int alpha, int beta, vector<Move> selectedMoves)
// // {
// // 	// cerr << "I recieved this board\n";
// // 	// b.printBoard();
// // 	if(b.isTerminal(isMaximizing))
// // 	{
// // 		// cerr << "Utility value: " << b.heuristicScore() << " at depth: " << depth << endl;
// // 		return {selectedMoves, b.heuristicScore()};
// // 	}

// // 	if(depth == 0)
// // 	{
// // 		// cerr << "End point " << endl;
// // 		// b.printBoard();
// // 		// cerr << "Heuristic value: " << b.heuristicScore() << endl << endl;
// // 		return {selectedMoves, b.heuristicScore()};
// // 	}

// // 	if(isMaximizing)
// // 	{
// // 		// cerr << "MAX node ----------- " << depth << endl << endl;
// // 		int maxScore = -100000;
// // 		vector<Move> legalMoves = b.validMoves(true);
// // 		// int optimalMoveIndex = -1;
// // 		vector<vector<Move>> optimalMoveList;
// // 		for(int i=0 ; i<legalMoves.size() ; i++)
// // 		{
// // 			// cerr << "************************---------------************ Max Trying move: " << moveToString(legalMoves[i]) << endl;
// // 			Board tempBoard = b;
// // 			vector<Move> tempSelectedMoves = selectedMoves;	// the list to be passed on initialized
// // 			pair<bool,bool> lethals;
			
// // 			lethals = (makeMove(legalMoves[i], &tempBoard));
// // 			tempSelectedMoves.push_back(legalMoves[i]);	// the list of selected moves updated and ready to be passed on
			
// // 			bool isLethalToTownhall = lethals.first;
// // 			bool isLethalToGoal = lethals.second;

// // 			// tempBoard.printBoard();
// // 			// if(depth >= searchDepth-1 && isLethalToGoal == true)
// // 			// {
// // 			// 	cerr << "///////////////////////// GOAL IN DANGER /////////////////////\n";
// // 			// 	// pair<Move,int> anticipation = alphaBetaMinimax(tempBoard, 1, false, alpha, beta);
// // 			// 	// if(tempBoard.config[anticipation.first.to.first][anticipation.first.to.second] != 'T')
// // 			// 		return {legalMoves[i], tempBoard.heuristicScore()};
// // 			// }
// // 			pair<vector<Move>,int> tempScore = alphaBetaMinimax(tempBoard, depth-1, false, alpha, beta, tempSelectedMoves);
			
// // 			if(tempScore.second > maxScore)
// // 				optimalMoveList = {};
// // 			maxScore = max(maxScore, tempScore.second);
// // 			if(maxScore == tempScore.second)
// // 				optimalMoveList.push_back(tempScore.first);
// // 			alpha = max(alpha, maxScore);
// // 			if(alpha >= beta){
// // 				return {optimalMoveList[rand() % optimalMoveList.size()], maxScore};
// // 				break;
// // 			}
// // 		}

// // 		// returning a randomly selected move from the list of optimal moves appended to the end of already selected moves
// // 		return {optimalMoveList[rand() % optimalMoveList.size()], maxScore};
// // 	}
// // 	else
// // 	{
// // 		// cerr << "MIN at : " << depth << endl;
// // 		int minScore = 100000;
// // 		vector<Move> legalMoves = b.validMoves(false);
// // 		vector<vector<Move>> optimalMoveList;
// // 		for(int i=0 ; i<legalMoves.size() ; i++)
// // 		{
// // 			// cerr << "************************---------------************ Min Trying move: " << moveToString(legalMoves[i]) << endl;
// // 			Board tempBoard = b;
// // 			vector<Move> tempSelectedMoves = selectedMoves;	// the list to be passed on initialized
// // 			pair<bool,bool> lethals;
			
// // 			lethals = (makeMove(legalMoves[i], &tempBoard));
// // 			tempSelectedMoves.push_back(legalMoves[i]);	// the list of selected moves updated and ready to be passed on

// // 			bool isLethalToTownhall = lethals.first;
// // 			bool isLethalToGoal = lethals.second;
// // 			//makeMove(legalMoves[i], &tempBoard);

// // 			// cerr << "Move at depth: " << depth << " is "
// // 			// if(depth >= searchDepth-1 && isLethalToGoal == true)
// // 			// {
// // 			// 	cerr << "///////////////////////// TOWNHALL IN DANGER /////////////////////\n";
// // 			// 	// pair<Move,int> anticipation = alphaBetaMinimax(tempBoard, 1, true, alpha, beta);
// // 			// 	// if(tempBoard.config[anticipation.first.to.first][anticipation.first.to.second] != 'G')
// // 			// 		return {legalMoves[i], tempBoard.heuristicScore()};
// // 			// }
// // 			pair<vector<Move>,int> tempScore = alphaBetaMinimax(tempBoard, depth-1, true, alpha, beta, tempSelectedMoves);
// // 			if(tempScore.second < minScore)
// // 				optimalMoveList = {};
// // 			minScore = min(minScore, tempScore.second);
// // 			if(minScore == tempScore.second)
// // 				optimalMoveList.push_back(tempScore.first);
// // 			beta = min(beta, minScore);
// // 			if(alpha >= beta){
// // 				return {optimalMoveList[rand() % optimalMoveList.size()], minScore};
// // 				break;
// // 			}
// // 		}

// // 		// cerr << "Min node will play: " << moveToString(legalMoves[optimalMoveIndex]) << endl;

// // 		// returning a randomly selected move from the list of optimal moves appended to the end of already selected moves
// // 		return {optimalMoveList[rand() % optimalMoveList.size()], minScore};
// // 	}
// // }


// void Game::explainMoves(vector<Move> trail, int score, Board boardConfig)
// {
// 	cerr << "The trail of moves predicted by bot is-\n";
// 	bool isOpponent = false;
// 	Board tempBoard = boardConfig;

// 	for(Move m : trail)
// 	{
// 		if(isOpponent)
// 			cerr << "Opponent : ";
// 		else
// 			cerr << "Beep-Dlue : ";
// 		cerr << moveToString(m) << '\n';
// 		makeMove(m, &tempBoard);
// 		tempBoard.printBoard();
// 		isOpponent = !isOpponent;
// 	}
	
// 	cerr << "The utility score predicted : " << score << "\n\n";
// }
