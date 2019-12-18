#pragma once
#include<cstdint>
#include<cstring>

template<typename... Args>
constexpr uint64_t bitsAt(Args&&... args)
{
	return ((uint64_t(1) << args) | ...);
}

template<typename T, int N>
struct SimpleList
{
	T data[N]{};
	int length = 0;
	SimpleList() {};
	SimpleList(const SimpleList<T, N>& r) { this->length = r.length; for (int i = 0; i < length; i++) data[i] = r.data[i]; }
	SimpleList& operator=(const SimpleList<T, N>& r) { this->length = r.length; for (int i = 0; i < length; i++) data[i] = r.data[i]; return *this; }
	void push_back(const T& r) { data[length++] = r; }
	void pop_back() { length--; }
	T& front()const { return data[0]; }
	T& back() { return data[length - 1]; }
	T& operator[](int i)const { return data[i]; }
	T& operator[](int i) { return data[i]; }
	T* begin() { return data; };
	T* end() { return data + length; };
	T& next() { return data[length++]; }
	const T* begin()const { return data; };
	const T* end()const { return data + length; };
	bool empty()const { return length == 0; }
	void clear() { length = 0; }
};

enum ChessType
{
	None,
	Pawn,
	Cannon,
	Rook,
	Knight,
	Elephant,
	Guard,
	King
};

namespace Action
{
	constexpr uint64_t fromIndexFlag = bitsAt(0, 1, 2, 3, 4, 5, 6, 7);
	constexpr uint64_t desIndexFlag = bitsAt(8, 9, 10, 11, 12, 13, 14, 15);
	constexpr uint64_t coveredTypeFlag = bitsAt(16, 17, 18);
}

enum ChessCountry
{
	Black,
	Red
};

struct ChessTarget
{
	SimpleList<uint64_t, 17> moveableList;
	SimpleList<uint64_t, 8> assaultableList;
	SimpleList<uint64_t, 8> defendableList;
	void clear() { moveableList.clear(); assaultableList.clear(); defendableList.clear(); }
};

extern int chessValue[8]; 
extern int positionValue[2][8][256]; 
extern int chessConfilictValue[8]; 
extern int moveShiftValue[8];

struct Debug
{
	static void assert(bool condition)
	{
#ifdef DEBUG 
		if (condition)
		{
			std::cout << "error condition\n";
		}
#endif
	}
};
 
