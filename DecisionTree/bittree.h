#pragma once
#include"bitboard.h"
#include"estimate.h"
#include<algorithm>
#include<ctime>
//#define MYDEBUG_ACTION_SORTED_LIST 
//#define MYDEBUG_HASH_COLLISION

struct CounterGuard
{
	CounterGuard(int& i) :counter(i)
	{
		counter++;
	}
	~CounterGuard()
	{
		counter--;
	}
	int& counter;
};

class BitTree
{
private: 
	static constexpr int hashTableMask = (1 << 24) - 1;
	struct HashTable
	{
		uint64_t key = 0;
		int depth = 0;
		enum Type
		{
			Upperbound,
			Lowerbound,
			PV
		}type;
		int value=0;
		uint64_t bestAction;
	};
	static HashTable hashTable[hashTableMask + 1];
	void updateHash(uint64_t key, int depth, HashTable::Type type, int value, uint64_t bestAction=0)
	{
		auto index = key & hashTableMask;
		auto& old = hashTable[index];
		if (old.depth < depth)
		{
			old.depth = depth;
			old.key = key;
			old.type = type;
			old.value = value;
			old.bestAction = bestAction;
		}
	}
	const HashTable& getHash(uint64_t key) const
	{
		auto index = key & hashTableMask;
		return hashTable[index];  
	}
	bool hasHash(uint64_t key)const 
	{
		auto index = key & hashTableMask;
		auto& old = hashTable[index];
#ifdef MYDEBUG_HASH_COLLISION 
		if (old.key == key)
		{
			std::cout << "hash hit " << key << '\n';
			board.printBoard();
		}
#endif // MYDEBUG_HASH_COLLISION 
		return old.key == key;
	}
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

	int estimateAction(uint64_t action,int country)
	{ 
		auto hash = board.getHash();
		if (hasHash(hash))
		{
			auto bestAction = getHash(hash).bestAction;
			if (action == bestAction)
			{
				return 5000000;
			}
		} 
		auto value = INT_MIN;
		if (killAction[depth][0] == 0)
		{
			board.doAction(action);
			value = board.getEstimatedValue(country);
			board.undoAction();
		}
		else
		{ 
			for (int i = 0; i < 2; i++)
			{
				if (killAction[depth][i] == action)
				{ 
					return 5000000 - i;
				}
			}
		}
		return 0;
	}


