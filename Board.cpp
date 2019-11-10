#include "Board.h"
#include "Move.h"
#include <vector>
#include <iostream>
#include <tuple>
#include <set>

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
	if(n == 10)
	{
		config.push_back(dashes);
		config.push_back(dashes);
		cerr << "here" << endl;
	}
	config.push_back(soldiers);
	config.push_back(soldiers);
	config.push_back(townhalls);
}

Board::Board()
{

}

Board::Board(int nin, int min, bool debug)
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
		// Board newBoard(initBoard);
		// board = newBoard;
		config = initBoard;	
	}
	else{
		// Board newBoard(n, m);
		// board = newBoard;
	}
}

Board::Board(vector<vector<char>> initBoard){
	config = initBoard;
	n = initBoard.size();
	m = initBoard[0].size(); 
}

vector<Move> Board::validMoves(bool side)
{
	vector<Move> movesList;

	if(!side)
		reverseBoard();

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
				if (checkEnemy({ i - 1,j - 1 }) || checkEnemy({ i - 1,j }) || checkEnemy({ i - 1,j + 1 }) || checkEnemy({ i,j - 1 }) || checkEnemy({ i,j + 1 }))
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
				if (checkEnemyOrGoal({ i,j - 1 }))
				{
					Move newMove({ i,j }, { i,j - 1 }, 'a', true);
					movesList.push_back(newMove);
				}
				if (checkEnemyOrGoal({ i,j + 1 }))
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


	if(!side)
	{
		reverseBoard();
		vector<Move> opponentList;
		for(int i=0 ; i<movesList.size() ; i++)
		{
			pair<int,int> newFrom = {n - movesList[i].from.first - 1, m - movesList[i].from.second - 1};
			pair<int,int> newTo = {n - movesList[i].to.first - 1, m - movesList[i].to.second - 1};
			Move move(newFrom, newTo, movesList[i].type, false);
			opponentList.push_back(move);
		}
		return opponentList;
	}

	return movesList;
}


