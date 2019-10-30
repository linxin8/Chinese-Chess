#include "tree.h"
#include "dllfunction.h"
#include "debug.h"
#include<iostream>
#include<ctime>
#include<algorithm>
#include"hash.h"

std::unordered_map<int, int> Tree::rootHashMap[17][17];


Tree::Tree(int state[10][9]):root(state)
{
	rootHashValue = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 10; j++)
		{ 
			auto cp = root.getChessPositoin(i,j);
			if (cp->chess->type != None)
			{ 
				rootHashValue ^= getChessPositionHash(cp);
				if (cp->chess->country == Black)
				{
					aiChessCandidate.push_back(cp->chess);
					if (cp->chess->type == King)
					{
						aiKingChess = cp->chess;
					}
				}
				else
				{
					playerChessCandidate.push_back(cp->chess);	
					if (cp->chess->type == King)
					{
						playerKingChess = cp->chess;
					}
				}
			}		
		}
	}
	constexpr int chessPriority[8] = { 0, 1, 3, 4, 3, 2, 2, 2 }; 
	auto cmp = [&chessPriority](Chess* a, Chess*  b) {return chessPriority[a->type] > chessPriority[b->type]; };
	std::sort(aiChessCandidate.begin(), aiChessCandidate.end(), cmp);
	std::sort(playerChessCandidate.begin(), playerChessCandidate.end(), cmp);
	aiChessCount = aiChessCandidate.size();
	playerChessCount = playerChessCandidate.size();
	for (int i = 0; i < aiChessCount; i++)
	{
		for (int j = 0; j < playerChessCount; j++)
		{
			rootHashMap[i][j].clear();
		}
	}
}


Tree::~Tree()
{
}

ChessTarget* Tree::getTarget(int x, int y)
{
	return root.getChessPositoin(x,y)->chess->getTarget();
}

void Tree::updateTime()
{
	if (--timeCheckCount == 0)
	{
		timeCheckCount = 100;
		if (std::time(nullptr) - lastTime > maxThinkTime)
		{
			timeOver = true;
		}
	}
}

Decision* Tree::deepSearch()
{
	//minThinkTime = 10;
	////maxThinkTime = minThinkTime = 100000;
	//maxDeep = 20;
	//lastTime = std::time(nullptr);
	//auto last= deepSearch(2);
	//std::cout << "think step -> " << 2 << ", max deep last -> " << maxDeepLast
	//	<< " (" << last->fromX << ',' << last->fromY << ") -> (" << last->desX << ',' << last->desY << ')' << std::endl;
	//return last;
	////auto d = deepSearch(2);
	timeCheckCount = 100;
	//timeCheckCount = 10000000000000;
	//auto d = deepSearch(4);
	timeOver = false;
	Decision* lastDecision = nullptr;
	int step = 2;
	int stepIncrement = 1;
	for (; step<20; step += stepIncrement)
	{
		maxDeepLast = 0;
		lastTime = std::time(nullptr);
		std::cout << "try step: " << step << std::endl;
		deepMax = std::max(10, step *2);
		auto decision = deepSearch(step);
		if (timeOver)
		{
			step -= stepIncrement;
			break;
		}
		if (lastDecision != nullptr)
		{
			delete lastDecision;
		}
		lastDecision = decision;
		auto diff = std::time(nullptr) - lastTime;
		if (diff >= minThinkTime)
		{
			break;
		} 
	}
	std::cout << "think step -> " << step << ", max last -> " << maxDeepLast
		<<"("<<lastDecision->fromX<<','<<lastDecision->fromY<<") -> ("<<lastDecision->desX<<','<<lastDecision->desY<<')'<< std::endl;
	return lastDecision;
}

int Tree::getEstimatedValue()
{
	auto& map = rootHashMap[aiChessCount][playerChessCount];
	auto it = map.find(rootHashValue);
	if (it != map.end())
	{
		return it->second;
	}
	auto value = root.getEstimatedValue(aiChessCandidate, playerChessCandidate);
	map[rootHashValue] = value;
	return value;
}

