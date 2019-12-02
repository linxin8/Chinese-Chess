#include "chess.h"
#include "node.h"
#include<cassert>
#include<algorithm>

 
//#define positonMap(x, y)  ((y << 3) + y + x) // y*9+x

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

void Chess::onBoardChessMove(int x1, int y1, int x2, int y2)
{ 
	auto& partialView = partialViewList.back(); 
	if (partialView.test(x1,y1) ||
		partialView.test(x2, y2))
	{
		updateTarget();
	}
}

void Chess::onBoardChessUndoMove(int x1, int y1, int x2, int y2)
{
	auto& partialView = partialViewList.back();
	if (partialView.test(x1, y1) ||
		partialView.test(x2, y2))
	{
		targetList.length--;
		target = &targetList.back(); 
		partialViewList.pop_back();
	}
}

void Chess::updateTarget()
{ 
	recomputeTargetPositionInfo(); 
	//sortAssaultableTarget();
}
void Chess::sortAssaultableTarget()
{
//	constexpr int chessPriority[8] = { 0, 1, 3, 4, 3, 2, 2, 5}; 
//	std::sort(target->assaultableList.position, target->assaultableList.position + target->assaultableList.length,
//		[&chessPriority](ChessPosition* a, ChessPosition*  b) {return chessPriority[a->chess->type] > chessPriority[b->chess->type]; });
//
} 

const Position PawnChess::relativePosition[4]{ {1,0},{-1,0},{0,-1},{0,1} };
void PawnChess::recomputeTargetPositionInfo()
{ 
	//constexpr const Position relativePosition[4]{ {1,0},{-1,0},{0,-1},{0,1} };
	//int x;
	//int y;
	//for (auto& i : relativePosition)
	//{
	//	if (country == Black)
	//	{
	//		if (i.y < 0 || (i.x != 0 && py <= 4))
	//		{
	//			continue;
	//		}
	//	}
	//	else
	//	{
	//		if (i.y > 0 || (i.x != 0 && py >= 5))
	//		{
	//			continue;
	//		}
	//	}
	//	x = px + i.x;
	//	y = py + i.y;
	//	if (isInRange(x, 0, 9) && isInRange(y, 0, 10))
	//	{
	//		auto cp = &node->board[y][x];
	//		if (cp->chess->type==None)
	//		{
	//			target->moveableList.push_back(cp);
	//		}
	//		else if (cp->chess->country!=country)
	//		{
	//			target->assaultableList.push_back(cp);
	//		}
	//		else
	//		{
	//			target->defendableList.push_back(cp);
	//		}
	//	}
	//}  
	partialViewList.length++;
	auto& partialView = partialViewList.back(); 
	partialView.reset();
	partialView.set(px,py);
	target = targetList.end();
	targetList.length++;
	target->clear();
	auto& candidate = country == Black ? cp->pawnCandidate.blackCandidate : cp->pawnCandidate.redCandidate;
	for (auto& i : candidate)
	{
		partialView.set(i->x, i->y);
		if (i->chess->type == None)
		{
			target->moveableList.push_back(i);  
		}
		else if (i->chess->country != country)
		{
			target->assaultableList.push_back(i); 
		}
		else
		{
			target->defendableList.push_back(i); 
		}
	} 
}

const Position CannonChess::relativePosition[4]{ {1,0},{-1,0},{0,-1},{0,1} };
void CannonChess::recomputeTargetPositionInfo()
{
	//constexpr const Position relativePosition[4] = { {1,0},{-1,0},{0,-1},{0,1} };
	//for (auto& v : relativePosition)
	//{
	//	for (int x = px + v.x, y = py + v.y; isInRange(x, 0, 9) && isInRange(y, 0, 10); x += v.x, y += v.y)
	//	{
	//		auto cp = &node->board[y][x];
	//		if (cp->chess->type == None)
	//		{
	//			target->moveableList.push_back(cp);
	//		}
	//		else
	//		{
	//			for (x += v.x, y += v.y; isInRange(x, 0, 9) && isInRange(y, 0, 10); x += v.x, y += v.y)
	//			{
	//				cp = &node->board[y][x];
	//				if (cp->chess->type != None)
	//				{ 
	//					if (cp->chess->country != country)
	//					{
	//						target->assaultableList.push_back(cp);
	//						break;
	//					}
	//					else
	//					{
	//						target->defendableList.push_back(cp);
	//						break;
	//					}
	//				}
	//			}
	//			break;
	//		}
	//	}
	//}  
	partialViewList.length++;
	auto& partialView = partialViewList.back();
	partialView.reset();
	partialView.set(px, py);
	target = targetList.end();
	targetList.length++;
	target->clear();
	ChessPosition* temp;
#define TEMP_MARCO(direction)\
	for (temp = cp->direction; temp != nullptr; temp = temp->direction)\
	{\
		partialView.set(temp->x, temp->y);\
		if (temp->chess->type == None)\
		{\
			target->moveableList.push_back(temp);\
		}\
		else\
		{\
			for (temp = temp->direction; temp != nullptr; temp = temp->direction)\
			{\
				partialView.set(temp->x, temp->y);\
				if (temp->chess->type != None)\
				{\
					if (temp->chess->country != country)\
					{\
						target->assaultableList.push_back(temp);\
						break;\
					}\
					else\
					{\
						target->defendableList.push_back(temp);\
						break;\
					}\
				}\
			}\
			break;\
		}\
	}
	TEMP_MARCO(up);
	TEMP_MARCO(down);
	TEMP_MARCO(left);
	TEMP_MARCO(right);
#undef TEMP_MARCO
}

