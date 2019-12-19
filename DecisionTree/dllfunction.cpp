#include <iostream>
#include "dllfunction.h"
//#include "tree.h"
#include"bittree.h"
 
 
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
	//DLL_init(); 
	//Tree tree(map);
	//*d = *tree.deepSearch();
	DLL_init();
	BitTree tree(map);
	tree.startSerach(d->fromX, d->fromY, d->desX, d->desY);
}

void getAccessiblePosition(int map[10][9], int x, int y, PositionList* positionList)
{
	DLL_init();
	//Tree tree(map);
	//auto target = tree.getTarget(x, y);   
	BitTree tree(map); 
	//std::cout << tree.getEstimatedValue(Black) << '\n';
	//tree.printBoard();
	auto target = tree.getTarget(x, y);
	int length = 0; 
	for (auto& p : target.assaultableList)
	{ 
		auto index = BitBoard::get_index(p);
		positionList->x[length] = (int)(index % 16 - 3);
		positionList->y[length] = (int)(index / 16 - 3);
		length++;
	} 
	for (auto& p : target.moveableList)
	{
		auto index = BitBoard::get_index(p);
		positionList->x[length] = (int)(index % 16 - 3);
		positionList->y[length] = (int)(index / 16 - 3);
		length++;
	}
	positionList->length = length; 
}