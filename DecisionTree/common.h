#pragma once   

template<typename T, int N>
struct SimpleList
{
	T data[N];
	int length = 0;
	SimpleList() {};
	SimpleList(const SimpleList<T, N>& r) { for (int i = 0; i < length; i++) data[i] = r.data[i]; this->length = r.length; }
	SimpleList& operator=(const SimpleList<T, N>& r) { for (int i = 0; i < length; i++) data[i] = r.data[i]; this->length = r.length; return *this; }
	void push_back(const T& r) { data[length++] = r; }
	void pop_back() { length--; }
	T& front()const { return data[0]; }
	T& back(){ return data[length - 1]; }
	T& operator[](int i)const { return data[i]; }
	T& operator[](int i) { return data[i]; }
	T* begin() { return data; };
	T* end() { return data + length; };
	const T* begin()const { return data; };
	const T* end()const { return data + length; };
	bool empty()const { return length == 0; }
	void clear() { length = 0; }
};

class Chess;
struct ChessPosition
{
	ChessPosition() :x(-1), y(-1), chess(nullptr) {}
	ChessPosition(int x, int y, Chess* chess) :x(x), y(y), chess(chess) {}
	ChessPosition(const ChessPosition& r) :x(r.x), y(r.y), chess(r.chess) {} 
	int x;
	int y;
	Chess* chess;
	ChessPosition* up;
	ChessPosition* left;
	ChessPosition* down;
	ChessPosition* right; 
	struct
	{
		SimpleList<ChessPosition*, 3> blackCandidate;
		SimpleList<ChessPosition*, 3> redCandidate;
	}pawnCandidate; 
	struct
	{
		SimpleList<ChessPosition*, 8> candidate;
		SimpleList<ChessPosition*, 8> handicap;
	}knightCandidate;	
	struct
	{
		SimpleList<ChessPosition*, 8> candidate;
		SimpleList<ChessPosition*, 8> handicap;
	}elephantCandidate;
	SimpleList<ChessPosition*, 4> guardCandidate;
	SimpleList<ChessPosition*, 4> kingCandidate; 
};


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
	SimpleList<ChessPosition*, 17> moveableList; 
	SimpleList<ChessPosition*, 8> assaultableList; 
	SimpleList<ChessPosition*, 8> defendableList;
	void clear() { moveableList.clear(); assaultableList.clear(); defendableList.clear(); }
};

struct Position
{
	int x;
	int y;
	constexpr Position(int x, int y) :x(x), y(y) {}
};

struct Action
{
	struct ChessPosition* from = nullptr;
	struct ChessPosition* des = nullptr;
	ChessType coveredChessType;
}; 
 