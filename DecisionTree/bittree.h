#pragma once
#include"bitboard.h"
#include"estimate.h"
#include<algorithm>

//#define MYDEBUG_ACTION_SORTED_LIST
#define MYDEBUG_SEARCH_PATH

class BitTree
{
public:
	BitTree(int map[10][9]) :board(map)
	{ 
	}
	~BitTree() = default;
	ChessTarget& getTarget(int x, int y)
	{ 
		return board.getTargetByIndex((y + 3) * 16 + x + 3);
	} 
	int getEstimatedValue(int country)
	{
		return board.getEstimatedValue(country);
	}

	uint64_t makeAcion(uint64_t fromIndex, uint64_t desIndex, uint64_t coveredType)
	{
		uint64_t action = 0;
		board.set_fromIndex(action,fromIndex);
		board.set_desIndex(action,desIndex);
		board.set_coveredType(action, coveredType);
		return action;
	}

	void appendSortedAction(SimpleList<uint64_t, 100>& candidate,int country)
	{ 
		std::pair< int, uint64_t> buff[100];
		int buffSize = 0;
		for (int i = 0; i < 32; i++)
		{
			auto fromChess = board.getChessById(i);
			if (board.get_type(fromChess) &&
				board.get_country(fromChess) == country)
			{
				auto target = board.getTargetByChess(fromChess);
				auto fromIndex = board.get_index(fromChess);
				for (int j = 0; j < target.assaultableList.length; j++)
				{ 
					auto desChess = target.assaultableList[j];
					auto desIndex = board.get_index(desChess);
					auto desType = board.get_type(desChess);
					Debug::assert(desType != None);
					auto action = makeAcion(fromIndex, desIndex, desType);
					board.doAction(action);
					auto value = board.getEstimatedValue(country);
					board.undoAction();
					buff[buffSize].first = value;
					buff[buffSize].second = action;
					buffSize++;
				}
				for (int j = 0; j < target.moveableList.length; j++)
				{
					auto desChess = target.moveableList[j];
					auto desIndex = board.get_index(desChess);
					auto action = makeAcion(fromIndex, desIndex, None);
					board.doAction(action);
					auto value = board.getEstimatedValue(country);
					board.undoAction();
					buff[buffSize].first = value;
					buff[buffSize].second = action;
					buffSize++;
				}
			}
		}
		std::sort(buff, buff + buffSize);
		for (int i = buffSize-1; i >= 0; i--)
		{
			candidate.push_back(buff[i].second);
		}
#ifdef MYDEBUG_ACTION_SORTED_LIST
		for (int i = buffSize - 1; i >= 0; i--)
		{
			std::cout << buffSize - i - 1 << ": " << buff[i].first << " ";
			printAction(buff[i].second);
		}
#endif // MYDEBUG_ACTION_SORTED_LIST

	}

	void appendQuiescentAction(SimpleList<uint64_t, 100>& candidate, int country)
	{ 
		for (int i = 0; i < 32; i++)
		{
			auto fromChess = board.getChessById(i);
			if (board.get_type(fromChess) &&
				board.get_country(fromChess) == country)
			{
				auto fromIndex = board.get_index(fromChess);
				auto target = board.getTargetByChess(fromChess);
				for (int j = 0; j < target.assaultableList.length; j++)
				{
					auto desChess = target.assaultableList[j];
					auto desIndex = board.get_index(desChess);
					Debug::assert(board.getChessByIndex(desIndex) == desChess);
					auto desType = board.get_type(desChess);
					Debug::assert(desType != None);
					auto action = makeAcion(fromIndex, desIndex, desType);
					candidate.push_back(action);
				} 
			}
		} 
	}

	void appendSafeAction(SimpleList<uint64_t, 100>& candidate, int country)
	{
		for (int i = 0; i < 32; i++)
		{
			auto fromChess = board.getChessById(i);
			if (board.get_type(fromChess) &&
				board.get_country(fromChess) == country)
			{
				auto fromIndex = board.get_index(fromChess);
				auto target = board.getTargetByChess(fromChess);
				for (int j = 0; j < target.assaultableList.length; j++)
				{
					auto desChess = target.assaultableList[j];
					auto desIndex = board.get_index(desChess);
					Debug::assert(board.getChessByIndex(desIndex) == desChess);
					auto desType = board.get_type(desChess);
					Debug::assert(desType != None);
					auto action = makeAcion(fromIndex, desIndex, desType);
					board.doAction(action);
					if (board.isKingSafe(country))
					{ 
						candidate.push_back(action);
					}
					board.undoAction();
				}
				for (int j = 0; j < target.moveableList.length; j++)
				{
					auto desChess = target.moveableList[j];
					auto desIndex = board.get_index(desChess);
					Debug::assert(board.getChessByIndex(desIndex) == desChess);
					auto desType = board.get_type(desChess);
					Debug::assert(desType != None);
					auto action = makeAcion(fromIndex, desIndex, desType);
					board.doAction(action);
					if (board.isKingSafe(country))
					{
						candidate.push_back(action);
					}
					board.undoAction();
				}
			}
		}
	}


