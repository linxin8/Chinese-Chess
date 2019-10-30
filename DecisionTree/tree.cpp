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
		maxDeep = std::max(10, step *2);
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


struct Decision* Tree::deepSearch(int level)
{ 
	Action bestAction{ nullptr,nullptr,None };
	Position from(0,0);  
	int alpha = INT32_MIN;	
	int beta = INT32_MAX;  
	ChessTarget targetBuffer[20];
	for (size_t i = 0; i < aiChessCandidate.size(); i++)
	{
		if (aiChessCandidate[i]->type != None)
		{
			targetBuffer[i] = *aiChessCandidate[i]->getTarget();
		}
	}
	for (size_t i = 0; i < aiChessCandidate.size(); i++)
	{
		if (aiChessCandidate[i]->type == None)
		{
			continue;
		}
		auto chessTarget = targetBuffer[i];
		auto cp = root.getChessPositoin(aiChessCandidate[i]->getX(), aiChessCandidate[i]->getY());
		for (int j = 0; j < chessTarget.assaultableList.length; j++)
		{
			auto target = chessTarget.assaultableList.position[j];
			doAction({ cp,target,target->chess->type });
			auto value = deepSearchMin(level, alpha, beta);
			if (value > alpha)
			{
				alpha = value;
				bestAction = actionStack.top();
			}
			undoAction();
		}
	}
	for (size_t i = 0; i < aiChessCandidate.size(); i++)
	{
		if (aiChessCandidate[i]->type == None)
		{
			continue;
		}
		auto chessTarget = targetBuffer[i];
		auto cp = root.getChessPositoin(aiChessCandidate[i]->getX(), aiChessCandidate[i]->getY());
		for (int j = 0; j < chessTarget.moveableList.length; j++)
		{
			auto target = chessTarget.moveableList.position[j];
			doAction({ cp,target,target->chess->type });
			auto value = deepSearchMin(level - 1, alpha, beta);
			if (value > alpha)
			{
				alpha = value;
				bestAction = actionStack.top();
			}
			undoAction();
		}
	}

	//for (auto& chess : aiChessCandidate)
	//{
	//	if (chess->type == None)
	//	{
	//		continue;
	//	}
	//	auto cp = root.getChessPositoin(chess->getX(), chess->getY());
	//	auto chessTarget = *chess->getTarget();
	//	for (int i = 0; i < chessTarget.assaultableList.length; i++)
	//	{
	//		auto target = chessTarget.assaultableList.position[i];
	//		doAction({ cp,target,target->chess->type });
	//		auto value = deepSearchMin(level, alpha, beta);
	//		if (value > alpha)
	//		{
	//			alpha = value;
	//			bestAction = actionStack.top();
	//		}
	//		undoAction();
	//	}
	//	for (int i = 0; i < chessTarget.moveableList.length; i++)
	//	{
	//		auto target = chessTarget.moveableList.position[i];
	//		doAction({ cp,target,target->chess->type });
	//		auto value = deepSearchMin(level - 1, alpha, beta);
	//		if (value > alpha)
	//		{
	//			alpha = value;
	//			bestAction = actionStack.top();
	//		}
	//		undoAction();
	//	}
	//}
	std::cout << "max value " << alpha << std::endl;  
	auto decision = new Decision;
 
	decision->fromX = bestAction.from->x;
	decision->fromY = bestAction.from->y;
	decision->desX = bestAction.des->x;
	decision->desY = bestAction.des->y;
	return decision;
}

