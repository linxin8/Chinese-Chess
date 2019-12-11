#include "tree.h"
#include "dllfunction.h"
#include "debug.h"
#include<iostream>
#include<ctime>
#include<algorithm>
#include"hash.h"

std::unordered_map<HashKeyType, int> Tree::rootHashMap[17][17];
SearchResultHash Tree::aiResultHash;
SearchResultHash Tree::playerResultHash;
SimpleList<HashKeyType,200> Tree::situationHistory;

const int Tree::maximumValue = 10000000;
const int Tree::minimumValue = -10000000;

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
	for (auto& i : aiChessCandidate)
	{
		i->updateTarget();
	}
	for (auto& i : playerChessCandidate)
	{
		i->updateTarget();
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
		timeCheckCount = 100000;
		if ((std::clock() - lastTime) / CLOCKS_PER_SEC > maxThinkTime)
		{
			timeOver = true;
		}
	}
}

Decision* Tree::deepSearch()
{
	root.debugPrintBoard();
	lastTime = std::clock();
	//lastTime = std::time(nullptr);
	////minThinkTime = 10;
	//maxThinkTime = minThinkTime = 100000;
	//depthMax = 18;
	//auto last1 = deepSearch(5);
	//depthMax = 6;
	//auto last2 = deepSearch(2);
	//depthMax = 9;
	//auto last3 = deepSearch(3);
	//depthMax = 12;
	//auto last4= deepSearch(4);
	//std::cout << "think step -> " << 2 << ", max deep last -> " << maxDeepLast
	//	<< " (" << last->fromX << ',' << last->fromY << ") -> (" << last->desX << ',' << last->desY << ')' << std::endl;
	//return last;
	//auto d = deepSearch(2);
	timeCheckCount = 100000;
	//timeCheckCount = 100000000;
	//auto d = deepSearch(3);
	timeOver = false;
	Action lastBestAction;
	int step = 2;
	int stepIncrement = 2; 
	maxThinkTime = 2;
	bool getAction = false;
	int actionTotalCount = 0; 
	for (; step < 20; step += stepIncrement)
	{
		depthMaxLast = 0;
		depthMax = step + 12;
		lastTime = std::clock(); 
		quiescentDepthMax = 6;
		actionCount = 0;
		std::cout << "try step: " << step << std::endl;
		auto bestAction = deepSearch(step, getAction ? &lastBestAction : nullptr);
		actionTotalCount += actionCount;
		std::cout << "  time cost " << 1000.0*(std::clock() - lastTime) / CLOCKS_PER_SEC << ", action count " << actionCount << std::endl;
		if (timeOver)
		{
			step -= stepIncrement;
			break;
		}
		lastBestAction = bestAction;
		getAction = true;
		//auto diff = std::time(nullptr) - lastTime;
		//if (diff >= minThinkTime)
		//{
		//	break;
		//}
	}
	std::cout << "reusult: think step -> " << step << ", depth max -> " << depthMaxLast
		<<", action => ("<<lastBestAction.from->x<<','<< lastBestAction.from->y<<") -> ("<< lastBestAction.des->x <<','<< lastBestAction.des->y <<')' << ", action total count " << actionTotalCount << std::endl << std::endl;
	if (lastBestAction.coveredChessType != None)
	{
		situationHistory.clear();
	}
	situationHistory.push_back(peekRootHashValue(lastBestAction));
	auto decision = new Decision();
	decision->fromX = lastBestAction.from->x;
	decision->fromY = lastBestAction.from->y;
	decision->desX = lastBestAction.des->x;
	decision->desY = lastBestAction.des->y;
	return decision;
}

int Tree::getEstimatedValue()
{ 
	//auto& map = rootHashMap[aiChessCount][playerChessCount];
	//auto it = map.find(rootHashValue);
	//if (it != map.end())
	//{ 
	//	return it->second;
	//}
	auto value = root.getEstimatedValue(aiChessCandidate, playerChessCandidate);
	//map[rootHashValue] = value; 
	return value;
}

