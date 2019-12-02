#pragma once 
#include"chess.h" 
#include<vector>

class Node
{
public:
	Node(int state[10][9]);
	~Node();
	void loadBoard(int state[10][9]);
	int getEstimatedValue(const std::vector<Chess*>& aiCandidate, const std::vector<Chess*>& playerCandidate, ChessCountry currentCountry);
	bool isNoneChess(int x, int y) { return board[y][x].chess->type == None; }
	bool isHostile(int x, int y, ChessCountry country) { return board[y][x].chess->country != country; }
	Position getStep(const Node& des, const Position& from)const;
	//std::list<ChessPosition*> getTargetChessPosition(ChessPosition* cp); 
	void SetChess(int x, int y, Chess* chess) { board[y][x].chess = chess; }
	ChessPosition* getChessPositoin(int x, int y) { return &board[y][x]; }
	static ChessPosition board[10][9];
	void debugPrintBoard()const;
private: 
private:
	void initChessPosistion(int x,int y);
};

