#include "pch.h"
#include "Board.h"
#include "Move.h"
#include <vector>
#include <iostream>

using namespace std;


//Constructor
Board::Board(int nin, int min)
{
	n = nin;
	m = min;
		
	vector<char> dashes;
	vector<char> townhalls;
	vector<char> goals;
	bool putTownhall = false;
	vector<char> soldiers;
	vector<char> enemies;
	bool putSoldier = true;

	for (int i = 0; i < m; i++)
		dashes.push_back('-');

	for (int i = 0; i < m; i++)
	{
		if (putTownhall)
		{
			townhalls.push_back('T');
			goals.push_back('E');
		}
		else
		{
			townhalls.push_back('S');
			goals.push_back('G');
		}

		if (putSoldier)
		{
			soldiers.push_back('S');
			enemies.push_back('-');
		}
		else
		{
			soldiers.push_back('-');
			enemies.push_back('E');
		}

		putTownhall = !putTownhall;
		putSoldier = !putSoldier;
	}

	//Make the following lines a function of 'n'
	config.push_back(goals);
	config.push_back(enemies);
	config.push_back(enemies);
	config.push_back(dashes);
	config.push_back(dashes);
	config.push_back(soldiers);
	config.push_back(soldiers);
	config.push_back(townhalls);
}

Board::Board()
{

}



