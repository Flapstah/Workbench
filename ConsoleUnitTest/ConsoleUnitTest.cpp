// ConsoleUnitTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>

#include "common/itime.h"

int _tmain(int argc, _TCHAR* argv[])
{
	const engine::ISystemClock* pSystemClock = engine::GetSystemClock();

	std::cout << pSystemClock->GetLocalDateString() << std::endl;
	std::cout << pSystemClock->GetLocalTimeString() << std::endl;

	return 0;
}

