#pragma once
#include<list>
#include"debug.h"
#include"common.h"

enum ChessType : char
{
	None,
	Pawn,
	Cannon,
	Rook,
	Knight,
	Elephant,
	Guard,
	King
};

enum ChessCountry : char
{
	Black,
	Red
};

struct ChessPosition;
struct ChessTarget
{
	struct 
	{
		ChessPosition* position[17]{};
		int length = 0;
		void push_back(ChessPosition* c) { position[length++] = c; } 
	}moveableList;
	struct
	{
		ChessPosition* position[8]{};
		int length = 0;
		void push_back(ChessPosition* c) { position[length++] = c; }
	}assaultableList;
	struct
	{
		ChessPosition* position[8]{};
		int length = 0;
		void push_back(ChessPosition* c) { position[length++] = c; }
	}defendableList;
	void clear() { moveableList.length = 0; assaultableList.length = 0; defendableList.length = 0; }
};

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
	ChessTarget* getTarget(){ updateTarget(); return target; }
	bool operator !=(const Chess& r) { return this->type != r.type || this->country != r.country || this->px != r.px || this->py != r.py; }
	void setCountry(ChessCountry country) { this->country = country; }
	void setType(ChessType type) { this->type = type; }
private:
	void updateTarget();
	void sortAssaultableTarget();
protected:
	virtual void recomputeTargetPositionInfo() = 0;
public: 
	ChessType type;
	ChessCountry country;
protected:
	class Node* node; 
	ChessTarget* target;
	int px;
	int py;
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
