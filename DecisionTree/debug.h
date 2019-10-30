#pragma once 
 
#ifndef DEBUGON
//#define DEBUGON
#ifdef DEBUGON
class Debug
{
public:
	Debug() = delete;
	~Debug() = delete;
	static void assert(bool condition, const char* log)
	{
		assertTracingLog(condition, log);
	}

	static void assert(bool condition)
	{
		assert(condition, "assert failed!");
	} 

	static void notReachable()
	{
		assert(false,"not reachable");
	}
private:
	static void assertTracingLog(bool on, const char* info);
};
#else
class Debug
{
public:
	static void assert(bool condition) {};

	static void assert(bool condition, const char* log) {};

	static void notReachable() {};
};
#endif
#endif