void Tree::appendAllActionCandidate(const std::vector<Chess*>& chessCandidate, SimpleList<Action, 100>& actionCandidate)
{
	ChessCountry currentTurnCountry;
	for (auto& c : chessCandidate)
	{
		if (c->type != None)
		{
			currentTurnCountry = c->country;
			break;
		}
	}
	Action actionTemp;
	auto lengthLast = actionCandidate.length; 
	for (size_t i = 0; i < chessCandidate.size(); i++)
	{
		if (chessCandidate[i]->type != None)
		{
			auto target = chessCandidate[i]->getTarget();
			actionTemp.from = root.getChessPositoin(chessCandidate[i]->getX(), chessCandidate[i]->getY());
			for (auto& p : target->assaultableList)
			{
				actionTemp.des = root.getChessPositoin(p->x, p->y);
				actionTemp.coveredChessType = actionTemp.des->chess->type;
				actionCandidate.push_back(actionTemp);
			}
			for (auto& p : target->moveableList)
			{
				actionTemp.des = root.getChessPositoin(p->x, p->y);
				actionTemp.coveredChessType = actionTemp.des->chess->type;
				actionCandidate.push_back(actionTemp);
			}
		}
	} 
	if (actionCandidate.length > 0)
	{
		std::pair<int, int> value[100];
		int sum = 0;
		for (int i = lengthLast; i < actionCandidate.length; i++)
		{
			doAction(actionCandidate[i]);
			value[i].first = getEstimatedValue();
			sum += value[i].first;
			value[i].second = i;
			undoAction();
		}
		if (currentTurnCountry == Black)
		{
			std::sort(value + lengthLast, value + actionCandidate.length, std::greater<std::pair<int, int>>());
		}
		else
		{
			std::sort(value + lengthLast, value + actionCandidate.length, std::less<std::pair<int, int>>());
		}
		Action temp[100];
		for (int i = lengthLast; i < actionCandidate.length; i++)
		{
			temp[i] = actionCandidate[value[i].second];
		}
		for (int i = lengthLast; i < actionCandidate.length; i++)
		{
			actionCandidate[i] = temp[i];
		} 
	}

}

void Tree::appendSortedActionCandidate(const std::vector<Chess*>& chessCandidate, SimpleList<Action, 100>& actionCandidate)
{
	ChessCountry currentTurnCountry;
	for (auto& c : chessCandidate)
	{
		if (c->type != None)
		{
			currentTurnCountry = c->country;
			break;
		}
	}
	Action actionTemp;
	auto lengthLast = actionCandidate.length;
	for (size_t i = 0; i < chessCandidate.size(); i++)
	{
		if (chessCandidate[i]->type != None)
		{
			auto target = chessCandidate[i]->getTarget();
			actionTemp.from = root.getChessPositoin(chessCandidate[i]->getX(), chessCandidate[i]->getY());
			for (auto& p : target->assaultableList)
			{
				actionTemp.des = root.getChessPositoin(p->x, p->y);
				actionTemp.coveredChessType = actionTemp.des->chess->type;
				actionCandidate.push_back(actionTemp);
			}
			for (auto& p : target->moveableList)
			{
				actionTemp.des = root.getChessPositoin(p->x, p->y);
				actionTemp.coveredChessType = actionTemp.des->chess->type;
				actionCandidate.push_back(actionTemp);
			}
		}
	}
	std::pair<int, int> value[100];
	for (int i = lengthLast; i < actionCandidate.length; i++)
	{
		doAction(actionCandidate[i]);
		value[i].first = getEstimatedValue();
		value[i].second = i;
		undoAction();
	}
	if (currentTurnCountry == Black)
	{
		std::sort(value + lengthLast, value + actionCandidate.length, std::greater<std::pair<int, int>>());
	}
	else
	{
		std::sort(value + lengthLast, value + actionCandidate.length, std::less<std::pair<int, int>>());
	}
	Action temp[100];
	for (int i = lengthLast; i < actionCandidate.length; i++)
	{
		temp[i] = actionCandidate[value[i].second];
	}
	for (int i = lengthLast; i < actionCandidate.length; i++)
	{
		actionCandidate[i] = temp[i];
	}
}

