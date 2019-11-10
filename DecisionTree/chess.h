#pragma once 
#include"debug.h"
#include"common.h"
#include<bitset>

class Chess
{ 
public:
	static Chess* newInstance(class Node* node, int x, int y, ChessType type, ChessCountry country);
public:
	Chess() = delete;
	virtual ~Chess() {}
protected:
	Chess(class Node* node,int x,int y, ChessType type, ChessCountry country)
		:node(node),px(x),py(y),type(type), country(country),target(new ChessTarget()){}
public:
	void setX(int x) { this->px = x; }
	void setY(int y) { this->py = y; }
	int getX()const { return px; }
	int getY()const { return py; }
	bool isAIChess() const { return country == Black; }
	bool isPlayerChess() const { return country == Red; }
	bool isHostile(Chess* chess) const { return this->country != chess->country; }
	bool isNoneTypeChess() const { return this->type == None; }
	//ChessType getType() const { return type; }
	//ChessCountry getCountry() const { return country; }
	ChessTarget* getTarget(){ /*updateTarget();*/ return target; }
	bool operator !=(const Chess& r) { return this->type != r.type || this->country != r.country || this->px != r.px || this->py != r.py; }
	void setCountry(ChessCountry country) { this->country = country; }
	void setType(ChessType type) { this->type = type; }
	void onBoardChessMove(int x1, int y1, int x2, int y2);
	void onBoardChessUndoMove(int x1, int y1, int x2, int y2);
	void updateTarget();
private:
	void sortAssaultableTarget();
protected:
	//bool isInRange(int value, int min, int max) { return min <= value && value < max; }// [min,max)
	virtual void recomputeTargetPositionInfo() = 0;
public: 
	ChessType type;
	ChessCountry country;
	ChessPosition* cp;
	int px;
	int py;
protected:
	class Node* node; 
	ChessTarget* target;
	std::bitset<90> partialView;
	SimpleList<std::bitset<90>,60> partialViewList;
	SimpleList<ChessTarget, 60> targetList;
};

class NoneChess :public Chess
{
public:
	NoneChess(class Node* node, int x, int y, ChessCountry country)
		:Chess(node, x, y, None, country) {}
protected:
	virtual void recomputeTargetPositionInfo() override { Debug::notReachable(); }
};

class PawnChess:public Chess
{
public: 	
	PawnChess(class Node* node, int x, int y, ChessCountry country)
	:Chess(node,x,y,Pawn,country) {} 
protected:
	virtual void recomputeTargetPositionInfo() override;
private:
	 static const Position relativePosition[4];
};

class CannonChess :public Chess
{
public:
	CannonChess(class Node* node, int x, int y, ChessCountry country)
		:Chess(node, x, y, Cannon, country) {}
protected:
	virtual void recomputeTargetPositionInfo() override;
private:
	static const Position relativePosition[4]; 
};

class RookChess :public Chess
{
public:
	RookChess(class Node* node, int x, int y, ChessCountry country)
		:Chess(node, x, y, Rook, country) {}
protected:
	virtual void recomputeTargetPositionInfo() override;
private:
	static const Position relativePosition[4]; 
};

class KnightChess :public Chess
{
public:
	KnightChess(class Node* node, int x, int y, ChessCountry country)
		:Chess(node, x, y, Knight, country) {}
protected:
	virtual void recomputeTargetPositionInfo() override;
private:
	static const Position relativePosition[8]; 
};

class ElephantChess :public Chess
{
public:
	ElephantChess(class Node* node, int x, int y, ChessCountry country)
		:Chess(node, x, y, Elephant, country) {}
protected:
	virtual void recomputeTargetPositionInfo() override;
private:
	static const Position relativePosition[4]; 
};  

class GuardChess :public Chess
{
public:
	GuardChess(class Node* node, int x, int y, ChessCountry country)
		:Chess(node, x, y, Guard, country) {}
protected:
	virtual void recomputeTargetPositionInfo() override;
private:
	static const Position relativePosition[4];
};

class KingChess :public Chess
{
public:
	KingChess(class Node* node, int x, int y, ChessCountry country)
		:Chess(node, x, y, King, country) {}
protected:
	virtual void recomputeTargetPositionInfo() override;
private:
	static const Position relativePosition[4];
}; 
