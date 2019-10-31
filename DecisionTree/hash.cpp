#include"hash.h"
#include"chess.h"
HashKeyType getChessPositionHash(ChessPosition* cp)
{
	auto& chess = cp->chess;
	if (chess->type == None)
	{
		return 0;
	}
	return chessPositionHashTable[chess->country][chess->type - 1][cp->y][cp->x];
}