	void appendAllAction(SimpleList<uint64_t, 100>& candidate, int country)
	{
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
					candidate.push_back(action);
				}
				for (int j = 0; j < target.moveableList.length; j++)
				{
					auto desChess = target.moveableList[j];
					auto desIndex = board.get_index(desChess);
					auto action = makeAcion(fromIndex, desIndex, None);
					candidate.push_back(action);
				}
			}
		}
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
					buff[buffSize].first = estimateAction(action, country);
					buff[buffSize].second = action;
					buffSize++;
				}
				for (int j = 0; j < target.moveableList.length; j++)
				{
					auto desChess = target.moveableList[j];
					auto desIndex = board.get_index(desChess);
					auto action = makeAcion(fromIndex, desIndex, None);
					buff[buffSize].first = estimateAction(action, country);
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
					if (board.getChessValueByChess(desChess)>900)//ignore pawn¡¢ elephant ¡¢ guard
					{
						candidate.push_back(action); 
					}
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
					Debug::assert(desType == None);
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
		for (auto& i : historyTable)
		{
			for (auto&j : i)
			{
				for (auto& k : j)
				{
					k >>= 1;
				}
			}
		}
		board.printBoard();
		int alpha = -20000000;
		int beta = 20000000;
		uint64_t actionResult = 0;
		//std::memset(hashTable, 0, sizeof(hashTable));
		for (int depth = 1; depth <= 8; depth += 1)
		{
			auto lastNodeCount = nodeCount;
			auto time = std::clock();
			std::cout << "step " << depth;
			auto value = search(alpha, beta, Black, depth, actionResult);
			//auto value = BNS(alpha, beta, Black, 7, actionResult);
			std::cout << ", node counted " << nodeCount - lastNodeCount
				<< ", time " << std::clock() - time;
			std::cout << ", value " << value << ", best action ";
			board.printAction(actionResult); 
			printHashTableState();
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
		//if (depth > 12)
		//{
		//	std::cout << depth << '\n';
		//}

		CounterGuard guard(depth); 
		nodeCount++;
		if (board.isKingDied(country))
		{
			return -100000000 + depth;
		}
		auto estimate = getEstimatedValue(country);
		if (estimate < alpha)
		{
			return estimate;
		}

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
			if (candidate.empty())
			{
				return estimate;
			}
		}
		else
		{
			appendSafeAction(candidate, country);
			if (candidate.empty())
			{
				return alpha;
			}
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
		return alpha;
	}  

	uint64_t tryTakeAction(SimpleList<uint64_t, 100>& candidate, uint64_t action)
	{
		for (int i = 0; i < candidate.length; i++)
		{
			if (candidate[i] == action)
			{ 
				for (int j = i+1; j < candidate.length; j++)
				{
					candidate[j - 1] = candidate[j];
				}
				candidate.pop_back();
				return action;
			}
		}
		return 0;
	}

	uint64_t takeBestAction(SimpleList<uint64_t, 100>& candidate, int country,int& step)
	{
		Debug::assert(!candidate.empty());
		enum Step
		{
			Transposition,
			AssaultInit, 
			Kill,
			HistoryInit,
			History,
		};
		uint64_t resultAction;
		if (step == Transposition)
		{
			step = Kill;
			auto rootValue = board.getHash();
			if (hasHash(rootValue))
			{
				auto action = getHash(rootValue).bestAction;
				resultAction = tryTakeAction(candidate, action);
				if (resultAction != 0)
				{
					return resultAction;
				}
			}
		}
		if (step == Kill)
		{
			for (int i = 0; i < 2; i++)
			{
				auto action = killAction[depth][i];
				if (tryTakeAction(candidate, action))
				{
					return action;
				}
			}
			step = HistoryInit;
		}
		if (step == HistoryInit)
		{//to do		
			step = History;
			std::pair<int, uint64_t> buff[100];
			int buffSize = candidate.length;
			for (int i = 0; i < candidate.length; i++)
			{
				auto action = candidate[i];
				buff[i].second = action;   
				buff[i].first = historyTable[country][board.get_fromIndex(action)][board.get_desIndex(action)];
			}
			if (isShallowDepth())
			{
				std::stable_sort(buff, buff + buffSize);
			}
			else
			{
				std::sort(buff, buff + buffSize);
			}
			candidate.clear();
			for (int i = buffSize - 1; i >= 0; i--)
			{
				candidate.push_back(buff[i].second);
			}
		}   
		if (step == History)
		{
			auto action = candidate[0];
			//Debug::assert(board.get_coveredType(action) == None);
			return tryTakeAction(candidate, action);
		}
		return 0;
	}

	bool isShallowDepth()const
	{
		return depth < 5;
	}

	void printHashTableState()
	{
		int cnt = 0;
		for (int i = 0; i < hashTableMask; i++)
		{
			if (hashTable[i].key != 0)
			{
				cnt++;
			}
		}
		std::cout << "hssh table " << cnt << " / " << hashTableMask << " (" << (double)cnt / hashTableMask << ") \n";
	}

	int search(int alpha, int beta, int country, int depthLeft,uint64_t& actionResult,bool ableNullMove=true)
	{  
		CounterGuard guard(depth);
		nodeCount++;
		if (board.isKingDied(country))
		{
			return -10000000 + depth;
		}
		if (depthLeft <= 0)
		{
			return quiescentSearch(alpha, beta, country);
			//return getEstimatedValue(country);
		} 

		if (hasHash(board.getHash()))
		{
			auto& hash = getHash(board.getHash());
			if (hash.depth >= depthLeft)
			{
				if (hash.type == HashTable::PV)
				{
					return hash.value;
				}
				else if (hash.type == HashTable::Lowerbound)
				{
					if (hash.value > alpha)
					{
						alpha = hash.value;
					}
				}
				else if (hash.type == HashTable::Upperbound)
				{
					if (hash.value > beta)
					{
						beta = hash.value;
					}
				}
				if (alpha >= beta)
				{
					return hash.value;
				}
			}
		}  

		SimpleList<uint64_t, 100> candidate;
		if (board.isKingSafe(country))
		{
			if (ableNullMove)
			{ 
				auto R = depthLeft > 6 ? 4 : 3;
				uint64_t actionResult = 0;
				auto value = -search(-beta, -beta + 1, !country, depthLeft - R - 1, actionResult,false);
				if (value >= beta)
				{
					depthLeft -= 4;
					if (depthLeft <= 0)
					{
						return quiescentSearch(alpha, beta, country);
						//return getEstimatedValue(country);
					}
				}
			}
			if (isShallowDepth())
			{
				appendSortedAction(candidate, country);
			}
			else
			{
				appendAllAction(candidate, country);
			}
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
		
		int takeStep = 0;
		while(!candidate.empty())
		{
			auto action = takeBestAction(candidate, country, takeStep); 
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
		if (!pv)
		{// upperbound
			updateHash(board.getHash(), depthLeft, HashTable::Upperbound, alpha);
		}
		else
		{
			if (alpha >= beta)
			{// lowerbound
				updateHash(board.getHash(), depthLeft, HashTable::Lowerbound, alpha,actionResult);
				addKillAction(actionResult);
				historyTable[country][board.get_fromIndex(actionResult)][board.get_desIndex(actionResult)] += 1 << depthLeft;
			}
			else
			{// pv
				updateHash(board.getHash(), depthLeft, HashTable::PV, alpha,actionResult);
				addKillAction(actionResult);
				historyTable[country][board.get_fromIndex(actionResult)][board.get_desIndex(actionResult)] += 1 << depthLeft;
			}
		} 
		return alpha;
	}
	int BNS(int alpha, int beta, int country, int depthLeft, uint64_t& actionResult)
	{ 
		CounterGuard guard(depth);
		nodeCount++;
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
			//appendSortedAction(candidate, country);
			appendAllAction(candidate, country);
		}
		else
		{
			appendSafeAction(candidate, country);
			if (candidate.empty())
			{
				return alpha;
			}
		}

		auto subtreeCount = candidate.length;
		auto betterCount = 0;
		alpha =-10000;
		beta = 10000;
		bool able[1000]{};
		for (int i = 0; i < 1000; i++)
		{
			able[i] = true;
		}
		while (alpha+1 < beta && betterCount != 1)
		{
			//auto test = int(alpha + std::max(1.0, (beta - alpha)*(subtreeCount - 1) / double(subtreeCount)));
			//if (test == alpha)
			//{
			//	test++;
			//}		
			//if (test == beta)
			//{
			//	test--;
			//}
			auto test = (alpha + beta) / 2;
			betterCount = 0;
			for (int i = 0; i < candidate.length; i++)
			{
				if (!able[i])
				{
					continue;
				}
				uint64_t subBsetAction;
				auto bestVal = -search(-test, -test + 1, country, depthLeft, subBsetAction);
				if (bestVal >= test)
				{
					betterCount++;
					actionResult = candidate[i]; 
					if (betterCount >= 2)
					{
						break;
					}
				}
				else
				{
					able[i] = false;
				}
			}
			if (betterCount == 0)
			{
				beta = test;
			}
			else
			{
				alpha = test;
				subtreeCount = betterCount;
			}
		}
		return 0;
	}
	void printBoard()const 
	{
		board.printBoard();
	}
	void printAction(uint64_t action)const
	{
		board.printAction(action);
	}
	void addKillAction(uint64_t action)
	{
		killAction[1][depth] = killAction[1][depth];
		killAction[0][depth] = action;
	}

private:
	BitBoard board;
	int depth = 0;
	int nodeCount = 0;
	uint64_t killAction[100][2]{};
	static int historyTable[2][256][256];
};