void Tree::calculateActionCandidate(const std::vector<Chess*>& chessCandidate, SimpleList<Action, 100>& actionCandidate)
{
	Action actionTemp;
	for (size_t i = 0; i < chessCandidate.size(); i++)
	{
		if (chessCandidate[i]->type != None)
		{
			auto target = chessCandidate[i]->getTarget();
			actionTemp.from = root.getChessPositoin(chessCandidate[i]->getX(), chessCandidate[i]->getY());
			for (int j = 0; j < target->assaultableList.length; j++)
			{
				actionTemp.des = root.getChessPositoin(target->assaultableList.position[j]->x, target->assaultableList.position[j]->y);
				actionTemp.coveredChessType = actionTemp.des->chess->type;
				actionCandidate.push_back(actionTemp);
			}
			for (int j = 0; j < target->moveableList.length; j++)
			{
				actionTemp.des = root.getChessPositoin(target->moveableList.position[j]->x, target->moveableList.position[j]->y);
				actionTemp.coveredChessType = actionTemp.des->chess->type;
				actionCandidate.push_back(actionTemp);
			}
		}
	}
}
 

struct Decision* Tree::deepSearch(int deepTotal)
{ 
	Action bestAction{ nullptr,nullptr,None };
	Position from(0,0);  
	int alpha = INT32_MIN;	
	int beta = INT32_MAX;   
	SimpleList<Action, 100> actionCandidate;
	calculateActionCandidate(aiChessCandidate, actionCandidate);
	int value = 0;
	bool hasPVValue = false;
	for (auto& action : actionCandidate)
	{
		doAction(action);
		int nextDeep = action.coveredChessType == None ? deepTotal - 1 : deepTotal;
		if (hasPVValue)
		{
			value = deepSearchMin(nextDeep, alpha, alpha + 1);
			if (value > alpha&&value <= beta)
			{
				value = deepSearchMin(nextDeep, alpha, beta);
			}
		}
		else
		{
			value = deepSearchMin(nextDeep, alpha, beta);
		}
		if (value > alpha)
		{
			alpha = value;
			bestAction = actionStack.top(); 
			hasPVValue = true;
		}
		undoAction();
	}
	std::cout << "max value " << alpha << std::endl;  
	auto decision = new Decision;
 
	decision->fromX = bestAction.from->x;
	decision->fromY = bestAction.from->y;
	decision->desX = bestAction.des->x;
	decision->desY = bestAction.des->y;
	return decision;
}

int Tree::deepSearchMax(int deepLeft, int alpha, int beta)
{// think in Black(AI)
	if (aiKingChess->type == None)
	{
		return INT32_MIN;
	}
	if (deepLeft == 0)
	{
		return getEstimatedValue();
	}
	if (deepCurrent >= deepMax)
	{
		return alpha;
	}
	updateTime();
	if (timeOver)
	{
		return alpha;
	}
	bool hasPVValue = false;
	SimpleList<Action, 100> actionCandidate;
	calculateActionCandidate(aiChessCandidate, actionCandidate); 
	int value = 0;
	for (auto& action : actionCandidate)
	{ 
		doAction(action);
		int nextDeep = action.coveredChessType == None ? deepLeft - 1 : deepLeft;
		if (hasPVValue)
		{
			value = deepSearchMin(nextDeep, alpha, alpha + 1);
			if (value > alpha&&value <= beta)
			{
				value = deepSearchMin(nextDeep, alpha, beta);
			}
		}
		else
		{
			value = deepSearchMin(nextDeep, alpha, beta);
		}
		undoAction();
		if (value >= beta)
		{
			return value;
		}
		else if (value > alpha)
		{
			alpha = value;
			hasPVValue = true;
		}
	}
	return alpha;
} 

