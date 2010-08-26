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
	LogAssert((_TEXT("Testing the assert log")));
	LogToDo((_TEXT("Testing the todo log")));
	LogPerformance((_TEXT("Testing the performance log")));

	return 0;
}

