#include "node.h"
#include<exception>
#include<cassert>
#include<iostream>
#include<cstdlib>

ChessPosition Node::board[10][9];

Node::Node(int state[10][9])
{
	loadBoard(state);
}


Node::~Node()
{ 
}

void Node::loadBoard(int state[10][9])
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			auto country = state[i][j] / 10 == 1 ? Black : Red;
			auto type = ChessType(state[i][j] % 10);
			board[i][j] = { j,i,Chess::newInstance(this, j, i, type, country) };
		}
	}
}

int Node::getEstimatedValue(const std::vector<Chess*>& aiCandidate, const std::vector<Chess*>& playerCandidate)
{
	//None,Pawn,Cannon,Rook,Knight,Elephant,Guard,King
	constexpr static int chessValue[8] = { 0,200,1500,2500,1500,500,500,/*100000000*/ 0 };
	constexpr static int moveValue[8] = { 0,0,0,5,5,0,0,0 };
	constexpr static int positionValue[2][8][10][9] =
	{
	{//black chess
		{// None
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
		},
		{// Pawn
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{  20,   0,  20,   0, 400,   0,  20,   0,  20},
			{  30,   0,  80,   0,  80,   0,  80,   0,  30},
			{  40,  60, 100, 130, 180, 130, 100,  60,  40},
			{  60,  90, 130, 180, 300, 180, 130, 100,  60},
			{  90, 130, 180, 240, 310, 240, 180, 130, 100},
			{ 130, 180, 240, 310, 390, 310, 240, 180, 130},
			{ 180, 240, 310, 390, 480, 390, 310, 240, 180},
		},
		{// Cannon
			{  20,  30,  30,  40,  40,  40,  30,  30,  20},
			{  20,  30,  30,  40,  40,  40,  30,  30,  20},
			{  20,  30,  30,  40,  80,  40,  30,  30,  20},
			{  20,  30,  30,  40,  40,  40,  30,  30,  20},
			{  20,  30,  30,  40,  40,  40,  30,  30,  20},
			{  20,  30,  30,  40,  40,  40,  30,  30,  20},
			{  20,  30,  30,  40,  40,  40,  30,  30,  20},
			{  20,  30,  30,  40,  40,  40,  30,  30,  20},
			{  20,  30,  30,  40,  40,  40,  30,  30,  20},
			{ 100, 100, 100,  80,  80,  80, 100, 100, 100},
		},
		{// Rook
			{-50,  20,  20, 100, 100, 100,  20,  20, -50},
			{  20,  30,  30, 100, 100, 100,  30,  30,  20},
			{  20,  30,  40, 100, 100, 100,  40,  30,  20},
			{  20,  30,  40, 100, 100, 100,  40,  30,  20},
			{  20, 100, 100, 100, 100, 100, 100, 100,  20},
			{  20, 100, 100, 100, 100, 100, 100, 100,  20},
			{  20,  30,  40, 100, 100, 100,  40,  30,  20},
			{  20,  30,  40, 100, 100, 100,  40,  30,  20},
			{  20,  30,  30, 100, 100, 100,  30,  30,  20},
			{ 100, 100, 100, 100, 100, 100, 100, 100, 100},
		},
		{// Knight
			{  10, -50,  10,  10,  10,  10,  10,-50,  10},
			{  10,  30,  30,  30,  30,  30,  30,  30,  10},
			{  10,  30, 100,  40,  40,  40, 100,  30,  10},
			{  10,  30,  40,  50,  50,  50,  40,  30,  10},
			{  10,  30,  40,  50,  50,  50,  40,  30,  10},
			{  10,  30,  40,  50,  50,  50,  40,  30,  10},
			{  10,  30,  40,  50,  50,  50,  40,  30,  10},
			{  10,  30,  40,  50,  50,  50,  40,  30,  10},
			{  10,  30,  30,  30,  30,  30,  30,  30,  10},
			{  10,  10,  10,  10,  10,  10,  10,  10,  10},
		},
		{// Elephant
			{   0,   0,  40,   0,   0,   0,  40,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{  20,   0,   0,   0, 100,   0,   0,   0,  20},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,  40,   0,   0,   0,  40,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
		},
		{// Guard
			{   0,   0,   0, 100,   0, 100,   0,   0,   0},
			{   0,   0,   0,   0, 100,   0,   0,   0,   0},
			{   0,   0,   0,  50,   0,  50,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
		},
		{// King 
			{   0,   0,   0, 150, 200, 150,   0,   0,   0},
			{   0,   0,   0, 100, 150, 100,   0,   0,   0},
			{   0,   0,   0,  50,  50,  50,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
		},
	},
	{//player chess
		{// None
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
		},
		{// Pawn
			{ 180, 240, 310, 390, 480, 390, 310, 240, 180},
			{ 130, 180, 240, 310, 390, 310, 240, 180, 130},
			{  90, 130, 180, 240, 310, 240, 180, 130, 100},
			{  60,  90, 130, 180, 300, 180, 130, 100,  60},
			{  40,  60, 100, 130, 180, 130, 100,  60,  40},
			{  20,  30,  40,  50,  80,  50,  40,  30,  20},
			{  20,   0,  20,   0, 400,   0,  20,   0,  20},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
		},
		{// Cannon

			{ 100, 100, 100,  80,  80,  80, 100, 100, 100},
			{  20,  30,  30,  40,  40,  40,  30,  30,  20},
			{  20,  30,  30,  40,  40,  40,  30,  30,  20},
			{  20,  30,  30,  40,  40,  40,  30,  30,  20},
			{  20,  30,  30,  40,  40,  40,  30,  30,  20},
			{  20,  30,  30,  40,  40,  40,  30,  30,  20},
			{  20,  30,  30,  40,  40,  40,  30,  30,  20},
			{  20,  30,  30,  40,  80,  40,  30,  30,  20},
			{  20,  30,  30,  40,  40,  40,  30,  30,  20},
			{  20,  30,  30,  40,  40,  40,  30,  30,  20},
		},
		{// Rook
			{ 100,  20,  20, 100, 100, 100,  20,  20, 100},
			{  20,  30,  30, 100, 100, 100,  30,  30,  20},
			{  20,  30,  40, 100, 100, 100,  40,  30,  20},
			{  20,  30,  40, 100, 100, 100,  40,  30,  20},
			{  20, 100, 100, 100, 100, 100, 100, 100,  20},
			{  20, 100, 100, 100, 100, 100, 100, 100,  20},
			{  20,  30,  40, 100, 100, 100,  40,  30,  20},
			{  20,  30,  40, 100, 100, 100,  40,  30,  20},
			{  20,  30,  30, 100, 100, 100,  30,  30,  20},
			{ -50, 100, 100, 100, 100, 100, 100, 100, -50},
		},
		{// Knight
			{  10,  10,  10,  10,  10,  10,  10,  10,  10},
			{  10,  30,  30,  30,  30,  30,  30,  30,  10},
			{  10,  30,  40,  40,  40,  40,  40,  30,  10},
			{  10,  30,  40,  50,  50,  50,  40,  30,  10},
			{  10,  30,  40,  50,  50,  50,  40,  30,  10},
			{  10,  30,  40,  50,  50,  50,  40,  30,  10},
			{  10,  30,  40,  50,  50,  50,  40,  30,  10},
			{  10,  30, 100,  50,  50,  50, 100,  30,  10},
			{  10,  30,  30,  30,  30,  30,  30,  30,  10},
			{  10, -50,  10,  10,  10,  10,  10, -50,  10},
		},
		{// Elephant
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,  40,   0,   0,   0,  40,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{  20,   0,   0,   0, 100,   0,   0,   0,  20},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,  40,   0,   0,   0,  40,   0,   0},
		},
		{// Guard
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,  50,   0,  50,   0,   0,   0},
			{   0,   0,   0,   0, 100,   0,   0,   0,   0},
			{   0,   0,   0, 100,   0, 100,   0,   0,   0},
		},
		{// King 
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,   0,   0,   0,   0,   0,   0},
			{   0,   0,   0,  50,  50,  50,   0,   0,   0},
			{   0,   0,   0, 100, 150, 100,   0,   0,   0},
			{   0,   0,   0, 150, 200, 150,   0,   0,   0},
		},
	},
	};
	int totalValue = 0;
	int defendMap[10][9]{};
	SimpleList<ChessTarget*, 100> aiTarget;
	SimpleList<ChessTarget*, 100> playerTarget;
	for (auto& chess : aiCandidate)
	{
		auto type = chess->type;
		if (type != None)
		{
			aiTarget.push_back(chess->getTarget());
			totalValue += positionValue[Black][type][chess->getY()][chess->getX()];
			totalValue += chessValue[type];
		}
	}
	for (auto& chess : playerCandidate)
	{
		auto type = chess->type;
		if (type != None)
		{
			playerTarget.push_back(chess->getTarget());
			totalValue -= positionValue[Red][type][chess->getY()][chess->getX()];
			totalValue -= chessValue[type];
		}
	}

	for (auto& target : aiTarget)
	{
		for (int i = 0; i < target->defendableList.length; i++)
		{
			auto& p = target->defendableList.position[i];
			defendMap[p->y][p->x] += 1;
		}
	}

	for (auto& target : playerTarget)
	{
		for (int i = 0; i < target->defendableList.length; i++)
		{
			auto& p = target->defendableList.position[i];
			defendMap[p->y][p->x] += 1;
		}
	}
	for (auto& target : aiTarget)
	{ 
		for (int i = 0; i < target->assaultableList.length; i++)
		{
			auto& p = target->assaultableList.position[i];
			auto v = defendMap[p->y][p->x];
			if (v == 0)
			{
				totalValue += chessValue[p->chess->type] >> 1;
			}
			else
			{
				totalValue += chessValue[p->chess->type] / (2 + v);
			} 
		} 
	}
	for (auto& target : playerTarget)
	{ 
		for (int i = 0; i < target->assaultableList.length; i++)
		{
			auto& p = target->assaultableList.position[i]; 
			auto v = defendMap[p->y][p->x];
			if (v == 0)
			{
				totalValue -= chessValue[p->chess->type] >> 1;
			}
			else
			{
				totalValue -= chessValue[p->chess->type] / (2 + v);
			}
		} 
	}  
	//for (auto chess : aiCandidate)
	//{
	//	auto type = chess->type;
	//	if (type != None)
	//	{ 
	//		int value = 0;
	//		auto target = chess->getTarget();
	//		auto& assaultableList = target->assaultableList;
	//		for (int i = 0; i < assaultableList.length; i++)
	//		{
	//			value += chessValue[assaultableList.position[i]->chess->type] >> 2;
	//		}
	//		value += positionValue[Black][type][chess->getY()][chess->getX()];
	//		value += chessValue[type];
	//		totalValue += value; 
	//	}
	//} 
	//for (auto chess : playerCandidate)
	//{
	//	auto type = chess->type;
	//	if (type != None)
	//	{ 
	//		int value = 0;
	//		auto target = chess->getTarget();
	//		auto& assaultableList = target->assaultableList;
	//		for (int i = 0; i < assaultableList.length; i++)
	//		{
	//			value += chessValue[assaultableList.position[i]->chess->type] >> 2;
	//		}
	//		value += positionValue[Black][type][chess->getY()][chess->getX()];
	//		value += chessValue[type];
	//		totalValue -= value; 
	//	}
	//} 
	return totalValue;
}