void Tree::appendSortedAssaultableActionCandidate(const std::vector<Chess*>& chessCandidate, SimpleList<Action, 100>& actionCandidate)
{
	Action actionTemp;
	ChessCountry currentTurnCountry;
	for (auto& c : chessCandidate)
	{
		if (c->type != None)
		{
			currentTurnCountry = c->country;
			break;
		}
	}
	auto lengthLast = actionCandidate.length;
	for (size_t i = 0; i < chessCandidate.size(); i++)
	{
		if (chessCandidate[i]->type != None)
		{
			auto target = chessCandidate[i]->getTarget();
			actionTemp.from = root.getChessPositoin(chessCandidate[i]->getX(), chessCandidate[i]->getY());
			for (auto& p : target->assaultableList)
			{
				actionTemp.des = root.getChessPositoin(p->x, p->y);
				actionTemp.coveredChessType = actionTemp.des->chess->type;
				actionCandidate.push_back(actionTemp);
			}
		}
	}
	//std::cout << actionCandidate.length << '\n';
	//if (actionCandidate.length > 0)
	//{
	//	int newSize = actionCandidate.length; 
	//	std::pair<int, int> value[20];
	//	for (int i = lengthLast; i < actionCandidate.length; i++)
	//	{
	//		doAction(actionCandidate[i]);
	//		value[i].first = getEstimatedValue();
	//		value[i].second = i;
	//		undoAction();
	//	}
	//	if (currentTurnCountry == Black)
	//	{
	//		std::partial_sort(value + lengthLast, value + newSize, value + actionCandidate.length, std::greater<std::pair<int, int>>());
	//	}
	//	else
	//	{
	//		std::partial_sort(value + lengthLast, value + newSize, value + actionCandidate.length, std::less<std::pair<int, int>>());
	//	}
	//	Action temp[20];
	//	for (int i = lengthLast; i < newSize; i++)
	//	{
	//		temp[i] = actionCandidate[value[i].second];
	//	}
	//	for (int i = lengthLast; i < newSize; i++)
	//	{
	//		actionCandidate[i] = temp[i];
	//	}
	//	actionCandidate.length = newSize;
	//}
}

void Tree::appendAssaultableActionCandidate(const std::vector<Chess*>& chessCandidate, SimpleList<Action, 100>& actionCandidate)
{
	Action actionTemp;
	auto lengthLast = actionCandidate.length;
	for (size_t i = 0; i < chessCandidate.size(); i++)
	{
		if (chessCandidate[i]->type != None)
		{
			auto target = chessCandidate[i]->getTarget();
			actionTemp.from = root.getChessPositoin(chessCandidate[i]->getX(), chessCandidate[i]->getY());
			for (auto& p : target->assaultableList)
			{
				actionTemp.des = root.getChessPositoin(p->x, p->y);
				actionTemp.coveredChessType = actionTemp.des->chess->type;
				actionCandidate.push_back(actionTemp);
			}
		}
	}
	//std::sort(actionCandidate.begin() + lengthLast, actionCandidate.end(),
	//	[](Action& a, Action& b)
	//{
	//	constexpr int chessValue[8] = { 0,200,1500,3500,1500,800,800,/*100000000*/ 0 };
	//	if (a.coveredChessType == None && b.coveredChessType == None)
	//	{
	//		return chessValue[a.from->chess->type] > chessValue[b.from->chess->type];
	//	}
	//	return chessValue[a.coveredChessType] - chessValue[a.from->chess->type] > chessValue[b.coveredChessType] - chessValue[b.from->chess->type];
	//});
	//std::sort(actionCandidate.begin() + lengthLast, actionCandidate.end(), [](Action&a, Action&b)
	//{
	//	constexpr int chessPriority[8] = { 0, 1, 3, 4, 3, 2, 2, 10 };
	//	return chessPriority[a.coveredChessType] > chessPriority[a.coveredChessType];
	//});
}