int Tree::deepSearchMin(int levelLeft, int alpha, int beta)
{// think in Red(Player)	 
	if (playerKingChess->type == None)
	{
		return INT32_MAX;
	}
	if (levelLeft == 0)
	{
		return getEstimatedValue();
	} 
	if (deepCurrent >= deepMax)
	{
		return beta;
	}
	updateTime();
	if (timeOver)
	{
		return beta;
	} 
	bool hasPVValue = false;
	SimpleList<Action, 100> actionCandidate;
	calculateActionCandidate(playerChessCandidate, actionCandidate);
	int value = 0;
	for (auto& action : actionCandidate)
	{
		doAction(action);
		int nextLevel = action.coveredChessType == None ? levelLeft - 1 : levelLeft;
		if (hasPVValue)
		{
			value = deepSearchMax(nextLevel, alpha, alpha + 1);
			if (value >= alpha && value < beta)
			{
				value = deepSearchMax(nextLevel, alpha, beta);
			}
		}
		else
		{
			value = deepSearchMax(nextLevel, alpha, beta);
		}
		undoAction();
		if (value <= alpha)
		{
			return value;
		}
		else if (value < beta)
		{
			beta = value;
			hasPVValue = true;
		}
	} 
	return beta;
}

void Tree::doAction(const Action& action)
{  
	nodeValueStack.push(getEstimatedValue());
	auto from = action.from;
	auto des = action.des;
	Debug::assert(des->chess->isNoneTypeChess() || from->chess->isHostile(des->chess));
	Debug::assert(action.coveredChessType == des->chess->type);  
	rootHashValue ^= getChessPositionHash(from);
	rootHashValue ^= getChessPositionHash(des);

	std::swap(from->chess, des->chess);
	from->chess->setX(from->x);
	from->chess->setY(from->y);
	des->chess->setX(des->x);
	des->chess->setY(des->y);
	from->chess->setType(None); 
	actionStack.push(action);
	deepCurrent++;
	if (deepCurrent > maxDeepLast)
	{
		maxDeepLast = deepCurrent;
	}
	rootHashValue ^= getChessPositionHash(from);
	rootHashValue ^= getChessPositionHash(des);
	if (action.coveredChessType != None)
	{
		if (des->chess->country == Black)
		{
			playerChessCount -= 1;
		}
		else
		{
			aiChessCount -= 1;
		}
	} 
}

void Tree::undoAction()
{
	auto action = actionStack.top();
	actionStack.pop();
	auto from = action.from;
	auto des = action.des;
	rootHashValue ^= getChessPositionHash(from);
	rootHashValue ^= getChessPositionHash(des);
	Debug::assert(from->chess->isNoneTypeChess()
		&& (action.coveredChessType == None || from->chess->isHostile(des->chess)));
	std::swap(from->chess, des->chess);
	from->chess->setX(from->x);
	from->chess->setY(from->y);
	des->chess->setX(des->x);
	des->chess->setY(des->y);
	des->chess->setType(action.coveredChessType);
	deepCurrent--;
	rootHashValue ^= getChessPositionHash(from);
	rootHashValue ^= getChessPositionHash(des);
	nodeValueStack.pop();
	if (action.coveredChessType != None)
	{
		if (from->chess->country == Black)
		{
			playerChessCount += 1;
		}
		else
		{
			aiChessCount += 1;
		}
	}
}
 
void Tree::debugPrintAction() const
{
	std::cout << "---debug print tree action---\n";
	auto temp = actionStack;
	std::stack<Action> stack;
	while(!temp.empty())
	{
		stack.push(temp.top());
		temp.pop();
	}
	while (!stack.empty())
	{
		auto top = stack.top();
		std::cout << '(' << top.from->x << ',' << top.from->y << ") -> ("
			<< top.des->x << ',' << top.des->y << ")" << std::endl;
		stack.pop();
	}
}
