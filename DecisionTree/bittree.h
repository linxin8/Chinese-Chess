#pragma once
#include"bitboard.h"
#include"estimate.h"
class BitTree
{
public:
	BitTree(int map[10][9]) :board(map)
	{ 
	}
	~BitTree() = default;
	ChessTarget& getTarget(int x, int y)
	{ 
		return board.getTargetByIndex((y + 3) * 16 + x + 3);
	} 
	void deepSearch(int alpha, int beta, int country)
	{
	}
	int getEstimatedValue()
	{
		return board.getEstimatedValue();
	}
private:
	BitBoard board;
};

