#pragma once

struct Decision
{
	int fromX;
	int fromY;
	int desX;
	int desY;
};

struct PositionList
{
	int x[20];
	int y[20];
	int length;
};

extern "C"
{
	__declspec(dllexport) const char* getName();
	__declspec(dllexport) const char* getVersion();
	__declspec(dllexport) void getDecision(int map[10][9], Decision* d);
	__declspec(dllexport) void getAccessiblePosition(int map[10][9], int x, int y, PositionList* positionList);
	  
}