int Tree::deepSearchMax(int levelLeft, int alpha, int beta)
{// think in Black(AI)
	if (aiKingChess->type == None)
	{
		return INT32_MIN;
	}
	if (levelLeft == 0)
	{
		return getEstimatedValue();
	}
	//if (getEstimatedValue() - searchGradient > nodeValueStack.top())
	//{
	//	return beta;
	//}
	//if (getEstimatedValue() + searchWindow <= alpha)
	//{
	//	return alpha;
	//}
	if (deep >= maxDeep)
	{
		return alpha;
	}
	updateTime();
	if (timeOver)
	{
		return alpha;
	} 
	if (levelLeft > 2)
	{
		//if (deepSearchMax(2, alpha, beta) - searchGradient > nodeValueStack.top())
		//{
		//	return beta;
		//}
		//if (deepSearchMin(levelLeft - 1, alpha, beta) > getEstimatedValue())
		//{
		//	//return  getEstimatedValue();
		//	return std::min(beta, getEstimatedValue());
		//}
	}
	ChessTarget targetBuffer[20];
	for (size_t i = 0; i < aiChessCandidate.size(); i++)
	{
		if (aiChessCandidate[i]->type != None)
		{
			targetBuffer[i] = *aiChessCandidate[i]->getTarget();
		}
	}
	for (size_t i = 0; i < aiChessCandidate.size(); i++)
	{
		if (aiChessCandidate[i]->type == None)
		{
			continue;
		}
		auto chessTarget = targetBuffer[i];
		auto cp = root.getChessPositoin(aiChessCandidate[i]->getX(), aiChessCandidate[i]->getY());
		for (int j = 0; j < chessTarget.assaultableList.length; j++)
		{
			auto target = chessTarget.assaultableList.position[j];
			doAction({ cp,target,target->chess->type });
			auto value = deepSearchMin(levelLeft, alpha, beta);
			undoAction();
			if (value >= beta)
			{
				return value;
			}
			else if (value > alpha)
			{
				alpha = value;
			}
			if (beta <= alpha)
			{
				return alpha;
			}
		}
	}
	for (size_t i = 0; i < aiChessCandidate.size(); i++)
	{
		if (aiChessCandidate[i]->type == None)
		{
			continue;
		}
		auto chessTarget = targetBuffer[i];
		auto cp = root.getChessPositoin(aiChessCandidate[i]->getX(), aiChessCandidate[i]->getY());
		for (int j = 0; j < chessTarget.moveableList.length; j++)
		{
			auto target = chessTarget.moveableList.position[j];
			doAction({ cp,target,target->chess->type });
			auto value = deepSearchMin(levelLeft-1, alpha, beta);
			undoAction();
			if (value >= beta)
			{
				return value;
			}
			else if (value > alpha)
			{
				alpha = value;
			}
			if (beta <= alpha)
			{
				return alpha;
			}
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
	//if (getEstimatedValue() + searchGradient < nodeValueStack.top())
	//{
	//	return alpha;
	//}
	//if (getEstimatedValue() - searchWindow >= beta)
	//{
	//	return beta;
	//}
	if (deep >= maxDeep)
	{
		return beta;
	}
	updateTime();
	if (timeOver)
	{
		return beta;
	}
	if (levelLeft > 2)
	{
		//if (deepSearchMin(2, alpha, beta) + searchGradient < nodeValueStack.top())
		//{
		//	return alpha;
		//}	
		//if (deepSearchMax(levelLeft-1, alpha, beta) < getEstimatedValue())
		//{
		//	//return  getEstimatedValue();
		//	return std::max(alpha, getEstimatedValue());
		//}
	}
	 
	ChessTarget targetBuffer[20]; 
	for (size_t i = 0; i < playerChessCandidate.size(); i++)
	{ 
		if (playerChessCandidate[i]->type != None)
		{
			targetBuffer[i] = *playerChessCandidate[i]->getTarget();
		}
	}
	for (size_t i = 0; i < playerChessCandidate.size(); i++)
	{
		if (playerChessCandidate[i]->type == None)
		{
			continue;
		}
		auto chessTarget = targetBuffer[i];
		auto cp = root.getChessPositoin(playerChessCandidate[i]->getX(), playerChessCandidate[i]->getY());
		for (int j = 0; j < chessTarget.assaultableList.length; j++)
		{
			auto target = chessTarget.assaultableList.position[j];
			doAction({ cp,target,target->chess->type });
			auto value = deepSearchMax(levelLeft, alpha, beta);
			undoAction();
			if (value <= alpha)
			{
				return value;
			}
			else if (value < beta)
			{
				beta = value;
			}
			if (beta <= alpha)
			{
				return beta;
			}
		}
	}
	for (size_t i = 0; i < playerChessCandidate.size(); i++)
	{
		if (playerChessCandidate[i]->type == None)
		{
			continue;
		}
		auto chessTarget = targetBuffer[i];
		auto cp = root.getChessPositoin(playerChessCandidate[i]->getX(), playerChessCandidate[i]->getY());
		for (int j = 0; j < chessTarget.moveableList.length; j++)
		{
			auto target = chessTarget.moveableList.position[j];
			doAction({ cp,target,target->chess->type });
			auto value = deepSearchMax(levelLeft-1, alpha, beta);
			undoAction();
			if (value <= alpha)
			{
				return value;
			}
			else if (value < beta)
			{
				beta = value;
			}
			if (beta <= alpha)
			{
				return beta;
			}
		}
	} 
	return beta;
}

void Tree::doAction(const Action& action)
{ 
	//auto estimatedValue = getEstimatedValue();
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
	deep++;
	if (deep > maxDeepLast)
	{
		maxDeepLast = deep;
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
	//std::cout << "estimated value change " << estimatedValue << " --> " << getEstimatedValue() << std::endl;
	//if (action.coveredChessType == Rook&&action.des->chess->type==Rook&&actionStack.size()==2)
	//{
	//	std::cout << "bug";
	//}
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
	deep--;
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