void Board::reverseBoard()
{
	vector<char> row(m, '-');
	vector<vector<char>> newConfig(n, row);

	for(int i=0 ; i<n ; i++)
	{
		for(int j=0 ; j<m ; j++)
		{
			if(config[i][j] == 'G')
				newConfig[n-i-1][m-j-1] = 'T';
			else if(config[i][j] == 'E')
				newConfig[n-i-1][m-j-1] = 'S';
			else if(config[i][j] == 'S')
				newConfig[n-i-1][m-j-1] = 'E';
			else if(config[i][j] == 'T')
				newConfig[n-i-1][m-j-1] = 'G';
		}
	}

	config = newConfig;
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

bool Board::checkEnemyOrGoal(pair<int, int> targetPos)
{	
	return positionIsValid(targetPos) && ((config[targetPos.first][targetPos.second] == 'E') || (config[targetPos.first][targetPos.second] == 'G'));
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


//Returns the number of soldiers, enemies
pair<int,int> Board::countEnemies()
{
	int numSoldiers = 0;
	int numEnemies = 0;
	for(int i=0 ; i<n ; i++)
	{
		for(int j=0 ; j<m ; j++)
		{
			if(config[i][j]=='S')
				numSoldiers++;
			if(config[i][j]=='E')
				numEnemies++;
		}
	}
	return {numSoldiers, numEnemies};
}

//Checks if the game has ended
bool Board::isTerminal(bool side)
{
	int townhallCount = 0;
	int goalCount = 0;
	int enemyCount = 0;
	for(int i=0 ; i<n ; i++)
	{
		for(int j=0 ; j<m ; j++)
		{
			if(config[i][j] == 'G')
				goalCount++;
			else if(config[i][j] == 'T')
				townhallCount++;
			else if(config[i][j] == 'E')
				enemyCount++;
		}
	}

	if(townhallCount <= 2 || goalCount <= 2 || enemyCount == 0)
		return true;

	return (validMoves(side).size() == 0);
}


//Returns the utility value of the end state
int Board::utilityScore()
{
	int townhallCount = 0;
	int goalCount = 0;
	for(int i=0 ; i<m ; i++)
	{
		if(config[0][i] == 'G')
			goalCount++;
		if(config[n-1][i] == 'T')
			townhallCount++;
	}

	return (townhallCount - goalCount)*100;
}

//Returns the number of vertical, horizontal and diagonal cannons, and number of enemies under attack by cannons, soldiers and retreats
tuple <int, int, int, int, int, int, int, int, int, int, int> Board::cannonsAndSoldierAttacks(bool side)
{
	int vertCannons = 0, horizCannons = 0, diagCannons = 0, cannonAttack = 0, soldierAttackNormal = 0, soldierAttackRetreat = 0, cannonAttackingPosition = 0;
	int goalScore = 3;		//checked that keeping it 10 results in more aggression
	int numSoldiersOnAttack = 0, numEnemiesUnderAttack = 0;
	int numGoalsUnderAttack = 0, numTownhallsUnderAttack = 0;
	set<pair<int,int>> hash_Set1;//soldiers on attack
	set<pair<int,int>> hash_Set2;//enemies under attack
	set<pair<int,int>> hash_Set3;//goals under 
	set<pair<int,int>> enemiesUnderCannonAttack;
	if(!side)
		reverseBoard();

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (checkSoldier({ i,j }))
			{
				//Adance soldier forward in 3 directions
				if (positionIsValid({ i - 1,j - 1 }) && !checkSoldierOrTownhall({ i - 1,j - 1 }))
				{
					if(checkEnemy({ i - 1,j - 1 }))
					{
						soldierAttackNormal++;
						hash_Set1.insert({i,j});
						hash_Set2.insert({ i - 1,j - 1 });
					}
					else if(checkGoal({ i - 1,j - 1 }))
					{
						soldierAttackNormal += goalScore;
						hash_Set1.insert({i,j});
						//hash_Set2.insert({ i - 1,j - 1 });
						hash_Set3.insert({ i - 1,j - 1 });
					}
				}
				if (positionIsValid({ i - 1,j }) && !checkSoldierOrTownhall({ i - 1,j }))
				{
					if(checkEnemy({ i - 1,j}))
					{
						soldierAttackNormal++;
						hash_Set1.insert({i,j});
						hash_Set2.insert({ i - 1,j });
					}
					else if(checkGoal({ i - 1,j }))
					{
						soldierAttackNormal += goalScore;
						hash_Set1.insert({i,j});
						// hash_Set2.insert({ i - 1,j });
						hash_Set3.insert({ i - 1,j });
					}
				}
				if (positionIsValid({ i - 1,j + 1 }) && !checkSoldierOrTownhall({ i - 1,j + 1 }))
				{
					if(checkEnemy({ i - 1,j + 1 }))
					{
						soldierAttackNormal++;
						hash_Set1.insert({i,j});
						hash_Set2.insert({ i - 1,j + 1 });
					}
					else if(checkGoal({ i - 1,j + 1 }))
					{
						soldierAttackNormal += goalScore;
						hash_Set1.insert({i,j});
						// hash_Set2.insert({ i - 1,j + 1 });
						hash_Set3.insert({ i - 1,j + 1});
					}
				}

				//Retreat in 3 directions
				if (checkEnemy({ i - 1,j - 1 }) || checkEnemy({ i - 1,j }) || checkEnemy({ i - 1,j + 1 }) || checkEnemy({ i,j - 1 }) || checkEnemy({ i,j + 1 }))
				{
					if (positionIsValid({ i + 2,j - 2 }) && !checkSoldierOrTownhall({ i + 2,j - 2 }))
					{
						if(checkEnemy({ i + 2,j - 2 }))
						{
							soldierAttackRetreat++;
							hash_Set1.insert({i,j});
							hash_Set2.insert({ i + 2,j - 2 });
						}
					}
					if (positionIsValid({ i + 2,j }) && !checkSoldierOrTownhall({ i + 2,j }))
					{
						if(checkEnemy({ i + 2,j }))
						{
							soldierAttackRetreat++;
							hash_Set1.insert({i,j});
							hash_Set2.insert({ i + 2,j });
						}
					}
					if (positionIsValid({ i + 2,j + 2 }) && !checkSoldierOrTownhall({ i + 2,j + 2 }))
					{
						if(checkEnemy({ i + 2,j + 2 }))
						{
							soldierAttackRetreat++;
							hash_Set1.insert({i,j});
							hash_Set2.insert({ i + 2,j + 2 });
						}
					}
				}

				//Kill horizontal enemies
				if(checkEnemy({ i,j - 1 }))
				{
					soldierAttackNormal++;
					hash_Set1.insert({i,j});
					hash_Set2.insert({ i ,j - 1 });
				}
				else if(checkGoal({ i ,j - 1 }))
				{
					soldierAttackNormal += goalScore;
					hash_Set1.insert({i,j});
					// hash_Set2.insert({ i ,j - 1 });
					hash_Set3.insert({ i ,j - 1});
				}
				
				if(checkEnemy({ i,j + 1 }))
				{
					soldierAttackNormal++;
					hash_Set1.insert({i,j});
					hash_Set2.insert({ i ,j + 1 });
				}
				else if(checkGoal({ i,j + 1 }))
				{
					soldierAttackNormal += goalScore;
					hash_Set1.insert({i,j});
					// hash_Set2.insert({ i ,j + 1 });
					hash_Set3.insert({ i ,j + 1});
				}

				//Check the 3 cannons possible with centre at [i,j]
				//Horizontal cannon
				if (checkSoldier({ i,j - 1 }) && checkSoldier({ i,j + 1 }))
				{
					horizCannons++;
					if (positionIsValid({ i,j - 3 }) && !checkSoldierOrTownhall({ i,j - 3 }) && checkEmpty({ i,j - 2 }))
					{
						cannonAttackingPosition++;
						if(checkEnemy({ i ,j - 3 }))
						{
							cannonAttack++;
							hash_Set2.insert({ i ,j - 3 });
							enemiesUnderCannonAttack.insert({ i ,j - 3 });
						}
					}
					if (positionIsValid({ i,j - 4 }) && !checkSoldierOrTownhall({ i,j - 4 }) && checkEmpty({ i,j - 2 }))
					{
						cannonAttackingPosition++;
						if(checkEnemy({ i ,j - 4 }))
						{
							cannonAttack++;
							hash_Set2.insert({ i ,j - 4 });
							enemiesUnderCannonAttack.insert({ i ,j - 4 });
						}
					}
					if (positionIsValid({ i,j + 3 }) && !checkSoldierOrTownhall({ i,j + 3 }) && checkEmpty({ i,j + 2 }))
					{
						cannonAttackingPosition++;
						if(checkEnemy({ i ,j + 3 }))
						{
							cannonAttack++;
							hash_Set2.insert({ i ,j + 3 });
							enemiesUnderCannonAttack.insert({ i ,j + 3 });
						}
					}
					if (positionIsValid({ i,j + 4 }) && !checkSoldierOrTownhall({ i,j + 4 }) && checkEmpty({ i,j + 2 }))
					{
						cannonAttackingPosition++;
						if(checkEnemy({ i ,j + 4 }))
						{
							cannonAttack++;
							hash_Set2.insert({ i ,j + 4 });
							enemiesUnderCannonAttack.insert({ i ,j + 4 });
						}
					}
				}

				//Vertical cannon
				if (checkSoldier({ i - 1,j }) && checkSoldier({ i + 1,j }))
				{
					vertCannons++;
					if (positionIsValid({ i - 3,j }) && !checkSoldierOrTownhall({ i - 3,j }) && checkEmpty({ i - 2,j }))
					{
						cannonAttackingPosition++;
						if(checkEnemy({ i - 3,j }))
						{
							cannonAttack++;
							hash_Set2.insert({ i - 3,j });
							enemiesUnderCannonAttack.insert({ i - 3,j });
						}
						else if(checkGoal({ i - 3,j }))
						{
							cannonAttack += goalScore;
							// hash_Set2.insert({ i - 3,j });
							hash_Set3.insert({ i - 3,j });
						}
					}
					if (positionIsValid({ i - 4,j }) && !checkSoldierOrTownhall({ i - 4,j }) && checkEmpty({ i - 2,j }))
					{
						cannonAttackingPosition++;
						if(checkEnemy({ i - 4,j }))
						{
							cannonAttack++;
							hash_Set2.insert({ i - 4,j });
							enemiesUnderCannonAttack.insert({ i - 4,j });
						}
						else if(checkGoal({ i - 4,j }))
						{
							cannonAttack += goalScore;
							// hash_Set2.insert({ i - 4,j });
							hash_Set3.insert({ i - 4,j });
						}
					}
					if (positionIsValid({ i + 3,j }) && !checkSoldierOrTownhall({ i + 3,j }) && checkEmpty({ i + 2,j }))
					{
						cannonAttackingPosition++;
						if(checkEnemy({ i + 3,j }))
						{
							cannonAttack++;
							hash_Set2.insert({ i + 3,j });
							enemiesUnderCannonAttack.insert({ i + 3,j });
						}
					}
					if (positionIsValid({ i + 4,j }) && !checkSoldierOrTownhall({ i + 4,j }) && checkEmpty({ i + 2,j }))
					{
						cannonAttackingPosition++;
						if(checkEnemy({ i + 4,j }))
						{
							cannonAttack++;
							hash_Set2.insert({ i + 4,j });
							enemiesUnderCannonAttack.insert({ i + 4,j });
						}
					}
				}

				//Diagonal1 cannon
				if (checkSoldier({ i - 1,j - 1 }) && checkSoldier({ i + 1,j + 1 }))
				{
					if(j == 3 || j == 4 || j == 5 || j == 6)
						diagCannons++;
					if (positionIsValid({ i - 3,j - 3 }) && !checkSoldierOrTownhall({ i - 3,j - 3 }) && checkEmpty({ i - 2,j - 2 }))
					{
						cannonAttackingPosition++;
						if(checkEnemy({ i - 3 ,j - 3 }))
						{
							cannonAttack++;
							hash_Set2.insert({ i - 3,j - 3 });
							enemiesUnderCannonAttack.insert({ i - 3,j - 3 });
						}
						else if(checkGoal({ i - 3,j - 3}))
						{
							cannonAttack += goalScore;
							// hash_Set2.insert({ i - 3,j - 3});
							hash_Set3.insert({ i - 3,j - 3});
						}
					}
					if (positionIsValid({ i - 4,j - 4 }) && !checkSoldierOrTownhall({ i - 4,j - 4 }) && checkEmpty({ i - 2,j - 2 }))
					{
						cannonAttackingPosition++;
						if(checkEnemy({ i - 4,j - 4 }))
						{
							cannonAttack++;
							hash_Set2.insert({ i - 4,j - 4 });
							enemiesUnderCannonAttack.insert({ i - 4,j - 4 });
						}
						else if(checkGoal({ i - 4,j - 4 }))
						{
							cannonAttack += goalScore;
							// hash_Set2.insert({ i - 4,j - 4 });
							hash_Set3.insert({ i - 4,j - 4});
						}
					}
					if (positionIsValid({ i + 3,j + 3 }) && !checkSoldierOrTownhall({ i + 3,j + 3 }) && checkEmpty({ i + 2,j + 2 }))
					{
						cannonAttackingPosition++;
						if(checkEnemy({ i + 3,j + 3 }))
						{
							cannonAttack++;
							hash_Set2.insert({ i + 3,j + 3 });
							enemiesUnderCannonAttack.insert({ i + 3,j + 3 });
						}
					}
					if (positionIsValid({ i + 4,j + 4 }) && !checkSoldierOrTownhall({ i + 4,j + 4 }) && checkEmpty({ i + 2,j + 2 }))
					{
						cannonAttackingPosition++;
						if(checkEnemy({ i + 4,j + 4 }))
						{
							cannonAttack++;
							hash_Set2.insert({ i + 4,j + 4 });
							enemiesUnderCannonAttack.insert({ i + 4,j + 4 });
						}
					}
				}

				//Diagonal2 cannon
				if (checkSoldier({ i - 1,j + 1 }) && checkSoldier({ i + 1,j - 1 }))
				{
					if(j == 1 || j == 2 || j == 3 || j == 4)
						diagCannons++;
					if (positionIsValid({ i - 3,j + 3 }) && !checkSoldierOrTownhall({ i - 3,j + 3 }) && checkEmpty({ i - 2,j + 2 }))
					{
						cannonAttackingPosition++;
						if(checkEnemy({ i - 3,j + 3 }))
						{
							cannonAttack++;
							hash_Set2.insert({ i - 3,j + 3});
							enemiesUnderCannonAttack.insert({ i - 3,j + 3});
						}
						else if(checkGoal({ i - 3,j + 3 }))
						{
							cannonAttack += goalScore;
							// hash_Set2.insert({ i - 3,j + 3 });
							hash_Set3.insert({ i - 3,j + 3});
						}
					}
					if (positionIsValid({ i - 4,j + 4 }) && !checkSoldierOrTownhall({ i - 4,j + 4 }) && checkEmpty({ i - 2,j + 2 }))
					{
						cannonAttackingPosition++;
						if(checkEnemy({ i - 4,j + 4 }))
						{
							cannonAttack++;
							hash_Set2.insert({ i - 4,j + 4});
							enemiesUnderCannonAttack.insert({ i - 4,j + 4});
						}
						else if(checkGoal({ i - 4,j + 4}))
						{
							cannonAttack += goalScore;
							// hash_Set2.insert({ i - 4,j + 4 });
							hash_Set3.insert({ i - 4,j + 4});
						}
					}
					if (positionIsValid({ i + 3,j - 3 }) && !checkSoldierOrTownhall({ i + 3,j - 3 }) && checkEmpty({ i + 2,j - 2 }))
					{
						cannonAttackingPosition++;
						if(checkEnemy({ i + 3,j - 3 }))
						{
							cannonAttack++;
							hash_Set2.insert({ i + 3,j - 3 });
							enemiesUnderCannonAttack.insert({ i + 3,j - 3 });
						}
					}
					if (positionIsValid({ i + 4,j - 4 }) && !checkSoldierOrTownhall({ i + 4,j - 4 }) && checkEmpty({ i + 2,j - 2 }))
					{
						cannonAttackingPosition++;
						if(checkEnemy({ i + 4 ,j + 4 }))
						{
							cannonAttack++;
							hash_Set2.insert({ i + 4,j + 4 });
							enemiesUnderCannonAttack.insert({ i + 4,j + 4 });
						}
					}
				}
			}
		}
	}

	if(!side)
		reverseBoard();

	return make_tuple(vertCannons, horizCannons, diagCannons, cannonAttack, soldierAttackNormal, soldierAttackRetreat, cannonAttackingPosition, hash_Set1.size(), hash_Set2.size(), hash_Set3.size(),enemiesUnderCannonAttack.size());

}

