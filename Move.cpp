#include "pch.h"
#include "Move.h"
#include <vector>

using namespace std;

//Constructor
Move::Move(pair<int, int> fromin, pair<int, int> toin, char typein, bool polarityin)
{
	from = fromin;
	to = toin;
	type = typein;
	polarity = polarityin;
}