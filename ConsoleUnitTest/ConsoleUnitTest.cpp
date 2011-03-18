// ConsoleUnitTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>

#include "common/ilogfile.h"
#include "application/consoleapplication.h"

int _tmain(int argc, TCHAR* argv[])
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

	engine::CConsoleApplication theApp;
	theApp.Initialise(argc, argv);
	theApp.SetFrameRate(30);
	theApp.StartUp();

	for (uint32 index = 0; index < 300; ++index)
	{
		theApp.Update();
	}

	theApp.ShutDown();
	theApp.Uninitialise();

	Log("Last entry");
	return 0;
}