vector<Move> Board::validMoves()
{
	vector<Move> movesList;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (checkSoldier({ i,j }))
			{
				//Adance soldier forward in 3 directions
				if (positionIsValid({ i - 1,j - 1 }) && !checkSoldierOrTownhall({ i - 1,j - 1 }))
				{
					Move newMove({ i,j }, { i - 1,j - 1 }, 'a', true);
					movesList.push_back(newMove);
				}
				if (positionIsValid({ i - 1,j }) && !checkSoldierOrTownhall({ i - 1,j }))
				{
					Move newMove({ i,j }, { i - 1,j }, 'a', true);
					movesList.push_back(newMove);
				}
				if (positionIsValid({ i - 1,j + 1 }) && !checkSoldierOrTownhall({ i - 1,j + 1 }))
				{
					Move newMove({ i,j }, { i - 1,j + 1 }, 'a', true);
					movesList.push_back(newMove);
				}

				//Retreat in 3 directions
				if (checkEnemy({ i - 1,j - 1 }) || checkEnemy({ i - 1,j }) || checkEnemy({ i - 1,j + 1 }) || checkEnemy({ i,j - 1 }) || checkEnemy({ i,j + 1 }) || checkEnemy({ i + 1,j - 1 }) || checkEnemy({ i + 1,j }) || checkEnemy({ i + 1,j + 1 }))
				{
					if (positionIsValid({ i + 2,j - 2 }) && !checkSoldierOrTownhall({ i + 2,j - 2 }))
					{
						Move newMove({ i,j }, { i + 2,j - 2 }, 'a', true);
						movesList.push_back(newMove);
					}
					if (positionIsValid({ i + 2,j }) && !checkSoldierOrTownhall({ i + 2,j }))
					{
						Move newMove({ i,j }, { i + 2,j }, 'a', true);
						movesList.push_back(newMove);
					}
					if (positionIsValid({ i + 2,j + 2 }) && !checkSoldierOrTownhall({ i + 2,j + 2 }))
					{
						Move newMove({ i,j }, { i + 2,j + 2 }, 'a', true);
						movesList.push_back(newMove);
					}
				}

				//Kill horizontal enemies
				if (checkEnemy({ i,j - 1 }))
				{
					Move newMove({ i,j }, { i,j - 1 }, 'a', true);
					movesList.push_back(newMove);
				}
				if (checkEnemy({ i,j + 1 }))
				{
					Move newMove({ i,j }, { i,j + 1 }, 'a', true);
					movesList.push_back(newMove);
				}

				//Check the 3 cannons possible with centre at [i,j]
				//Horizontal cannon
				if (checkSoldier({ i,j - 1 }) && checkSoldier({ i,j + 1 }))
				{
					if (positionIsValid({ i,j - 3 }) && !checkSoldierOrTownhall({ i,j - 3 }) && checkEmpty({ i,j - 2 }))
					{
						Move newMove({ i,j }, { i,j - 3 }, 'f', true);
						movesList.push_back(newMove);
					}
					if (positionIsValid({ i,j - 4 }) && !checkSoldierOrTownhall({ i,j - 4 }) && checkEmpty({ i,j - 2 }))
					{
						Move newMove({ i,j }, { i,j - 4 }, 'f', true);
						movesList.push_back(newMove);
					}
					if (positionIsValid({ i,j + 3 }) && !checkSoldierOrTownhall({ i,j + 3 }) && checkEmpty({ i,j + 2 }))
					{
						Move newMove({ i,j }, { i,j + 3 }, 'f', true);
						movesList.push_back(newMove);
					}
					if (positionIsValid({ i,j + 4 }) && !checkSoldierOrTownhall({ i,j + 4 }) && checkEmpty({ i,j + 2 }))
					{
						Move newMove({ i,j }, { i,j + 4 }, 'f', true);
						movesList.push_back(newMove);
					}
					if (positionIsValid({ i,j - 2 }) && !checkSoldierOrTownhall({ i,j - 2 }) && !checkEnemy({ i,j - 2 }) && !checkGoal({ i,j - 2 }))
					{
						Move newMove({ i,j + 1 }, { i,j - 2 }, 'a', true);
						movesList.push_back(newMove);
					}
					if (positionIsValid({ i,j + 2 }) && !checkSoldierOrTownhall({ i,j + 2 }) && !checkEnemy({ i,j + 2 }) && !checkGoal({ i,j + 2 }))
					{
						Move newMove({ i,j - 1 }, { i,j + 2 }, 'a', true);
						movesList.push_back(newMove);
					}
				}

				//Vertical cannon
				if (checkSoldier({ i - 1,j }) && checkSoldier({ i + 1,j }))
				{
					if (positionIsValid({ i - 3,j }) && !checkSoldierOrTownhall({ i - 3,j }) && checkEmpty({ i - 2,j }))
					{
						Move newMove({ i,j }, { i - 3,j }, 'f', true);
						movesList.push_back(newMove);
					}
					if (positionIsValid({ i - 4,j }) && !checkSoldierOrTownhall({ i - 4,j }) && checkEmpty({ i - 2,j }))
					{
						Move newMove({ i,j }, { i - 4,j }, 'f', true);
						movesList.push_back(newMove);
					}
					if (positionIsValid({ i + 3,j }) && !checkSoldierOrTownhall({ i + 3,j }) && checkEmpty({ i + 2,j }))
					{
						Move newMove({ i,j }, { i + 3,j }, 'f', true);
						movesList.push_back(newMove);
					}
					if (positionIsValid({ i + 4,j }) && !checkSoldierOrTownhall({ i + 4,j }) && checkEmpty({ i + 2,j }))
					{
						Move newMove({ i,j }, { i + 4,j }, 'f', true);
						movesList.push_back(newMove);
					}
					if (positionIsValid({ i - 2,j }) && !checkSoldierOrTownhall({ i - 2,j }) && !checkEnemy({ i - 2,j }) && !checkGoal({ i - 2,j }))
					{
						Move newMove({ i + 1,j }, { i - 2,j }, 'a', true);
						movesList.push_back(newMove);
					}
					if (positionIsValid({ i + 2,j }) && !checkSoldierOrTownhall({ i + 2,j }) && !checkEnemy({ i + 2,j }) && !checkGoal({ i + 2,j }))
					{
						Move newMove({ i - 1,j }, { i + 2,j }, 'a', true);
						movesList.push_back(newMove);
					}
				}

				//Diagonal1 cannon
				if (checkSoldier({ i - 1,j - 1 }) && checkSoldier({ i + 1,j + 1 }))
				{
					if (positionIsValid({ i - 3,j - 3 }) && !checkSoldierOrTownhall({ i - 3,j - 3 }) && checkEmpty({ i - 2,j - 2 }))
					{
						Move newMove({ i,j }, { i - 3,j - 3 }, 'f', true);
						movesList.push_back(newMove);
					}
					if (positionIsValid({ i - 4,j - 4 }) && !checkSoldierOrTownhall({ i - 4,j - 4 }) && checkEmpty({ i - 2,j - 2 }))
					{
						Move newMove({ i,j }, { i - 4,j - 4 }, 'f', true);
						movesList.push_back(newMove);
					}
					if (positionIsValid({ i + 3,j + 3 }) && !checkSoldierOrTownhall({ i + 3,j + 3 }) && checkEmpty({ i + 2,j + 2 }))
					{
						Move newMove({ i,j }, { i + 3,j + 3 }, 'f', true);
						movesList.push_back(newMove);
					}
					if (positionIsValid({ i + 4,j + 4 }) && !checkSoldierOrTownhall({ i + 4,j + 4 }) && checkEmpty({ i + 2,j + 2 }))
					{
						Move newMove({ i,j }, { i + 4,j + 4 }, 'f', true);
						movesList.push_back(newMove);
					}
					if (positionIsValid({ i - 2,j - 2 }) && !checkSoldierOrTownhall({ i - 2,j - 2 }) && !checkEnemy({ i - 2,j - 2 }) && !checkGoal({ i - 2,j - 2 }))
					{
						Move newMove({ i + 1,j + 1 }, { i - 2,j - 2 }, 'a', true);
						movesList.push_back(newMove);
					}
					if (positionIsValid({ i + 2,j + 2 }) && !checkSoldierOrTownhall({ i + 2,j + 2 }) && !checkEnemy({ i + 2,j + 2 }) && !checkGoal({ i + 2,j + 2 }))
					{
						Move newMove({ i - 1,j - 1 }, { i + 2,j + 2 }, 'a', true);
						movesList.push_back(newMove);
					}
				}

				//Diagonal2 cannon
				if (checkSoldier({ i - 1,j + 1 }) && checkSoldier({ i + 1,j - 1 }))
				{
					if (positionIsValid({ i - 3,j + 3 }) && !checkSoldierOrTownhall({ i - 3,j + 3 }) && checkEmpty({ i - 2,j + 2 }))
					{
						Move newMove({ i,j }, { i - 3,j + 3 }, 'f', true);
						movesList.push_back(newMove);
					}
					if (positionIsValid({ i - 4,j + 4 }) && !checkSoldierOrTownhall({ i - 4,j + 4 }) && checkEmpty({ i - 2,j + 2 }))
					{
						Move newMove({ i,j }, { i - 4,j + 4 }, 'f', true);
						movesList.push_back(newMove);
					}
					if (positionIsValid({ i + 3,j - 3 }) && !checkSoldierOrTownhall({ i + 3,j - 3 }) && checkEmpty({ i + 2,j - 2 }))
					{
						Move newMove({ i,j }, { i + 3,j - 3 }, 'f', true);
						movesList.push_back(newMove);
					}
					if (positionIsValid({ i + 4,j - 4 }) && !checkSoldierOrTownhall({ i + 4,j - 4 }) && checkEmpty({ i + 2,j - 2 }))
					{
						Move newMove({ i,j }, { i + 4,j - 4 }, 'f', true);
						movesList.push_back(newMove);
					}
					if (positionIsValid({ i - 2,j + 2 }) && !checkSoldierOrTownhall({ i - 2,j + 2 }) && !checkEnemy({ i - 2,j + 2 }) && !checkGoal({ i - 2,j + 2 }))
					{
						Move newMove({ i + 1,j - 1 }, { i - 2,j + 2 }, 'a', true);
						movesList.push_back(newMove);
					}
					if (positionIsValid({ i + 2,j - 2 }) && !checkSoldierOrTownhall({ i + 2,j - 2 }) && !checkEnemy({ i + 2,j - 2 }) && !checkGoal({ i + 2,j - 2 }))
					{
						Move newMove({ i - 1,j + 1 }, { i + 2,j - 2 }, 'a', true);
						movesList.push_back(newMove);
					}
				}
			}
		}
	}

	return movesList;
}


