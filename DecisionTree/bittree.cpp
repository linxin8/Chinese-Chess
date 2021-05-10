#include"bittree.h"
BitTree::HashTable BitTree::hashTable[hashTableMask + 1]{};
int BitTree::historyTable[2][256][256]{}; 