	void startSerach(int& fromX, int& fromY, int& desX, int& desY)
	{
		int alpha = -20000000;
		int beta = 20000000;
		uint64_t actionResult = 0; 
		for (int depth = 1; depth <= 6; depth += 1)
		{
			std::cout << "step " << depth << '\n';
			auto value = search(alpha, beta, Black, depth, actionResult);
			std::cout << "best action ";
			board.printAction(actionResult);
			std::cout << "value " << value << '\n';
		}
		auto from = board.get_fromIndex(actionResult);
		auto des = board.get_desIndex(actionResult);
		fromX = (int)(from % 16 - 3);
		fromY = (int)(from / 16 - 3);
		desX = (int)(des % 16 - 3);
		desY = (int)(des / 16 - 3);
	}

	int quiescentSearch(int alpha, int beta, int country)
	{
		depth++;
		if (board.isKingDied(country))
		{
			return -100000000 + depth;
		}
		auto estimate = getEstimatedValue(country);
		if (estimate > alpha)
		{
			alpha = estimate;
		}
		if (alpha >= beta)
		{
			return alpha;
		}
		SimpleList<uint64_t, 100> candidate;
		if (board.isKingSafe(country))
		{ 
			appendQuiescentAction(candidate, country);
		}
		else
		{
			appendSafeAction(candidate, country);
		}
		if (candidate.empty())
		{
			return estimate;
		}
		for (int i = 0; i < candidate.length; i++)
		{
			auto action = candidate[i];
			uint64_t bestAction = 0;
			board.doAction(action);
			auto value = -quiescentSearch(-beta, -alpha, !country);
			board.undoAction();
			if (value > alpha)
			{ 
				alpha = value;
			}
			if (alpha >= beta)
			{ 
				break;
			}
		}
		depth--;
		return alpha;
	}


	int search(int alpha, int beta, int country, int depthLeft,uint64_t& actionResult)
	{ 
		depth++;
		if (board.isKingDied(country))
		{
			return -10000000 + depth;
		}
		if (depthLeft <= 0)
		{
			return quiescentSearch(alpha, beta, country);
			//return getEstimatedValue(country);
		}
		SimpleList<uint64_t, 100> candidate;
		if (board.isKingSafe(country))
		{
			auto R = depthLeft > 6 ? 4 : 3; 
			uint64_t actionResult = 0;
			auto value = -search(-beta, -beta + 1, !country, depthLeft - R - 1, actionResult);
			if (value >= beta)
			{
				depthLeft -= 4;
				if (depthLeft <= 0)
				{
					//return quiescentSearch(alpha, beta,country);
					return getEstimatedValue(country);
				}
			}
			appendSortedAction(candidate, country);
		}
		else
		{
			appendSafeAction(candidate, country);
			if (candidate.empty())
			{
				return alpha;
			}
		}
		auto value = alpha;
		bool pv = false;
		for (int i = 0; i < candidate.length; i++)
		{
			auto action = candidate[i];
			uint64_t bestAction = 0;
			uint64_t subActionResult = 0;
			board.doAction(action);
			if (pv)
			{
				value = -search(-alpha - 1, -alpha, !country, depthLeft - 1, subActionResult);
				if (alpha < value&&value < beta)
				{
					value = -search(-beta, -alpha, !country, depthLeft - 1, subActionResult);
				} 
			}
			else
			{ 
				value = -search(-beta, -alpha, !country, depthLeft - 1, subActionResult);
			}
			board.undoAction();
			if (value > alpha)
			{
				actionResult = action;
				alpha = value;
				pv = true;
			}
			if (alpha >= beta)
			{ 
				break;
			}
		}
		depth--;
		return alpha;
	}
	void printBoard()const 
	{
		board.printBoard();
	}
	void printAction(uint64_t action)const
	{
		board.printAction(action);
	}
private:
	BitBoard board;
	int depth = 0;
};