bool Board::positionIsValid(pair<int, int> pos)
{
	return (pos.first >= 0) && (pos.first < n) && (pos.second >= 0) && (pos.second < m);
}

bool Board::checkEnemy(pair<int, int> targetPos)
{
	return positionIsValid(targetPos) && (config[targetPos.first][targetPos.second] == 'E');
}

bool Board::checkSoldier(pair<int, int> targetPos)
{
	return positionIsValid(targetPos) && (config[targetPos.first][targetPos.second] == 'S');
}

bool Board::checkTownhall(pair<int, int> targetPos)
{
	return positionIsValid(targetPos) && (config[targetPos.first][targetPos.second] == 'T');
}

bool Board::checkGoal(pair<int, int> targetPos)
{
	return positionIsValid(targetPos) && (config[targetPos.first][targetPos.second] == 'G');
}

bool Board::checkEmpty(pair<int,int> targetPos)
{
	return positionIsValid(targetPos) && (config[targetPos.first][targetPos.second] == '-');	
}

bool Board::checkSoldierOrTownhall(pair<int, int> targetPos)
{	
	return positionIsValid(targetPos) && ((config[targetPos.first][targetPos.second] == 'S') || (config[targetPos.first][targetPos.second] == 'T'));
}

void Board::printBoard()
{
	for(int i=0 ; i<n ; i++)
	{
		for(int j=0 ; j<m ; j++)
		{
			cerr << config[i][j] << " ";
		}
		cerr << endl;
	}
}


bool Board::isTerminal()
{
	int townhallCount = 0;
	int goalCount = 0;
	for(int i=0 ; i<m ; i++)
	{
		if(config[0][i] == 'G')
			goalCount++;
		if(config[7][i] == 'T')
			townhallCount++;
	}

	if(townhallCount <= 2 || goalCount <= 2)
		return true;

	return (validMoves().size() == 0);
}


int Board::utilityScore()
{
	return rand()%10;
}


int Board::heuristicScore()
{
	return rand()%10;
}