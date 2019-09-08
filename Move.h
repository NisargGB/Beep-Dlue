#pragma once
#include <vector>

using namespace std;

class Move
{
public:
	pair<int, int> from, to;
	char type;			//'f' for fire, 'c' for cannon move, 'k' for kill, 'a' for advance soldier, 'r' for retreat soldier
	bool polarity;
	Move(pair<int, int> fromin, pair<int, int> toin, char typein, bool polarityin);
};