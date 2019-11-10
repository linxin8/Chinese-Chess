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
	Action deepSearch(int depth, Action* recommendAction = nullptr);
	int deepSearchMax(int depth, int alpha, int beta);
	int deepSearchMin(int depth, int alpha, int beta);
	int quiescentMax(int alpha, int beta, int quiescentDepth = 0);
	int quiescentMin(int alpha, int beta, int quiescentDepth = 0);
	int quiescentMinWithDelta(int alpha, int beta, int standardValue);
	int quiescentMaxWithDelta(int alpha, int beta, int standardValue);

	void doAction(const Action& action);
	void undoAction();
	void notifyBoardChange(int x1, int y1, int x2, int y2);
	void notifyBoardUndoChange(int x1, int y1, int x2, int y2);
public:
	ChessTarget* getTarget(int x,int y);
private:
	void updateTime();
	void debugPrintAction()const;
	int getEstimatedValue();
	void appendActionCandidate(const std::vector<Chess*>& chessCandidate, SimpleList<Action, 100>& actionCandidate);
	void appendAssaultableActionCandidate(const std::vector<Chess*>& chessCandidate, SimpleList<Action, 100>& actionCandidate);
 private: 
	Node root; 
	std::stack<Action> actionStack; 
	int maxThinkTime = 3;
	int minThinkTime = 2;
	bool timeOver;
	int timeCheckCount = 100;
	time_t lastTime;  
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
	static const int maximumValue;
	static const int minimumValue;
	static SimpleList<HashKeyType, 200> situationHistory;
	bool checkIsCurrentSituationRepeated()const;
	HashKeyType peekRootHashValue(const Action& action);
	bool isKingSafe(ChessCountry country); 
	int quiescentDepthMax = 4;
	int depthMax;
}; 