//Returns the heuristic goodness of the board configuration
int Board::heuristicScore()
{
	int townhallCount = 0;
	int goalCount = 0;
	int enemyCount = 0;
	int soldierCount = 0;
	int vertCannons, horizCannons, diagCannons, cannonAttack, soldierAttackNormal, soldierAttackRetreat, cannonAttackingPosition;
	int numOfSoldierOnAttack, numOfEnemiesOnAttack, numOfSoldierUnderAttack, numOfEnemiesUnderAttack, enemiesUnderCannonAttack, soldiersUnderCannonAttack;
	int numGoalsOnAttack, numTownhallsUnderAttack;
	int myCannonAttack, enemyCannonAttack;
	int soldierPosition = 0, enemyPosition = 0; 
	//Counting the pieces on the board
	for(int i=0 ; i<n ; i++)
	{
		for(int j=0 ; j<m ; j++)
		{
			if(config[i][j] == 'G')
				goalCount++;
			if(config[i][j] == 'T')
				townhallCount++;
			if(config[i][j] == 'E'){
				enemyCount += 1;
				enemyPosition += i;
			}
			if(config[i][j] == 'S'){
				soldierCount += 1;
				soldierPosition += n-i;
			}
		}
	}

	tuple<int, int, int, int, int, int, int, int, int, int, int> answerBlack = cannonsAndSoldierAttacks(true);
	tuple<int, int, int, int, int, int, int, int, int, int, int> answerWhite = cannonsAndSoldierAttacks(false);
	vertCannons = get<0>(answerBlack) - get<0>(answerWhite);
	horizCannons =  get<1>(answerBlack) - get<1>(answerWhite);
	diagCannons =  get<2>(answerBlack) - get<2>(answerWhite);
	cannonAttack =  get<3>(answerBlack) - get<3>(answerWhite);
	myCannonAttack = get<3>(answerBlack);
	enemyCannonAttack = get<3>(answerWhite);
	soldierAttackNormal = get<4>(answerBlack) - get<4>(answerWhite);
	soldierAttackRetreat =  get<5>(answerBlack) - get<5>(answerWhite);
	cannonAttackingPosition = get<6>(answerBlack) - get<6>(answerWhite);
	numOfSoldierOnAttack = get<7>(answerBlack);
	numOfEnemiesOnAttack = get<7>(answerWhite);
	numOfEnemiesUnderAttack = get<8>(answerBlack);
	numOfSoldierUnderAttack = get<8>(answerWhite);
	numGoalsOnAttack = get<9>(answerBlack);
	numTownhallsUnderAttack = get<9>(answerWhite);
	enemiesUnderCannonAttack = get<10>(answerBlack);
	soldiersUnderCannonAttack = get<10>(answerWhite);

	// return (soldierPosition - enemyPosition) + 4*vertCannons + 3*diagCannons + 1*horizCannons + 3*cannonAttack + 2*cannonAttackingPosition + 6*soldierAttackRetreat+ 0*soldierAttackNormal + 5*(numOfEnemiesUnderAttack) - 5*(numOfSoldierUnderAttack) -5*soldiersUnderCannonAttack + 5*enemiesUnderCannonAttack + 100*(soldierCount - enemyCount) + 50*soldierCount + 3000*(townhallCount - goalCount) + 0*numGoalsOnAttack - 500*numTownhallsUnderAttack;
	// return  4*vertCannons + 3*diagCannons + 1*horizCannons + 3*cannonAttack + 2*cannonAttackingPosition + 6*soldierAttackRetreat+ 0*soldierAttackNormal + 0*(numOfSoldierOnAttack) - 5*(numOfSoldierUnderAttack) -5*soldiersUnderCannonAttack + 5*enemiesUnderCannonAttack + 100*(soldierCount - enemyCount) + 50*soldierCount + 3000*(townhallCount - goalCount) + 0*numGoalsOnAttack - 500*numTownhallsUnderAttack;
	
	if(soldierCount >= 7)
		return (soldierPosition - enemyPosition) + 4*vertCannons + 3*diagCannons + 1*horizCannons + 3*cannonAttack + 2*cannonAttackingPosition + 6*soldierAttackRetreat+ 0*soldierAttackNormal + 5*(numOfEnemiesUnderAttack) - 5*(numOfSoldierUnderAttack) - 5*soldiersUnderCannonAttack + 5*enemiesUnderCannonAttack + 100*(soldierCount - enemyCount) + 50*soldierCount + 3000*(townhallCount - goalCount) + 0*numGoalsOnAttack - 500*numTownhallsUnderAttack;
	else
		return (soldierPosition - enemyPosition) + 3*vertCannons + 2*diagCannons + 2*horizCannons + 3*cannonAttack + 2*cannonAttackingPosition + 6*soldierAttackRetreat+ 0*soldierAttackNormal + 10*numOfEnemiesUnderAttack - 15*(numOfSoldierUnderAttack) - 5*soldiersUnderCannonAttack + 5*enemiesUnderCannonAttack + 100*(soldierCount - enemyCount) + 50*soldierCount + 3000*(townhallCount - goalCount) + 0*numGoalsOnAttack - 500*numTownhallsUnderAttack;

	// if(soldierCount >= 7)
	// 	return 4*vertCannons + 3*diagCannons + 1*horizCannons + 3*cannonAttack + 2*cannonAttackingPosition + 6*soldierAttackRetreat+ 0*soldierAttackNormal + 0*(numOfSoldierOnAttack) - 10*(numOfSoldierUnderAttack) + 100*(soldierCount - enemyCount) + 50*soldierCount + 3000*(townhallCount - goalCount) + 0*numGoalsOnAttack - 500*numTownhallsUnderAttack;
	// else
	// 	return 3*vertCannons + 2*diagCannons + 2*horizCannons + 3*cannonAttack + 2*cannonAttackingPosition + 6*soldierAttackRetreat+ 0*soldierAttackNormal + 10*(numOfSoldierOnAttack) - 15*(numOfSoldierUnderAttack) + 100*(soldierCount - enemyCount) + 50*soldierCount + 3000*(townhallCount - goalCount) + 0*numGoalsOnAttack - 500*numTownhallsUnderAttack;
	
}