void Tree::appendSafeActionCandidate(ChessCountry currentTurnCountry, SimpleList<Action, 100>& actionCandidate)
{
	Action actionTemp; 
	auto& selfChess = (currentTurnCountry == Black) ? aiChessCandidate : playerChessCandidate;
	appendSortedActionCandidate(selfChess, actionCandidate);
	return;
	for (size_t i = 0; i < selfChess.size(); i++)
	{
		if (selfChess[i]->type != None)
		{
			auto target = selfChess[i]->getTarget();
			actionTemp.from = root.getChessPositoin(selfChess[i]->getX(), selfChess[i]->getY());
			for (auto& p : target->assaultableList)
			{
				actionTemp.des = root.getChessPositoin(p->x, p->y);
				actionTemp.coveredChessType = actionTemp.des->chess->type;
				if (p->chess->type == King)
				{
					actionCandidate.push_back(actionTemp);
				}
				else
				{
					doAction(actionTemp);
					if (isKingSafe(currentTurnCountry))
					{ 
						actionCandidate.push_back(actionTemp);
					}
					undoAction();
				}
			}
			for (auto& p : target->moveableList)
			{
				actionTemp.des = root.getChessPositoin(p->x, p->y);
				actionTemp.coveredChessType = actionTemp.des->chess->type; 
				doAction(actionTemp);
				if (isKingSafe(currentTurnCountry))
				{
					actionCandidate.push_back(actionTemp);
				}
				undoAction();
			}
		}
	}
}

bool Tree::checkIsCurrentSituationRepeated()const
{
	for (auto& i : situationHistory)
	{
		if (i == rootHashValue)
		{
			return true;
		}
	}
	return false;
}

HashKeyType Tree::peekRootHashValue(const Action& action)
{
	doAction(action);
	auto ret = rootHashValue; 
	undoAction();
	return ret;
}

bool Tree::isKingSafe(ChessCountry country)
{ 
	for (auto& i : (country == Black ? playerChessCandidate : aiChessCandidate))
	{
		auto target = i->getTarget();
		for (auto& p : target->assaultableList)
		{
			if (p->chess->type == King)
			{
				return false;
			} 
		} 
	}
	return true;
}
 

Action Tree::deepSearch(int depthTotal, Action* recommendAction)
{ 
	Action bestAction{ nullptr,nullptr,None };
	Position from(0,0);  
	int alpha = -20000000;
	int beta = 20000000; 

	SimpleList<Action, 100> actionCandidate;
	if (recommendAction != nullptr)
	{
		actionCandidate.push_back(*recommendAction);
	}
	appendSortedActionCandidate(aiChessCandidate, actionCandidate);
	int value = 0;
	bool hasPVValue = false;
	for (auto& action : actionCandidate)
	{
		doAction(action); 
		//int nextDeep = action.coveredChessType == None ? depthTotal - 1 : depthTotal;
		int nextDeep = depthTotal - 1;
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
		if (value > alpha&&value < beta)
		{
			alpha = value;
			bestAction = actionStack.top();
			hasPVValue = true;
		}
		undoAction();
	}
	if (timeOver)
	{ 
		std::cout << "  time over" << std::endl; 
	}
	else
	{ 
		std::cout << "  max value :" << alpha <<", depth max: "<< depthMaxLast << ", best aciton: " << "(" << bestAction.from->x << ','
			<< bestAction.from->y << ") -> (" << bestAction.des->x << ',' << bestAction.des->y << ')' << std::endl;
	}
	//auto decision = new Decision;
 //
	//decision->fromX = bestAction.from->x;
	//decision->fromY = bestAction.from->y;
	//decision->desX = bestAction.des->x;
	//decision->desY = bestAction.des->y;
	//return decision;
	return bestAction;
}

