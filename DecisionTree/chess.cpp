#include "chess.h"
#include "node.h"
#include<cassert>
#include<algorithm>


Chess* Chess::newInstance(Node* node, int x, int y, ChessType type, ChessCountry country)
{
	switch (type)
	{
	case None:
		return new NoneChess(node, x, y, country); 
	case Pawn:
		return new PawnChess(node, x, y, country); 
	case Cannon:
		return new CannonChess(node, x, y, country);
	case Rook:
		return new RookChess(node, x, y, country);
	case Knight:
		return new KnightChess(node, x, y, country);
	case Elephant:
		return new ElephantChess(node, x, y, country);
	case Guard:
		return new GuardChess(node, x, y, country);
	case King:
		return new KingChess(node, x, y, country);
	default:
		throw std::exception("error case");
	}
	return nullptr;
}

void Chess::updateTarget()
{  
	target->clear();
	recomputeTargetPositionInfo();
	sortAssaultableTarget();
}
void Chess::sortAssaultableTarget()
{
	constexpr int chessPriority[8] = { 0, 1, 3, 4, 3, 2, 2, 5}; 
	std::sort(target->assaultableList.position, target->assaultableList.position + target->assaultableList.length,
		[&chessPriority](ChessPosition* a, ChessPosition*  b) {return chessPriority[a->chess->type] > chessPriority[b->chess->type]; });
}

const Position PawnChess::relativePosition[4]{ {1,0},{-1,0},{0,-1},{0,1} };
void PawnChess::recomputeTargetPositionInfo()
{ 
	constexpr const Position relativePosition[4]{ {1,0},{-1,0},{0,-1},{0,1} };
	int x;
	int y;
	for (auto& i : relativePosition)
	{
		if (country == Black)
		{
			if (i.y < 0 || (i.x != 0 && py <= 4))
			{
				continue;
			}
		}
		else
		{
			if (i.y > 0 || (i.x != 0 && py >= 5))
			{
				continue;
			}
		}
		x = px + i.x;
		y = py + i.y;
		if (isInRange(x, 0, 9) && isInRange(y, 0, 10))
		{
			auto cp = node->getChessPositoin(x, y);
			if (cp->chess->type==None)
			{
				target->moveableList.push_back(cp);
			}
			else if (cp->chess->country!=country)
			{
				target->assaultableList.push_back(cp);
			}
			else
			{
				target->defendableList.push_back(cp);
			}
		}
	}
}

const Position CannonChess::relativePosition[4]{ {1,0},{-1,0},{0,-1},{0,1} };
void CannonChess::recomputeTargetPositionInfo()
{
	constexpr const Position relativePosition[4] = { {1,0},{-1,0},{0,-1},{0,1} };
	for (auto& v : relativePosition)
	{
		for (int x = px + v.x, y = py + v.y; isInRange(x, 0, 9) && isInRange(y, 0, 10); x += v.x, y += v.y)
		{
			auto cp = node->getChessPositoin(x, y);
			if (cp->chess->type == None)
			{
				target->moveableList.push_back(cp);
			}
			else
			{
				for (x += v.x, y += v.y; isInRange(x, 0, 9) && isInRange(y, 0, 10); x += v.x, y += v.y)
				{
					cp = node->getChessPositoin(x, y);
					if (cp->chess->type != None)
					{ 
						if (cp->chess->country != country)
						{
							target->assaultableList.push_back(cp);
							break;
						}
						else
						{
							target->defendableList.push_back(cp);
							break;
						}
					}
				}
				break;
			}
		}
	}
}

const Position RookChess::relativePosition[4]{ {1,0},{-1,0},{0,-1},{0,1} };
void RookChess::recomputeTargetPositionInfo()
{ 
	constexpr const Position relativePosition[4] = { {1,0},{-1,0},{0,-1},{0,1} };
	for (auto& v : relativePosition)
	{
		for (int x = px + v.x, y = py + v.y; isInRange(x, 0, 9) && isInRange(y, 0, 10); x += v.x, y += v.y)
		{
			auto cp = node->getChessPositoin(x, y);
			if (cp->chess->type==None)
			{
				target->moveableList.push_back(cp);
			}
			else if (cp->chess->country!=country)
			{
				target->assaultableList.push_back(cp);
				break;
			}
			else
			{
				target->defendableList.push_back(cp);
				break;
			}
		}
	}
} 

