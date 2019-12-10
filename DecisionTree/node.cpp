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
			board[i][j].up = i == 0 ? nullptr : &board[i - 1][j];
			board[i][j].down = i == 9 ? nullptr : &board[i + 1][j];
			board[i][j].left = j == 0 ? nullptr : &board[i][j - 1];
			board[i][j].right = j == 8 ? nullptr : &board[i][j + 1];
			board[i][j].chess->cp = &board[i][j]; 
			initChessPosistion(j, i);
		}
	}
}

void Node::initChessPosistion(int x, int y)
{
	auto it = &board[y][x];
	//pawn 
	{
		if (it->left != nullptr)
		{
			if (y < 5)
			{
				it->pawnCandidate.redCandidate.push_back(it->left);
			}
			else
			{
				it->pawnCandidate.blackCandidate.push_back(it->left);
			}
		}
		if (it->right != nullptr)
		{
			if (y < 5)
			{
				it->pawnCandidate.redCandidate.push_back(it->right);
			}
			else
			{
				it->pawnCandidate.blackCandidate.push_back(it->right);
			}
		}
		if (it->up != nullptr)
		{
			it->pawnCandidate.redCandidate.push_back(it->up);
		}
		if (it->down != nullptr)
		{
			it->pawnCandidate.blackCandidate.push_back(it->down);
		}
	}
	//cannon
	{

	}
	//rook
	{

	}
	//knight
	{
		Position rv[8] = { {2,1},{2,-1},{-2,1},{-2,-1},{1,2},{1,-2},{-1,2},{-1,-2} };
		Position hrv[8] = { {1,0},{1,0},{-1,0},{-1,0},{0,1},{0,-1},{0,1},{0,-1} };
		for (int i = 0; i < 8; i++)
		{ 
			int ax = it->x + rv[i].x;
			int ay = it->y + rv[i].y;
			if (0 <= ax && ax < 9 && 0 <= ay && ay < 10)
			{
				it->knightCandidate.candidate.push_back(&board[ay][ax]);
				it->knightCandidate.handicap.push_back(&board[it->y + hrv[i].y][it->x + hrv[i].x]);
			}
		} 
	}
	//Elephant
	{
		Position hrv[4] = { {1,1},{1,-1},{-1,1},{-1,-1} };
		Position rv[4] = { {2,2},{2,-2},{-2,2},{-2,-2} };
		for (int i = 0; i < 4; i++)
		{
			int ax = it->x + rv[i].x;
			int ay = it->y + rv[i].y;
			if (0 <= ax && ax < 9 && 0 <= ay && ay < 10 &&
				((it->y < 5 && ay < 5) || it->y >= 5 && ay >= 5))
			{
				it->elephantCandidate.candidate.push_back(&board[ay][ax]);
				it->elephantCandidate.handicap.push_back(&board[it->y + hrv[i].y][it->x + hrv[i].x]);
			}
		}  
	}
	//Guard
	{
		Position rv[4] = { {1,1},{1,-1},{-1,1},{-1,-1} };
		for (auto& p : rv)
		{
			int ax = it->x + p.x;
			int ay = it->y + p.y;
			if (3 <= ax && ax < 6 &&
				((0 <= ay && ay < 3) || (7 <= ay && ay < 10))
				)
			{
				it->guardCandidate.push_back(&board[ay][ax]);
			}
		}
	}
	//king
	{
		Position rv[4] = { {1,0},{-1,0},{0,-1},{0,1} };
		for (auto& p : rv)
		{
			int ax = it->x + p.x;
			int ay = it->y + p.y;
			if (3 <= ax && ax < 6 &&
				((0 <= ay && ay < 3) || (7 <= ay && ay < 10))
				)
			{
				it->kingCandidate.push_back(&board[ay][ax]);
			}
		}
	}
}

