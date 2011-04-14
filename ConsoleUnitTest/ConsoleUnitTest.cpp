// ConsoleUnitTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>

#include "common/ilogfile.h"
#include "common/itime.h"
#include "application/consoleapplication.h"

int main(int argc, char* argv[])
{
  IGNORE_PARAMETER(argc);
  IGNORE_PARAMETER(argv);

	//Log("Testing the log");
	//LogError("Testing the error log");
	//LogWarning("Testing the warning log");
	//LogToDo("Testing the todo log");
	//LogPerformance("Testing the performance log");

	//assert(1 == 0);
	//assertf(1 == 0, "Hopefully, we're asserting that 1 does not equal 0...");

	engine::CConsoleApplication theApp;
	theApp.Initialise(argc, argv);
	theApp.SetFrameRate(80);
	theApp.StartUp();

	double startTime = engine::GetRealTimeClock()->GetTickTimePrecise();

	for (uint32 index = 0; index < 1200; ++index)
	{
		theApp.Update();
	}

	double endTime = engine::GetRealTimeClock()->GetTickTimePrecise();
	Log("time: %.02f", endTime - startTime);

	theApp.ShutDown();
	theApp.Uninitialise();

	return 0;
}
