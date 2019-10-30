#pragma once
#include"node.h"
#include<stack>
#include<map>
#include<unordered_map>

struct Action
{
	ChessPosition* from; 
	ChessPosition* des;
	ChessType coveredChessType;
};

class Tree
{
public:
	Tree(int state[10][9]);
	~Tree();
public:
	struct Decision* deepSearch();
private:
	struct Decision* deepSearch(int level);
	int deepSearchMax(int level, int alpha, int beta);
	int deepSearchMin(int level, int alpha, int beta);
	void doAction(const Action& action);
	void undoAction();
public:
	ChessTarget* getTarget(int x,int y);
private:
	void updateTime();
	void debugPrintAction()const;
	int getEstimatedValue();
	void calculateActionCandidate(const std::vector<Chess*>& chessCandidate, SimpleList<Action, 100>& actionCandidate);
 private: 
	Node root; 
	std::stack<Action> actionStack;
	std::stack<int> nodeValueStack;
	int maxThinkTime = 10;
	int minThinkTime = 5;
	bool timeOver;
	int timeCheckCount = 100;
	time_t lastTime; 
	int deepMax = 10;
	int deepCurrent = 0;
	int maxDeepLast = 0;
	int rootHashValue; 
	static std::unordered_map<int, int> rootHashMap[17][17];
	std::vector<Chess*> aiChessCandidate;
	std::vector<Chess*> playerChessCandidate;
	int aiChessCount;
	int playerChessCount; 
	Chess* aiKingChess;
	Chess* playerKingChess;   
};