const Position RookChess::relativePosition[4]{ {1,0},{-1,0},{0,-1},{0,1} };
void RookChess::recomputeTargetPositionInfo()
{ 
	//constexpr const Position relativePosition[4] = { {1,0},{-1,0},{0,-1},{0,1} };
	//for (auto& v : relativePosition)
	//{
	//	for (int x = px + v.x, y = py + v.y; isInRange(x, 0, 9) && isInRange(y, 0, 10); x += v.x, y += v.y)
	//	{
	//		auto cp = &node->board[y][x];
	//		if (cp->chess->type==None)
	//		{
	//			target->moveableList.push_back(cp);
	//		}
	//		else if (cp->chess->country!=country)
	//		{
	//			target->assaultableList.push_back(cp);
	//			break;
	//		}
	//		else
	//		{
	//			target->defendableList.push_back(cp);
	//			break;
	//		}
	//	}
	//}
	partialViewList.length++;
	auto& partialView = partialViewList.back();
	partialView.reset();
	partialView.set(px, py);
	target = targetList.end();
	targetList.length++;
	target->clear();
	ChessPosition* temp; 
#define  TEMP_MARCO(direction) \
	for (temp = cp->direction; temp != nullptr; temp = temp->direction) \
	{\
		partialView.set(temp->x, temp->y);\
		if (temp->chess->type == None)\
		{\
			target->moveableList.push_back(temp);\
		}\
		else if (temp->chess->country != country)\
		{\
			target->assaultableList.push_back(temp);\
			break;\
		}\
		else\
		{\
			target->defendableList.push_back(temp);\
			break;\
		}\
	}
	TEMP_MARCO(up);
	TEMP_MARCO(down);
	TEMP_MARCO(left);
	TEMP_MARCO(right);
#undef TEMP_MARCO
} 

const Position KnightChess::relativePosition[8]{ {2,1},{2,-1},{-2,1},{-2,-1},{1,2},{1,-2},{-1,2},{-1,-2} };
void KnightChess::recomputeTargetPositionInfo()
{
	//constexpr const Position relativePosition[8] = { {2,1},{2,-1},{-2,1},{-2,-1},{1,2},{1,-2},{-1,2},{-1,-2} };
	//constexpr const Position hv[8] = { {1,0},{1,0},{-1,0},{-1,0},{0,1},{0,-1},{0,1},{0,-1} };
	//for (int i = 0; i < 8; i++)
	//{
	//	int x = px + relativePosition[i].x;
	//	int y = py + relativePosition[i].y;
	//	if (isInRange(x, 0, 9) && isInRange(y, 0, 10))
	//	{
	//		if (node->isNoneChess(px + hv[i].x, py + hv[i].y))
	//		{ 
	//			auto cp = &node->board[y][x];
	//			if (cp->chess->type == None)
	//			{
	//				target->moveableList.push_back(cp);
	//			}
	//			else if (cp->chess->country!=country)
	//			{
	//				target->assaultableList.push_back(cp);
	//			}
	//			else
	//			{
	//				target->defendableList.push_back(cp);
	//			}
	//		}
	//	}
	//} 
	partialViewList.length++;
	auto& partialView = partialViewList.back();
	partialView.reset();
	partialView.set(px, py);
	target = targetList.end();
	targetList.length++;
	target->clear();
	for (int i = 0; i < cp->knightCandidate.candidate.length; i++)
	{
		auto& candidate = cp->knightCandidate.candidate[i];
		auto& handicap = cp->knightCandidate.handicap[i];
		partialView.set(handicap->x, handicap->y);
		if (handicap->chess->type == None)
		{
			partialView.set(candidate->x, candidate->y);
			if (candidate->chess->type == None)
			{
				target->moveableList.push_back(candidate);
			}
			else if (candidate->chess->country != country)
			{
				target->assaultableList.push_back(candidate);
			}
			else
			{
				target->defendableList.push_back(candidate);
			}
		}
	}
}

