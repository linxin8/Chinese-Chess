#include <iostream>
#include "dllfunction.h"
#include "tree.h"
 
const char* getName()
{
	return  "tree decision dll";
}

const char* getVersion()
{
	return  "v1.0"; 
}

void DLL_init()
{
	setvbuf(stdout, NULL, _IONBF, 0);// none buff
}

void getDecision(int map[10][9], Decision* d)
{
	DLL_init(); 
	Tree tree(map);
	*d = *tree.deepSearch();
}

void getAccessiblePosition(int map[10][9], int x, int y, PositionList* positionList)
{
	DLL_init();
	Tree tree(map);
	auto target = tree.getTarget(x, y);
	int length = 0; 
	for (auto& p : target->assaultableList)
	{ 
		positionList->x[length] = p->x;
		positionList->y[length] = p->y;
		length++;
	} 
	for (auto& p : target->moveableList)
	{
		positionList->x[length] = p->x;
		positionList->y[length] = p->y;
		length++;
	}
	positionList->length = length; 
}