int Tree::deepSearchMax(int depthLeft, int alpha, int beta)
{// think in Black(AI)
	if (aiKingChess->type == None)
	{
		return minimumValue + depthCurrent;
	}
	//if (checkIsCurrentSituationRepeated())
	//{
	//	return maximumValue;
	//}
	auto result = aiResultHash.getHash(rootHashValue); 
	//if (result != nullptr&&result->depth >= depthLeft)
	//{
	//	if (result->type == SearchResult::PV)
	//	{
	//		return result->value;
	//	}
	//	if (result->type == SearchResult::Alpha&&result->value <= alpha)
	//	{
	//		return alpha;
	//	}
	//	if (result->type == SearchResult::Beta&&result->value >= beta)
	//	{
	//		return beta;
	//	}
	//}
	if (depthLeft <= 0)
	{
		//return getEstimatedValue();
		return quiescentMax(alpha, beta);
		//return quiescentMaxWithDelta(alpha, beta, beta);
	}  
	bool isSafe = isKingSafe(Black);
	if (isSafe)
	{
		auto R = depthLeft > 6 ? 4 : 3;
		//auto value = deepSearchMin(depthLeft - R - 1, alpha, beta);
		auto value = deepSearchMin(depthLeft - R - 1, beta-1, beta);
		if (value >= beta)
		{
			depthLeft -= 4;
			if (depthLeft <= 0)
			{
				return quiescentMax(alpha, beta);
			}
			//return beta;
		}
	}
	//if (depthLeft > 3)
	//{
	//	if (deepSearchMin(depthLeft - 3, beta - 1, beta) >= beta &&
	//		deepSearchMin(depthLeft - 2, beta - 1, beta) >= beta)
	//	{
	//		return beta;
	//	}
	//} 
	updateTime();
	if (timeOver)
	{
		return  getEstimatedValue();
	}
	bool hasPVValue = false;
	SimpleList<Action, 100> actionCandidate;
	if (result != nullptr&&result->depth >= depthLeft)
	{
		actionCandidate.push_back(result->bestAction);
	}
	if (isSafe)
	{
		appendSortedActionCandidate(aiChessCandidate, actionCandidate); 
	}
	else
	{
		appendSafeActionCandidate(Black, actionCandidate);
	}
	Action bestAction{ nullptr,nullptr,None };
	int value = 0;
	for (auto& action : actionCandidate)
	{ 
		doAction(action);
		//int nextDepth = action.coveredChessType == None ? depthLeft - 1 : depthLeft;
		int nextDepth = depthLeft - 1;
		//if (nextDepth == 0 && action.coveredChessType != None)
		//{
		//	nextDepth = 1;
		//}
		if (hasPVValue)
		{
			value = deepSearchMin(nextDepth, alpha, alpha + 1);
			if (value > alpha&&value <= beta)
			{
				value = deepSearchMin(nextDepth, alpha, beta);
			}
		}
		else
		{
			value = deepSearchMin(nextDepth, alpha, beta);
		}
		undoAction();
		if (value >= beta)
		{
			bestAction = action;
			aiResultHash.updateHash({ rootHashValue,depthLeft,SearchResult::Beta,beta,bestAction }); 
			return beta;
		}
		else if (value > alpha)
		{
			alpha = value;
			hasPVValue = true;
			bestAction = action;
		}
	}
	if (bestAction.from != nullptr)
	{ 
		aiResultHash.updateHash({ rootHashValue,depthLeft,hasPVValue ? SearchResult::PV : SearchResult::Alpha,alpha,bestAction });
	}
	return alpha;
} 