Position Node::getStep(const Node & des, const Position & from) const
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			auto chess = board[i][j].chess;
			auto desChess = des.board[i][j].chess;
			if (*chess != *desChess && from.x != j && from.y != i)
			{
				return { j,i };
			}
		}
	}
	throw new std::exception("des position not found");
}
//
//std::list<ChessPosition*> Node::getTargetChessPosition(ChessPosition* cp)
//{  
//	auto chess = cp->chess;
//	if (chess->getType() == None)
//	{
//		return {};
//	}
//	chess->updateTarget();
//	std::list<ChessPosition*> ret;
//	ret.splice(ret.end(),chess->getAssaultablePostion());
//	ret.splice(ret.end(), chess->getMoveablePosition());
//	//for (auto& p : chess->getMoveablePosition())
//	//{
//	//	ret.push_back(p);
//	//} 
//	//for (auto& p : chess->getAssaultablePostion())
//	//{
//	//	ret.push_back(p);
//	//}
//	return std::move(ret);
//}

void Node::debugPrintBoard() const
{
	std::cout << "---debug print node board---\n";
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			auto chess = board[i][j].chess;
			if (chess->isNoneTypeChess())
			{
				std::cout << "00 ";
			}
			else
			{
				std::cout << chess->type + chess->country * 10 + 10 << ' '; 
			}
		}
		std::cout << "\n";
	}
	std::cout << std::endl;
}
