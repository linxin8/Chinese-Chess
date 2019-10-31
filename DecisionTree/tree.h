#pragma once 
#include<stack>
#include<map>
#include<unordered_map>  
#include"node.h"  
#include"hash.h"

class Tree
{
public:
	Tree(int state[10][9]);
	~Tree();
public:
	struct Decision* deepSearch();
private:
	struct Decision* deepSearch(int depth);
	int deepSearchMax(int depth, int alpha, int beta);
	int deepSearchMin(int depth, int alpha, int beta);
	void doAction(const Action& action);
	void undoAction();
public:
	ChessTarget* getTarget(int x,int y);
private:
	void updateTime();
	void debugPrintAction()const;
	int getEstimatedValue();
	void appendActionCandidate(const std::vector<Chess*>& chessCandidate, SimpleList<Action, 100>& actionCandidate);
 private: 
	Node root; 
	std::stack<Action> actionStack; 
	int maxThinkTime = 10;
	int minThinkTime = 5;
	bool timeOver;
	int timeCheckCount = 100;
	time_t lastTime; 
	int depthMax = 5;
	int depthCurrent = 0;
	int depthMaxLast = 0;
	HashKeyType rootHashValue;
	static std::unordered_map<HashKeyType, int> rootHashMap[17][17];
	std::vector<Chess*> aiChessCandidate;
	std::vector<Chess*> playerChessCandidate;
	int aiChessCount;
	int playerChessCount; 
	Chess* aiKingChess;
	Chess* playerKingChess;
	static SearchResultHash aiResultHash;
	static SearchResultHash playerResultHash;
};

 