int Tree::deepSearchMin(int depthLeft, int alpha, int beta)
{// think in Red(Player)
	if (playerKingChess->type == None)
	{
		return maximumValue - depthCurrent;
	}
	if (checkIsCurrentSituationRepeated())
	{
		return minimumValue;
	}
	auto result = playerResultHash.getHash(rootHashValue);
	//if (result != nullptr&&result->depth >= depthLeft)
	//{
	//	if (result->type == SearchResult::PV)
	//	{
	//		return result->value;
	//	}
	//	if (result->type == SearchResult::Alpha&&result->value <= alpha)
	//	{
	//		return alpha;
	//	}		
	//	if (result->type == SearchResult::Beta&&result->value >= beta)
	//	{
	//		return beta;
	//	} 
	//}
	if (depthLeft <= 0)
	{
		//return getEstimatedValue();
		return quiescentMin(alpha, beta);
		//return quiescentMinWithDelta(alpha, beta, alpha);
	} 
	const bool isSafe = isKingSafe(Red);
	if (isSafe)
	{
		auto R = depthLeft > 6 ? 4 : 3;
		//auto value = deepSearchMax(depthLeft - R - 1, alpha, beta);
		auto value = deepSearchMax(depthLeft - R - 1, alpha, alpha + 1);
		if (value <= alpha)
		{
			//return alpha;
			depthLeft -= 4;
			if (depthLeft <= 0)
			{
				return quiescentMax(alpha, beta);
			}
		}
	}
	//if (depthLeft > 3)
	//{
	//	if (deepSearchMax(depthLeft - 3, alpha, alpha + 1) <= alpha&&
	//		deepSearchMax(depthLeft - 2, alpha, alpha + 1) <= alpha)
	//	{
	//		return alpha;
	//	}
	//}

	updateTime();
	if (timeOver)
	{
		return  getEstimatedValue();
	} 
	bool hasPVValue = false;
	SimpleList<Action, 100> actionCandidate;
	if (result != nullptr&&result->depth >= depthLeft)
	{
		actionCandidate.push_back(result->bestAction);
	}
	if (isSafe)
	{
		appendSortedActionCandidate(playerChessCandidate, actionCandidate); 
	}
	else
	{
		appendSafeActionCandidate(Red, actionCandidate);
	}
	Action bestAction{ nullptr,nullptr,None };
	int value = 0;
	for (auto& action : actionCandidate)
	{
		doAction(action); 
		//int nextDepth = action.coveredChessType == None ? depthLeft - 1 : depthLeft;
		int nextDepth = depthLeft - 1;
		//if (nextDepth == 0 && action.coveredChessType != None)
		//{
		//	nextDepth = 1;

		//}
		if (hasPVValue)
		{
			value = deepSearchMax(nextDepth, beta-1, beta);
			if (value >= alpha && value < beta)
			{
				value = deepSearchMax(nextDepth, alpha, beta);
			} 
		}
		else
		{
			value = deepSearchMax(nextDepth, alpha, beta); 
		}
		undoAction();
		if (value <= alpha)
		{
			bestAction = action;
			playerResultHash.updateHash({ rootHashValue,depthLeft,SearchResult::Alpha,alpha,bestAction });
			return alpha;
		}
		else if (value < beta)
		{
			beta = value;
			hasPVValue = true;
			bestAction = action;
		}
	}
	if (bestAction.from != nullptr)
	{
		playerResultHash.updateHash({ rootHashValue,depthLeft,hasPVValue ? SearchResult::PV : SearchResult::Beta,beta,bestAction });
	}
	return beta;
}

