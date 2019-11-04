#pragma once   

template<typename T, int N>
struct SimpleList
{
	T data[N];
	int length = 0;
	void push_back(const T& r) { data[length++] = r; }
	void pop_back() { length--; }
	T& front()const { return data[0]; }
	T& back()const { return data[length - 1]; }
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