class BitBoard
{
public:
#define SETTER_GETTER(x)												\
	static constexpr uint64_t get_##x(uint64_t value)					\
	{																	\
		return (value & x##Flag) >> countFlagIndex(x##Flag);			\
	}																	\
	static constexpr void set_##x(uint64_t& value,uint64_t flagValue)	\
	{																	\
		value &= ~ x##Flag;												\
		value |= (flagValue << countFlagIndex(x##Flag));				\
	}
	SETTER_GETTER(index);
	SETTER_GETTER(country);
	SETTER_GETTER(type);
	SETTER_GETTER(legal);
	SETTER_GETTER(globalId);
	SETTER_GETTER(typeId);
	SETTER_GETTER(alive);
#undef SETTER_GETTER   
public:  
	BitBoard(int map[10][9])
	{
		initBoard();
		initKing();
		initGuard();
		initElephant();
		initKnight();
		initRook();
		initCannon();
		initPawn();
		load(map);
		updateAllTarget();
	}

	void beforeUpdateTarget(uint64_t chess)
	{
		auto& target = getTargetByChess(chess);
		for (int i = 0; i < target.assaultableList.length; i++)
		{
			auto c = target.assaultableList[i];
			conflictCountById[get_globalId(c)] += 1;
		} 		
		for (int i = 0; i < target.defendableList.length; i++)
		{
			auto c = target.defendableList[i];
			conflictCountById[get_globalId(c)] -= 1;
		}
	}

	void afterUpdateTarget(uint64_t chess)
	{ 
		auto& target = getTargetByChess(chess);
		for (int i = 0; i < target.assaultableList.length; i++)
		{
			auto c = target.assaultableList[i];
			conflictCountById[get_globalId(c)] -= 1;
		}
		for (int i = 0; i < target.defendableList.length; i++)
		{
			auto c = target.defendableList[i];
			conflictCountById[get_globalId(c)] += 1;
		}
	}

	void updateTarget(uint64_t chess)
	{
		beforeUpdateTarget(chess);
		switch (ChessType(get_type(chess)))
		{
		case None:
			break;
		case ChessType::Pawn:
			updatePawnTarget(chess);
			break;
		case ChessType::Cannon:
			updateCannonTarget(chess);
			break;
		case ChessType::Rook:
			updateRookTarget(chess);
			break;
		case ChessType::Knight:
			updateKnightTarget(chess);
			break;
		case ChessType::Elephant:
			updateElephantTarget(chess);
			break;
		case ChessType::Guard:
			updateGuardTarget(chess);
			break;
		case ChessType::King:
			updateKingTarget(chess);
			break;
		default:
			break;
		}
		afterUpdateTarget(chess);
	}

	void updateAllTarget()
	{
		for (int i = 0; i < 32; i++)
		{
			auto chess = boardById[i];
			if (get_alive(chess))
			{
				updateTarget(chess);
			}
		}
	}

	int getEstimatedValue()
	{
		int sum = 0;
		for (int i = 0; i < 32; i++)
		{
			auto chess = boardById[i];
			if (chess&&get_alive(chess))
			{
				auto type = get_type(chess);
				auto index = get_index(chess);
				auto country = get_country(chess);
				auto moveLength = getTargetByChess(chess).moveableList.length;
				if (country == Black)
				{ 
					sum += chessValue[type] + positionValue[country][type][index] + (moveLength << moveShiftValue[type]);
					if (conflictCountById[i] < 0)
					{
						sum -= chessConfilictValue[type];
					}
				}
				else
				{
					sum -= chessValue[type] + positionValue[country][type][index] + (moveLength << moveShiftValue[type]);
					if (conflictCountById[i] < 0)
					{
						sum += chessConfilictValue[type];
					}
				}
			}
		}
		return sum;
	}

	uint64_t getChess(uint64_t index)
	{
		return board[index];
	}

	void load(int data[10][9])
	{
		int globalIdCounter = 2;
		int typeIdCounter[10]{};
		std::memset(boardById, 0, sizeof(boardById));
		std::memset(conflictCountById, 0, sizeof(conflictCountById));

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				auto type = data[i][j] % 10;
				auto country = (data[i][j] / 10 == 1 ? Black : Red);
				auto index = (i + 3) * 16 + (j + 3);
				auto& chess = board[index];
				set_country(chess, country);
				set_type(chess, type);
				if (type != None)
				{
					if (type == King)
					{ 
						set_globalId(chess, country == Black ? blackKingGlobalId : redKingGlobalId);
					}
					else
					{ 
						set_globalId(chess, globalIdCounter); 
					}
					set_typeId(chess, typeIdCounter[type]);
					set_alive(chess, true);
					typeIdCounter[type]++;
					if (type == King)
					{ 
						boardById[country == Black ? blackKingGlobalId : redKingGlobalId] = chess;
					}
					else
					{
						boardById[globalIdCounter] = chess;
						globalIdCounter++;
					}
				}
			}
		}
	}


	ChessTarget& getTargetByChess(uint64_t chess)
	{
		static ChessTarget nullTarget;
		ChessType type = (ChessType)get_type(chess);
		auto typeId = get_typeId(chess);
		switch (type)
		{
		case None:
			return nullTarget;
		case ChessType::Pawn:
			return pawnTargetStack[typeId].back();
		case ChessType::Cannon:
			return cannonTargetStack[typeId].back();
		case ChessType::Rook:
			return rookTargetStack[typeId].back();
		case ChessType::Knight:
			return knightTargetStack[typeId].back();
		case ChessType::Elephant:
			return elephantTargetStack[typeId].back();
		case ChessType::Guard:
			return guardTargetStack[typeId].back();
		case ChessType::King:
			return kingTargetStack[typeId].back();
		default:
			Debug::assert(false);
			break;
		}
		return nullTarget;
	}

	ChessTarget& getTargetByIndex(int index)
	{
		return  getTargetByChess(getChess(index));
	}

private:
	// chess info 0-31 bits
	static constexpr uint64_t countryFlag = bitsAt(0);
	static constexpr uint64_t typeFlag = bitsAt(1, 2, 3);
	static constexpr uint64_t globalIdFlag = bitsAt(4, 5, 6, 7, 8);
	static constexpr uint64_t typeIdFlag = bitsAt(9, 10, 11, 12);
	static constexpr uint64_t aliveFlag = bitsAt(13);
	static constexpr uint64_t chessInfoMask = 0b11111111'11111111'11111111'11111111;
	// chess position info 32-63 bits
	static constexpr uint64_t indexFlag = bitsAt(32, 33, 34, 35, 36, 37, 38, 39);
	static constexpr uint64_t legalFlag = bitsAt(40);
	static constexpr uint64_t chessPositionInfoMask = 0b11111111'11111111'11111111'11111111'00000000'00000000'00000000'00000000;

	static_assert((countryFlag & typeFlag & globalIdFlag & typeIdFlag & indexFlag & legalFlag) == 0ULL);
	static_assert((chessInfoMask & chessPositionInfoMask) == 0ULL);
	static constexpr uint64_t countFlagIndex(uint64_t flag)
	{
		int counter = 0;
		while (flag && !(flag & 1))
		{
			counter++;
			flag >>= 1;
		}
		return counter;
	}
	//static_assert((countFlagIndex(chessPositionInfoMask)) == 32);

public:
	uint64_t board[256];
	void initBoard()
	{
		std::memset(board, 0, sizeof(board));
		for (int i = 0; i < 256; i++)
		{
			auto x = i % 16 - 3;
			auto y = i / 16 - 3;
			board[i] = 0;
			if (x < 0 || x >= 9 || y < 0 || y >= 10)
			{
				set_legal(board[i], false);
			}
			else
			{
				set_index(board[i], i);
				set_legal(board[i], true);
			}
		}
	}

private:
	void updateKnightTarget(uint64_t chess)
	{
		Debug::assert(get_type(chess) == ChessType::Knight);
		auto index = get_index(chess);
		auto typeId = get_typeId(chess);
		auto country = get_country(chess);
		auto& target = knightTargetStack[typeId].next();
		target.clear();
		for (int i = 0; i < 4; i++)
		{
			auto handicapIndex = knightHandicap[index][i];
			if (handicapIndex&&										// if handicap is legal
				get_type(getChess(handicapIndex)) == None)	// if handicap is none
			{
				for (int j = 0; j < 2; j++)
				{
					auto candidateIndex = knightCandidate[index][i * 2 + j];
					if (candidateIndex)// if candidate is legal
					{
						auto targetChess = getChess(candidateIndex);
						if (get_type(targetChess) == None)// if target chess type is none
						{
							target.moveableList.push_back(targetChess);
						}
						else if (get_country(targetChess) == country)// if target chess type is the same country
						{
							target.defendableList.push_back(targetChess);
						}
						else // if target chess type is not the same country
						{
							target.assaultableList.push_back(targetChess);
						}
					}
				}
			}
		}
	}
	void initKnight()
	{
		constexpr int han[4] = { -16,-1,16,1 };
		constexpr int can[8] = { -16 - 16 - 1,-16 - 16 + 1,-1 - 1 - 16,-1 - 1 + 16,16 + 16 - 1,16 + 16 + 1,1 + 1 - 16,1 + 1 + 16 };
		std::memset(knightCandidate, 0, sizeof(knightCandidate));
		std::memset(knightHandicap, 0, sizeof(knightHandicap));
		for (int i = 0; i < 4; i++)
		{
			knightTargetStack[i].clear();
		}
		for (int i = 0; i < 256; i++)
		{
			if (!get_legal(getChess(i)))
			{
				continue;
			}
			for (int j = 0; j < 4; j++)
			{
				for (int k = 0; k < 2; k++)
				{
					auto c = getChess(i + can[j * 2 + k]);
					knightCandidate[i][j * 2 + k] = get_legal(c) ? get_index(c) : 0;
				}
				if (knightCandidate[i][j * 2] || knightCandidate[i][j * 2 + 1])
				{
					knightHandicap[i][j] = i + han[j];
				}
			}
		}
	}

	void updateCannonTarget(uint64_t chess)
	{
		Debug::assert(get_type(chess) == ChessType::Cannon);
		auto typeId = get_typeId(chess);
		auto country = get_country(chess);
		auto& target = cannonTargetStack[typeId].next();
		auto currrentIndex = get_index(chess);
		target.clear();
		for (int i = 0; i < 4; i++)
		{
			auto d = cannonDirection[i];
			for (auto index = currrentIndex + d; ; index += d)
			{
				auto targetChess = getChess(index);
				if (!get_legal(targetChess))
				{
					break;
				}
				if (get_type(targetChess) == None)
				{
					target.moveableList.push_back(targetChess);
				}
				else
				{
					for (index += d; ; index += d)
					{
						targetChess = getChess(index);
						if (!get_legal(targetChess))
						{
							break;
						}
						if (get_type(targetChess) != None)
						{
							if (get_country(targetChess) != country)
							{
								target.assaultableList.push_back(targetChess);
								break;
							}
							else
							{
								target.defendableList.push_back(targetChess);
								break;
							}
						}
					}
					break;
				}
			}
		}
	}
	void initCannon()
	{
		for (int i = 0; i < 4; i++)
		{
			cannonTargetStack[i].clear();
		}
	}

	void updateRookTarget(uint64_t chess)
	{
		Debug::assert(get_type(chess) == ChessType::Rook);
		auto typeId = get_typeId(chess);
		auto country = get_country(chess);
		auto& target = rookTargetStack[typeId].next();
		auto currrentIndex = get_index(chess);
		target.clear();
		for (int i = 0; i < 4; i++)
		{
			auto d = rookDirection[i];
			for (auto index = currrentIndex + d; ; index += d)
			{
				auto targetChess = getChess(index);
				if (!get_legal(targetChess))
				{
					break;
				}
				if (get_type(targetChess) == None)
				{
					target.moveableList.push_back(targetChess);
				}
				else if (get_country(targetChess) != country)
				{
					target.assaultableList.push_back(targetChess);
					break;
				}
				else
				{
					target.defendableList.push_back(targetChess);
					break;
				}
			}
		}
	}
	void initRook()
	{
		for (int i = 0; i < 4; i++)
		{
			rookTargetStack[i].clear();
		}
	}

	void updateElephantTarget(uint64_t chess)
	{
		Debug::assert(get_type(chess) == ChessType::Elephant);
		auto index = get_index(chess);
		auto typeId = get_typeId(chess);
		auto country = get_country(chess);
		auto& target = elephantTargetStack[typeId].next();
		target.clear();
		for (int i = 0; i < 4; i++)
		{
			auto handicapIndex = elephantHandicap[index][i];
			if (handicapIndex&&										// if handicap is legal
				get_type(getChess(handicapIndex)) == None)	// if handicap is none
			{
				auto candidateIndex = elephantCandidate[index][i];
				if (candidateIndex)// if candidate is legal
				{
					auto targetChess = getChess(candidateIndex);
					if (get_type(targetChess) == None)// if target chess type is none
					{
						target.moveableList.push_back(targetChess);
					}
					else if (get_country(targetChess) == country)// if target chess type is the same country
					{
						target.defendableList.push_back(targetChess);
					}
					else // if target chess type is not the same country
					{
						target.assaultableList.push_back(targetChess);
					}
				}
			}
		}
	}
	void initElephant()
	{
		constexpr int han[4] = { -16 - 1,-16 + 1,16 - 1,16 + 1 };
		constexpr int can[4] = { -16 - 16 - 1 - 1,-16 - 16 + 1 + 1,16 + 16 - 1 - 1 ,16 + 16 + 1 + 1 };
		std::memset(elephantCandidate, 0, sizeof(elephantCandidate));
		std::memset(elephantHandicap, 0, sizeof(elephantHandicap));
		for (int i = 0; i < 4; i++)
		{
			elephantTargetStack[i].clear();
		}
		for (int i = 0; i < 256; i++)
		{
			if (!get_legal(getChess(i)))
			{
				continue;
			}
			for (int j = 0; j < 4; j++)
			{
				auto x1 = i % 16 - 3;
				auto y1 = i / 16 - 3;
				auto x2 = (i + can[j]) % 16 - 3;
				auto y2 = (i + can[j]) / 16 - 3;
				if (y1 >= 0 && y2 >= 0 && y1 < 10 && y2 < 10 && x1 >= 0 && x2 >= 0 && x1 < 9 && x2 < 9)
				{
					if ((y1 < 5 && y2 < 5) ||
						(y1 >= 5 && y2 >= 5))
					{
						elephantCandidate[i][j] = i + can[j];
						elephantHandicap[i][j] = i + han[j];
					}
				}
			}
		}
	}

	void updateKingTarget(uint64_t chess)
	{
		Debug::assert(get_type(chess) == ChessType::King);
		auto country = get_country(chess);
		auto typeId = get_typeId(chess);
		auto& target = kingTargetStack[typeId].next();
		auto currentIndex = get_index(chess);
		target.clear();
		for (int i = 0; i < 4; i++)
		{
			auto c = kingCandidate[currentIndex][i];
			if (c == 0)
			{
				continue;
			}
			auto targetChess = getChess(c);
			if (get_legal(targetChess))
			{
				if (get_type(targetChess) == None)
				{
					target.moveableList.push_back(targetChess);
				}
				else if (get_country(targetChess) != country)
				{
					target.assaultableList.push_back(targetChess);
				}
				else
				{
					target.defendableList.push_back(targetChess);
				}
			}
		}
		uint64_t d = (country == Black ? 16 : -16);
		for (auto index = currentIndex + d;; index += d)
		{
			auto targetChess = getChess(index);
			auto type = get_type(targetChess);
			if (type == ChessType::King)
			{
				target.assaultableList.push_back(targetChess);
			}
			else if (type != None)
			{
				break;
			}
		}
	}
	void initKing()
	{
		std::memset(kingCandidate, 0, sizeof(kingCandidate));
		constexpr int dir[4] = { -16,-1,16,1 };
		for (int i = 0; i < 2; i++)
		{
			kingTargetStack[i].clear();
		}
		for (int i = 0; i < 256; i++)
		{
			auto chess = getChess(i);
			if (!get_legal(chess))
			{
				continue;
			}
			auto currentIndex = get_index(chess);
			for (int j = 0; j < 4; j++)
			{
				auto d = dir[j];
				auto targetChess = getChess(i + d);
				auto targetIndex = get_index(targetChess);
				auto x = targetIndex % 16 - 3;
				auto y = targetIndex / 16 - 3;
				if ((3 <= x && x <= 5) &&
					((0 <= y && y <= 2) || (7 <= y && y <= 9))
					)
				{
					kingCandidate[i][j] = i + d;
				}
			}
		}
	}

	void updatePawnTarget(uint64_t chess)
	{
		Debug::assert(get_type(chess) == ChessType::Pawn);
		auto typeId = get_typeId(chess);
		auto country = get_country(chess);
		auto& target = pawnTargetStack[typeId].next();
		auto currentIndex = get_index(chess);
		target.clear();
		for (int i = 0; i < 4; i++)
		{
			auto c = pawnCandidate[country][currentIndex][i];
			if (c == 0)
			{
				continue;
			}
			auto targetChess = getChess(c);
			if (get_type(targetChess) == None)
			{
				target.moveableList.push_back(targetChess);
			}
			else if (get_country(targetChess) != country)
			{
				target.assaultableList.push_back(targetChess);
			}
			else
			{
				target.defendableList.push_back(targetChess);
			}
		}
	}
	void initPawn()
	{
		constexpr int dir[4] = { -16,-1,16,1 };
		std::memset(pawnCandidate, 0, sizeof(pawnCandidate));
		for (int i = 0; i < 10; i++)
		{
			pawnTargetStack[i].clear();
		}
		for (int i = 0; i < 256; i++)
		{
			if (!get_legal(getChess(i)))
			{
				continue;
			}
			for (int j = 0; j < 4; j++)
			{
				auto d = dir[j];
				auto c = getChess(i + d);
				if (!get_legal(c))
				{
					continue;
				}
				if (d == -16)
				{
					pawnCandidate[Red][i][j] = i + d;
				}
				else if (d == 16)
				{
					pawnCandidate[Black][i][j] = i + d;
				}
				else
				{
					auto y = i / 16 - 3;
					if (y < 5)
					{
						pawnCandidate[Red][i][j] = i + d;
					}
					else
					{
						pawnCandidate[Black][i][j] = i + d;
					}
				}
			}
		}
	}

	void updateGuardTarget(uint64_t chess)
	{
		Debug::assert(get_type(chess) == ChessType::Guard);
		auto country = get_country(chess);
		auto typeId = get_typeId(chess);
		auto& target = guardTargetStack[typeId].next();
		auto currentIndex = get_index(chess);
		target.clear();
		for (int i = 0; i < 4; i++)
		{
			auto c = guardCandidate[currentIndex][i];
			if (c == 0)
			{
				continue;
			}
			auto targetChess = getChess(c);
			if (get_type(targetChess) == None)
			{
				target.moveableList.push_back(targetChess);
			}
			else if (get_country(targetChess) != country)
			{
				target.assaultableList.push_back(targetChess);
			}
			else
			{
				target.defendableList.push_back(targetChess);
			}
		}
	}
	void initGuard()
	{
		std::memset(guardCandidate, 0, sizeof(guardCandidate));
		constexpr int dir[4] = { -16 - 1,-16 + 1,16 - 1,16 + 1 };
		for (int i = 0; i < 4; i++)
		{
			guardTargetStack[i].clear();
		}
		for (int i = 0; i < 256; i++)
		{
			auto chess = getChess(i);
			if (!get_legal(chess))
			{
				continue;
			}
			auto currentIndex = get_index(chess);
			for (int j = 0; j < 4; j++)
			{
				auto d = dir[j];
				auto targetChess = getChess(i + d);
				auto targetIndex = get_index(targetChess);
				auto x = targetIndex % 16 - 3;
				auto y = targetIndex / 16 - 3;
				if ((3 <= x && x <= 5) &&
					((0 <= y && y <= 2) || (7 <= y && y <= 9))
					)
				{
					guardCandidate[i][j] = i + d;
				}
			}
		}
	}


private:
	uint64_t knightHandicap[256][4];
	uint64_t knightCandidate[256][8];
	SimpleList<ChessTarget, 100> knightTargetStack[4];
	static constexpr int cannonDirection[4] = { -16,-1,16,1 };
	SimpleList<ChessTarget, 100> cannonTargetStack[4];
	SimpleList<ChessTarget, 100> rookTargetStack[4];
	static constexpr int rookDirection[4] = { -16,-1,16,1 };
	uint64_t elephantHandicap[256][4];
	uint64_t elephantCandidate[256][4];
	SimpleList<ChessTarget, 100> elephantTargetStack[4];
	SimpleList<ChessTarget, 100> pawnTargetStack[10];
	uint64_t pawnCandidate[2][256][4]; 
	SimpleList<ChessTarget, 100> kingTargetStack[2];
	uint64_t kingCandidate[256][4]; 
	SimpleList<ChessTarget, 100> guardTargetStack[4];
	uint64_t guardCandidate[256][4];
	int conflictCountById[32];
	uint64_t boardById[32];
	static constexpr int blackKingGlobalId = 0;
	static constexpr int redKingGlobalId = 1;
};