int Tree::quiescentMax(int alpha, int beta, int quiescentDepth)
{
	if (aiKingChess->type == None)
	{
		return minimumValue + depthCurrent;
	} 
	//if (checkIsCurrentSituationRepeated()) //useless
	//{
	//	return maximumValue;
	//}
	//auto baseValue = getEstimatedValue();
	//if (depth >= 5)
	//{
	//	return getEstimatedValue();
	//}
	if (depthCurrent > depthMax || quiescentDepth >= quiescentDepthMax)
	{
		//return getEstimatedValue();
		return alpha;
	}
	updateTime();
	if (timeOver)
	{
		return  getEstimatedValue();
	}	
	//SimpleList<Action, 100> assaultedActionCandidate;
	//appendAssaultableActionCandidate(playerChessCandidate, assaultedActionCandidate);
	//for (auto& i : assaultedActionCandidate)
	//{
	//	if (i.coveredChessType == King)
	//	{
	//		depthMax += 1;
	//		auto ret = deepSearchMax(1, alpha, beta);
	//		depthMax -= 1;
	//		return ret;
	//	}
	//} 

	////alpha = std::max(alpha, getEstimatedValue());
	auto estimatevalue = getEstimatedValue()  ;
	if (estimatevalue > alpha)
	{
		alpha = estimatevalue;
	}
	if (beta <= alpha)
	{
		return beta;
	}	
	SimpleList<Action, 100> actionCandidate;
	if (isKingSafe(Black))
	{
		appendAssaultableActionCandidate(aiChessCandidate, actionCandidate);
		//appendSortedAssaultableActionCandidate(aiChessCandidate, actionCandidate);
		if (actionCandidate.empty())
		{
			return estimatevalue;
		}
	}
	else
	{
		//appendAllActionCandidate(aiChessCandidate, actionCandidate);
		appendSafeActionCandidate(Black, actionCandidate);
		if (actionCandidate.empty())
		{
			return minimumValue + depthCurrent;
		}
		quiescentDepth -= 1;
	}
	int value = 0;
	for (auto& action : actionCandidate)
	{
		doAction(action);
		auto value = quiescentMin(alpha, alpha + 1, quiescentDepth + 1);
		if (value > alpha)
		{
			value = quiescentMin(alpha, beta, quiescentDepth + 1);
		} 
		undoAction(); 
		if (value >= beta)
		{  
			return beta;
		}
		else if (value > alpha)
		{
			alpha = value;  
		}
	}
	return alpha;
}

int Tree::quiescentMin(int alpha, int beta, int quiescentDepth)
{
	if (playerKingChess->type == None)
	{
		return maximumValue - depthCurrent;
	} 
	//if (checkIsCurrentSituationRepeated()) //soft limitation
	//{
	//	return minimumValue;
	//} 
	//if (depth >= 5)
	//{
	//	return getEstimatedValue();
	//}
	if (depthCurrent > depthMax || quiescentDepth >= quiescentDepthMax)
	{
		//return getEstimatedValue();
		return beta;
	}
	updateTime();
	if (timeOver)
	{
		return getEstimatedValue();
	}
	//SimpleList<Action, 100> assaultedActionCandidate;
	//appendAssaultableActionCandidate(aiChessCandidate, assaultedActionCandidate);
	//for (auto& i : assaultedActionCandidate)
	//{
	//	if (i.coveredChessType == King)
	//	{
	//		//depthMax += 1;
	//		appendActionCandidate(playerChessCandidate, actionCandidate);
	//		//auto ret = deepSearchMin(1, alpha, beta);
	//		//depthMax -= 1;
	//		//return ret;
	//	}
	//}


	int value = 0;
	//beta = std::min(beta, getEstimatedValue());
	auto estimateValue = getEstimatedValue();
	if (estimateValue < beta)
	{
		beta = estimateValue;
	}
	if (beta <= alpha)
	{
		return alpha;
	}	
	SimpleList<Action, 100> actionCandidate;
	if (isKingSafe(Red))
	{
		appendAssaultableActionCandidate(playerChessCandidate, actionCandidate);
		//appendSortedAssaultableActionCandidate(playerChessCandidate, actionCandidate);
		if (actionCandidate.empty())
		{
			return estimateValue;
		}
	}
	else
	{
		//appendAllActionCandidate(playerChessCandidate, actionCandidate);
		appendSafeActionCandidate(Red, actionCandidate);
		if (actionCandidate.empty())
		{
			return  maximumValue - depthCurrent;
		}
		quiescentDepth -= 1;
	}
	for (auto& action : actionCandidate)
	{
		doAction(action); 
		value = quiescentMax(beta - 1, beta, quiescentDepth + 1);
		if (value < beta)
		{
			value = quiescentMax(alpha, beta, quiescentDepth + 1);
		} 
		undoAction();
		if (value <= alpha)
		{ 
			return alpha;
		}
		else if (value < beta)
		{
			beta = value; 
		}
	} 
	return beta;
}

