// ConsoleUnitTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>

#include "common/ilogfile.h"

int _tmain(int argc, _TCHAR* argv[])
{
  IGNORE_PARAMETER(argc);
  IGNORE_PARAMETER(argv);

	Log("Testing the log");
	LogError("Testing the error log");
	LogWarning("Testing the warning log");
	LogToDo("Testing the todo log");
	LogPerformance("Testing the performance log");

	assert(1 == 0);
	assertf(1 == 0, "Hopefully, we're asserting that 1 does not equal 0...");

	Log("Last entry");
	return 0;
}