const Position KnightChess::relativePosition[8]{ {2,1},{2,-1},{-2,1},{-2,-1},{1,2},{1,-2},{-1,2},{-1,-2} };
void KnightChess::recomputeTargetPositionInfo()
{
	constexpr const Position relativePosition[8] = { {2,1},{2,-1},{-2,1},{-2,-1},{1,2},{1,-2},{-1,2},{-1,-2} };
	constexpr const Position hv[8] = { {1,0},{1,0},{-1,0},{-1,0},{0,1},{0,-1},{0,1},{0,-1} };
	for (int i = 0; i < 8; i++)
	{
		int x = px + relativePosition[i].x;
		int y = py + relativePosition[i].y;
		if (isInRange(x, 0, 9) && isInRange(y, 0, 10))
		{
			if (node->isNoneChess(px + hv[i].x, py + hv[i].y))
			{
				auto cp = node->getChessPositoin(x, y);
				if (cp->chess->type == None)
				{
					target->moveableList.push_back(cp);
				}
				else if (cp->chess->country!=country)
				{
					target->assaultableList.push_back(cp);
				}
				else
				{
					target->defendableList.push_back(cp);
				}
			}
		}
	}
}

const Position ElephantChess::relativePosition[4]{ {2,2},{2,-2},{-2,2},{-2,-2} };
void ElephantChess::recomputeTargetPositionInfo()
{
	constexpr const Position relativePosition[4] = { {2,2},{2,-2},{-2,2},{-2,-2} };
	constexpr const Position hv[4] = { {1,1},{1,-1},{-1,1},{-1,-1} };
	for (int i = 0; i < 4; i++)
	{
		int x = px + relativePosition[i].x;
		int y = py + relativePosition[i].y;
		int yMin = country == Black ? 0 : 5;
		int yMax = yMin + 5;
		if (isInRange(x, 0, 9) && isInRange(y, yMin, yMax))
		{
			if (node->isNoneChess(px + hv[i].x, py + hv[i].y))
			{
				auto cp = node->getChessPositoin(x, y);
				if (cp->chess->type == None)
				{
					target->moveableList.push_back(cp);
				}
				else if (cp->chess->country!=country)
				{
					target->assaultableList.push_back(cp);
				}
				else
				{
					target->defendableList.push_back(cp);
				}
			}
		}
	}
}

const Position GuardChess::relativePosition[4]{ {1,1},{1,-1},{-1,1},{-1,-1} };
void GuardChess::recomputeTargetPositionInfo()
{
	constexpr const Position relativePosition[4] = { {1,1},{1,-1},{-1,1},{-1,-1} };
	for (int i = 0; i < 4; i++)
	{
		int x = px + relativePosition[i].x;
		int y = py + relativePosition[i].y;
		int yMin = country == Black ? 0 : 7;
		int yMax = yMin + 3;
		if (isInRange(x, 3, 6) && isInRange(y, yMin, yMax))
		{
			auto cp = node->getChessPositoin(x, y);
			if (cp->chess->type == None)
			{
				target->moveableList.push_back(cp);
			}
			else if (cp->chess->country!=country)
			{
				target->assaultableList.push_back(cp);
			}
			else
			{
				target->defendableList.push_back(cp);
			}
		}
	}
}

const Position KingChess::relativePosition[4]{ {1,1},{1,-1},{-1,1},{-1,-1} };
void KingChess::recomputeTargetPositionInfo()
{
	constexpr const Position relativePosition[4] = { {1,0},{-1,0},{0,-1},{0,1} };
	for (int i = 0; i < 4; i++)
	{
		int x = px + relativePosition[i].x;
		int y = py + relativePosition[i].y;
		int yMin = country == Black ? 0 : 7;
		int yMax = yMin + 3;
		if (isInRange(x, 3, 6) && isInRange(y, yMin, yMax))
		{
			auto cp = node->getChessPositoin(x, y);
			if (cp->chess->type == None)
			{
				target->moveableList.push_back(cp);
			}
			else if (cp->chess->country!=country)
			{
				target->assaultableList.push_back(cp);
			}
			else
			{
				target->defendableList.push_back(cp);
			}
		}
	}
}