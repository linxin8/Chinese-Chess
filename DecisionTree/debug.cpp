#include<iostream>
#include<string>
#include "debug.h"  

#ifdef DEBUGON
// for tracing debug info
void Debug::assertTracingLog(bool on, const char* info)
{ 
	if (!on)
	{ 
		std::cout << info << std::endl;
	}
} 
#endif