int Tree::quiescentMinWithDelta(int alpha, int beta, int standardValue)
{
	if (playerKingChess->type == None)
	{
		return maximumValue - depthCurrent;
	}
	if (checkIsCurrentSituationRepeated())
	{
		return minimumValue;
	}
	auto currentValue = getEstimatedValue();
	updateTime();
	if (timeOver)
	{
		return currentValue;
	} 
	SimpleList<Action, 100> actionCandidate;
	appendSortedActionCandidate(playerChessCandidate, actionCandidate);    
	for (auto& action : actionCandidate)
	{
		doAction(action);
		auto value = quiescentMaxWithDelta(alpha, beta, currentValue);
		undoAction();
		if (value <= alpha)
		{
			return alpha;
		}
		else if (value < beta)
		{
			beta = value;
		}
	}
	return beta;
}

int Tree::quiescentMaxWithDelta(int alpha, int beta, int standardValue)
{
	if (playerKingChess->type == None)
	{
		return maximumValue - depthCurrent;
	}
	if (checkIsCurrentSituationRepeated())
	{
		return minimumValue;
	}
	auto currentValue = getEstimatedValue();
	updateTime();
	if (timeOver)
	{
		return currentValue;
	} 
	SimpleList<Action, 100> actionCandidate;
	appendSortedActionCandidate(aiChessCandidate, actionCandidate);
	for (auto& action : actionCandidate)
	{
		doAction(action);
		auto value = quiescentMinWithDelta(alpha, beta, currentValue);
		undoAction();
		if (value >= beta)
		{
			return beta;
		}
		else if (value > alpha)
		{
			alpha = value;
		}
	}
	return alpha;
}

void Tree::doAction(const Action& action)
{
	situationHistory.push_back(rootHashValue);
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
	from->chess->cp = from;
	des->chess->cp = des;
	actionStack.push(action);
	depthCurrent++;
	if (depthCurrent > depthMaxLast)
	{
		depthMaxLast = depthCurrent;
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
	notifyBoardChange(from->x, from->y, des->x, des->y);
	actionCount++;
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
	notifyBoardUndoChange(from->x, from->y, des->x, des->y);
	des->chess->setType(action.coveredChessType);
	from->chess->cp = from;
	des->chess->cp = des;
	depthCurrent--;
	rootHashValue ^= getChessPositionHash(from);
	rootHashValue ^= getChessPositionHash(des); 
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
	situationHistory.pop_back();  
}

void Tree::notifyBoardChange(int x1, int y1, int x2, int y2)
{
	for (auto& i : aiChessCandidate)
	{
		if (i->type != None)
		{ 
			i->onBoardChessMove(x1, y1, x2, y2);
		}
	}	
	for (auto& i : playerChessCandidate)
	{
		if (i->type != None)
		{
			i->onBoardChessMove(x1, y1, x2, y2);
		}
	}
}

void Tree::notifyBoardUndoChange(int x1, int y1, int x2, int y2)
{
	for (auto& i : aiChessCandidate)
	{
		if (i->type != None)
		{
			i->onBoardChessUndoMove(x1, y1, x2, y2);
		}
	}
	for (auto& i : playerChessCandidate)
	{
		if (i->type != None)
		{
			i->onBoardChessUndoMove(x1, y1, x2, y2);
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
