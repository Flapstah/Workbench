// ConsoleUnitTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>

#include "common/ilogfile.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Log((_TEXT("Testing the log")));
	LogError((_TEXT("Testing the error log")));
	LogWarning((_TEXT("Testing the warning log")));
	LogToDo((_TEXT("Testing the todo log")));
	LogPerformance((_TEXT("Testing the performance log")));

	assert(1 == 0);
	assertf(1 == 0, (_TEXT("Hopefully, we're asserting that 1 does not equal 0...")));

	return 0;
}

