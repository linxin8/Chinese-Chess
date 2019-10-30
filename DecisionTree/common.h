#pragma once 

struct Position
{
	int x;
	int y;
	constexpr Position(int x, int y) :x(x), y(y) {}
};
 

// [min,max)
inline bool isInRange(int value,int min, int max)
{
	return min <= value && value < max;
}