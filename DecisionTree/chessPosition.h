#pragma once
class Chess;
struct ChessPosition
{
	ChessPosition():x(-1), y(-1),chess(nullptr) {}
	ChessPosition(int x, int y, Chess* chess) :x(x), y(y), chess(chess) {}
	ChessPosition(const ChessPosition& r) :x(r.x), y(r.y), chess(r.chess) {}
	ChessPosition& operator=(const ChessPosition& r)  {*const_cast<int*>(&this->x) = r.x; *const_cast<int*>(&this->y) = r.y; this->chess = r.chess; return *this; }
	const int x;
	const int y;
	Chess* chess;
	//Chess*&  operator->() 
	//{
	//	return chess;
	//}
	//Chess*const&  operator->()const
	//{
	//	return chess;
	//}
	//operator Chess*const()const
	//{
	//	return chess;
	//}	
	//operator Chess*()
	//{
	//	return chess;
	//}
};