//None,Pawn,Cannon,Rook,Knight,Elephant,Guard,King 
constexpr static int chessValue[8] = { 0,200,1500,3500,1500,800,800,/*100000000*/ 4000 };
constexpr static int chessConfilictValueSente[8] = { 0,300 / 4,1500 / 4,3500 / 4,1500 / 4,800 / 4,800 / 4, 2000 };
constexpr static int chessConfilictValueGote[8] = { 0,300 / 4,1500 / 4,3500 / 4,1500 / 4,800 / 4,800 / 4, 2000 };
//constexpr static int chessConfilictValueGote[8] = { 0,300 / 3,1500 / 3,3500 / 3,1500 / 3,800 / 3,800 / 3, 400 };
constexpr static int moveShiftValue[8] = { 0,0,1,2,2,1,1,0 };
static int positionValue[2][8][10][9] =
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
		{  20,   0,  20,   0, 300,   0,  20,   0,  20},
		{  30,   0, 100,   0,  80,   0, 100,   0,  30},
		{ 140, 160, 300, 330, 380, 330, 200, 160, 140},
		{ 160, 201, 232, 483, 404, 483, 232, 201, 160},
		{ 200, 231, 382, 543, 514, 543, 382, 231, 200},
		{ 230, 281, 342, 590, 590, 590, 342, 281, 230},
		{ 280, 341, 412, 590, 590, 590, 412, 341, 280},
	},
	{// Cannon
		{  20,  40,  30,  40,  40,  40,  30,  30,  20},
		{  40,  30,  30,  40,  40,  40,  30,  30,  20},
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
		{ -50,  50,  20, 100, 100, 100,  20,  50, -50},
		{  50,  30,  30, 100, 100, 100,  30,  30,  50},
		{  20,  30,  40, 100, 100, 100,  40,  30,  20},
		{  20,  30,  40, 100, 100, 100,  40,  30,  20},
		{  20, 100, 100, 100, 100, 100, 100, 100,  20},
		{  20, 100, 100, 100, 100, 100, 100, 100,  20},
		{  20,  30, 140, 100, 100, 100, 140,  30,  20},
		{  20,  30,  40, 100, 100, 100,  40,  30,  20},
		{  20,  30,  30, 100, 100, 100,  30,  30,  20},
		{  20,  30,  30, 100, 100, 100,  30,  30,  20},
	},
	{// Knight
		{  10, -50,  10,  10,  10,  10,  10, -50,  10},
		{  10,  30,  30,  30,-100,  30,  30,  30,  10},
		{  10,  30,  40,  40,  40,  40,  40,  30,  10},
		{  10,  30,  40,  50,  50,  50,  40,  30,  10},
		{  10,  30,  40,  50,  50,  50,  40,  30,  10},
		{ 110, 130, 140, 150, 150, 150, 140, 130, 110},
		{ 110, 130, 140, 150, 150, 150, 140, 130, 110},
		{ 110, 130, 140, 150, 150, 150, 140, 130, 110},
		{ 110, 130, 130, 130, 130, 130, 130, 130, 110},
		{ 110, 110, 110, 110, 110, 110, 110, 110, 110},
	},
	{// Elephant
		{   0,   0,  40,   0,   0,   0,  40,   0,   0},
		{   0,   0,   0,   0,   0,   0,   0,   0,   0},
		{  20,   0,   0,   0, 100,   0,   0,   0,  20},
		{   0,   0,   0,   0,   0,   0,   0,   0,   0},
		{   0,   0,  30,   0,   0,   0,  30,   0,   0},
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
		{   0,   0,   0, 350, 600, 350,   0,   0,   0},
		{   0,   0,   0, 100, 350, 100,   0,   0,   0},
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
	{// Cannon 
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
	{// Rook
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
	{// Knight
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
	{// Elephant
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
	{// Guard
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
	{// King 
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
},
};
 
struct __InitPositionValue
{
	__InitPositionValue()
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				for (int k = 0; k < 9; k++)
				{
					positionValue[1][i][j][k] = positionValue[0][i][9 - j][k];
				}
			}
		}
	}
}__initPositionValue;

int Node::getEstimatedValue(const std::vector<Chess*>& aiCandidate, const std::vector<Chess*>& playerCandidate,ChessCountry currentCountry)
{ 
	int totalValue = 0;
	char confilictMap[10][9]{}; 
	//char threatenMap[10][9]{};
	//SimpleList<ChessTarget*, 16> aiTarget;
	//SimpleList<ChessTarget*, 16> playerTarget;
	for (auto& chess : aiCandidate)
	{
		auto type = chess->type;
		if (type != None)
		{
			auto target = chess->getTarget();
			//aiTarget.push_back(target);
			for(auto& p : target->assaultableList) 
			{
				confilictMap[p->y][p->x] += 1;
				//threatenMap[p->y][p->x] += 1;
				//auto targetType = board[p->x][p->y].chess->type;
				//if (targetType != None)
				//{
				//	totalValue += chessValue[targetType] >> 5;
				//} 
			}
			for (auto& p : target->defendableList)
			{
				confilictMap[p->y][p->x] -= 1;
			}
			totalValue += positionValue[Black][type][chess->py][chess->px];
			totalValue += chessValue[type];
			totalValue += target->moveableList.length << moveShiftValue[type];
		}
	}
	for (auto& chess : playerCandidate)
	{
		auto type = chess->type;
		if (type != None)
		{
			auto target = chess->getTarget();
			//playerTarget.push_back(target);
			for (auto& p : target->assaultableList) 
			{ 
				confilictMap[p->y][p->x] += 1;
				//threatenMap[p->y][p->x] += 1;
				//auto targetType = board[p->x][p->y].chess->type;
				//if (targetType != None)
				//{
				//	totalValue -= chessValue[targetType] >> 5;
				//}
			}
			for (auto& p : target->defendableList)
			{ 
				confilictMap[p->y][p->x] -= 1;
			}
			totalValue -= positionValue[Red][type][chess->py][chess->px];
			totalValue -= chessValue[type];
			totalValue -= target->moveableList.length << moveShiftValue[type];
		}
	}

	//for (auto& target : aiTarget)
	//{
	//	for (int i = 0; i < target->assaultableList.length; i++)
	//	{
	//		auto& p = target->assaultableList.position[i]; 
	//		confilictMap[p->y][p->x] += 1; 
	//		//threatenMap[p->y][p->x] += 1;
	//	}
	//	for (int i = 0; i < target->defendableList.length; i++)
	//	{
	//		auto& p = target->defendableList.position[i];
	//		confilictMap[p->y][p->x] -= 1;
	//	}
	//}

	//for (auto& target : playerTarget)
	//{
	//	for (int i = 0; i < target->assaultableList.length; i++)
	//	{
	//		auto& p = target->assaultableList.position[i];
	//		confilictMap[p->y][p->x] += 1;
	//		//threatenMap[p->y][p->x] += 1;
	//	}
	//	for (int i = 0; i < target->defendableList.length; i++)
	//	{
	//		auto& p = target->defendableList.position[i];
	//		confilictMap[p->y][p->x] -= 1;
	//	}
	//} 
	if (currentCountry == Black)
	{
		for (auto&chess : aiCandidate)
		{//sente
			if (chess->type != None)
			{
				if (confilictMap[chess->py][chess->px] > 0)
				{
					totalValue -= chessConfilictValueSente[chess->type];
				}
				//if (threatenMap[y][x] > 0)
				//{
				//	totalValue -= chessValue[chess->type] / 8 * threatenMap[y][x];
				//}
			}
		}
		for (auto&chess : playerCandidate)
		{//gote
			if (chess->type != None)
			{
				if (confilictMap[chess->py][chess->px] > 0)
				{
					totalValue += chessConfilictValueGote[chess->type];
				}
				//if (threatenMap[y][x] > 0)
				//{
				//	totalValue += chessValue[chess->type] / 8 * threatenMap[y][x];
				//}
			}
		}
	}
	else
	{
		for (auto&chess : aiCandidate)
		{//gote
			if (chess->type != None)
			{
				if (confilictMap[chess->py][chess->px] > 0)
				{
					totalValue -= chessConfilictValueGote[chess->type];
				}
				//if (threatenMap[y][x] > 0)
				//{
				//	totalValue -= chessValue[chess->type] / 8 * threatenMap[y][x];
				//}
			}
		}
		for (auto&chess : playerCandidate)
		{//sente
			if (chess->type != None)
			{
				if (confilictMap[chess->py][chess->px] > 0)
				{
					totalValue += chessConfilictValueSente[chess->type];
				}
				//if (threatenMap[y][x] > 0)
				//{
				//	totalValue += chessValue[chess->type] / 8 * threatenMap[y][x];
				//}
			}
		}
	}


	//for (auto& target : aiTarget)
	//{ 
	//	for (int i = 0; i < target->assaultableList.length; i++)
	//	{
	//		auto& p = target->assaultableList.position[i];
	//		auto v = confilictMap[p->y][p->x];
	//		if (v == 0)
	//		{
	//			totalValue += chessValue[p->chess->type] * 2 / 3;
	//		}
	//		else
	//		{
	//			totalValue += chessValue[p->chess->type] / (2 + v);
	//		} 
	//	} 
	//}
	//for (auto& target : playerTarget)
	//{ 
	//	for (int i = 0; i < target->assaultableList.length; i++)
	//	{
	//		auto& p = target->assaultableList.position[i]; 
	//		auto v = confilictMap[p->y][p->x];
	//		if (v == 0)
	//		{
	//			totalValue -= chessValue[p->chess->type] * 2 / 3;
	//		}
	//		else
	//		{
	//			totalValue -= chessValue[p->chess->type] / (2 + v);
	//		}
	//	} 
	//}  
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
	//		value += positionValue[Black][type][chess->py][chess->px];
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
	//		value += positionValue[Black][type][chess->py][chess->px];
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
