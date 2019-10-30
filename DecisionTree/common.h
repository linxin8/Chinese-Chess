#pragma once 

struct Position
{
	int x;
	int y;
	constexpr Position(int x, int y) :x(x), y(y) {}
};
 

// [min,max)
inline bool isInRange(int value,int min, int max)
{
	return min <= value && value < max;
}

template<typename T, int N>
struct SimpleList
{
	T data[N];
	int length = 0;
	void push_back(const T& r) { data[length++] = r; }
	int pop_back() { length--; }
	T& front()const { return data[0]; }
	T& back()const { return data[length - 1]; }
	T& operator[](int i)const { return data[i]; }
	T* begin() { return data; };
	T* end() { return data + length; };
	const T* begin()const { return data; };
	const T* end()const { return data + length; };
};