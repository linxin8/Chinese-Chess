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
	auto& assaultableList = target->assaultableList;
	for (int i = 0; i < assaultableList.length; i++)
	{
		positionList->x[length] = assaultableList.position[i]->x;
		positionList->y[length] = assaultableList.position[i]->y;
		length++;
	}	
	auto& moveableList = target->moveableList;
	for (int i = 0; i < moveableList.length; i++)
	{
		positionList->x[length] = moveableList.position[i]->x;
		positionList->y[length] = moveableList.position[i]->y;
		length++;
	}
	positionList->length = length; 
}