const Position ElephantChess::relativePosition[4]{ {2,2},{2,-2},{-2,2},{-2,-2} };
void ElephantChess::recomputeTargetPositionInfo()
{
	//constexpr const Position relativePosition[4] = { {2,2},{2,-2},{-2,2},{-2,-2} };
	//constexpr const Position hv[4] = { {1,1},{1,-1},{-1,1},{-1,-1} };
	//for (int i = 0; i < 4; i++)
	//{
	//	int x = px + relativePosition[i].x;
	//	int y = py + relativePosition[i].y;
	//	int yMin = country == Black ? 0 : 5;
	//	int yMax = yMin + 5;
	//	if (isInRange(x, 0, 9) && isInRange(y, yMin, yMax))
	//	{
	//		if (node->isNoneChess(px + hv[i].x, py + hv[i].y))
	//		{
	//			auto cp = &node->board[y][x];
	//			if (cp->chess->type == None)
	//			{
	//				target->moveableList.push_back(cp);
	//			}
	//			else if (cp->chess->country!=country)
	//			{
	//				target->assaultableList.push_back(cp);
	//			}
	//			else
	//			{
	//				target->defendableList.push_back(cp);
	//			}
	//		}
	//	}
	//}
	partialViewList.length++;
	auto& partialView = partialViewList.back();
	partialView.reset();
	partialView.set(px, py);
	target = targetList.end();
	targetList.length++;
	target->clear();
	for (int i = 0; i < cp->elephantCandidate.candidate.length; i++)
	{
		auto& candidate = cp->elephantCandidate.candidate[i];
		auto& handicap = cp->elephantCandidate.handicap[i];
		partialView.set(handicap->x, handicap->y);
		if (handicap->chess->type == None)
		{
			partialView.set(candidate->x, candidate->y);
			if (candidate->chess->type == None)
			{
				target->moveableList.push_back(candidate);
			}
			else if (candidate->chess->country != country)
			{
				target->assaultableList.push_back(candidate);
			}
			else
			{
				target->defendableList.push_back(candidate);
			}
		}
	}
}

const Position GuardChess::relativePosition[4]{ {1,1},{1,-1},{-1,1},{-1,-1} };
void GuardChess::recomputeTargetPositionInfo()
{
	//constexpr const Position relativePosition[4] = { {1,1},{1,-1},{-1,1},{-1,-1} };
	//for (int i = 0; i < 4; i++)
	//{
	//	int x = px + relativePosition[i].x;
	//	int y = py + relativePosition[i].y;
	//	int yMin = country == Black ? 0 : 7;
	//	int yMax = yMin + 3;
	//	if (isInRange(x, 3, 6) && isInRange(y, yMin, yMax))
	//	{
	//		auto cp = &node->board[y][x];
	//		if (cp->chess->type == None)
	//		{
	//			target->moveableList.push_back(cp);
	//		}
	//		else if (cp->chess->country!=country)
	//		{
	//			target->assaultableList.push_back(cp);
	//		}
	//		else
	//		{
	//			target->defendableList.push_back(cp);
	//		}
	//	}
	//} 
	partialViewList.length++;
	auto& partialView = partialViewList.back();
	partialView.reset();
	partialView.set(px, py);
	target = targetList.end();
	targetList.length++;
	target->clear();
	for (auto& i : cp->guardCandidate)
	{
		partialView.set(i->x, i->y);
		if (i->chess->type == None)
		{
			target->moveableList.push_back(i);
		}
		else if (i->chess->country != country)
		{
			target->assaultableList.push_back(i);
		}
		else
		{
			target->defendableList.push_back(i);
		}
	}
}

const Position KingChess::relativePosition[4]{ {1,1},{1,-1},{-1,1},{-1,-1} };
void KingChess::recomputeTargetPositionInfo()
{
	//constexpr const Position relativePosition[4] = { {1,0},{-1,0},{0,-1},{0,1} };
	//for (int i = 0; i < 4; i++)
	//{
	//	int x = px + relativePosition[i].x;
	//	int y = py + relativePosition[i].y;
	//	int yMin = country == Black ? 0 : 7;
	//	int yMax = yMin + 3;
	//	if (isInRange(x, 3, 6) && isInRange(y, yMin, yMax))
	//	{
	//		auto cp = &node->board[y][x];
	//		if (cp->chess->type == None)
	//		{
	//			target->moveableList.push_back(cp);
	//		}
	//		else if (cp->chess->country!=country)
	//		{
	//			target->assaultableList.push_back(cp);
	//		}
	//		else
	//		{
	//			target->defendableList.push_back(cp);
	//		}
	//	}
	//}
	partialViewList.length++;
	auto& partialView = partialViewList.back();
	partialView.reset();
	partialView.set(px, py);
	target = targetList.end();
	targetList.length++;
	target->clear();
	for (auto& i : cp->kingCandidate)
	{
		partialView.set(i->x, i->y);
		if (i->chess->type == None)
		{
			target->moveableList.push_back(i);
		}
		else if (i->chess->country != country)
		{
			target->assaultableList.push_back(i);
		}
		else
		{
			target->defendableList.push_back(i);
		}
	}
	ChessPosition* temp;
	if (country == Red)
	{ 
		for (temp = cp->up; temp != nullptr; temp = temp->up)
		{
			partialView.set(temp->x, temp->y);
			auto& t = temp->chess->type;
			if (t == King)
			{
				target->assaultableList.push_back(temp);
				break;
			}
			if (t != None)
			{
				break;
			}
		}
	}
	else
	{
		for (temp = cp->down; temp != nullptr; temp = temp->down)
		{
			partialView.set(temp->x, temp->y);
			auto& t = temp->chess->type;
			if (t == King)
			{
				target->assaultableList.push_back(temp);
				break;
			}
			if (t != None)
			{
				break;
			}
		}
	}

}