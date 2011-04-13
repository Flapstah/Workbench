#include "stdafx.h"

#include "common/ilogfile.h"
#include "common/itime.h"

#include "application/consoleapplication.h"

//==============================================================================

namespace engine
{
	//============================================================================

	CConsoleApplication::CConsoleApplication(void)
	{

	}

	//============================================================================

	CConsoleApplication::~CConsoleApplication(void)
	{

	}

	//============================================================================

	bool CConsoleApplication::Update(double frameTime)
	{
		LogDebug("fps: %f", GetFrameRate(true));

		return true;
	}

	//============================================================================

	//============================================================================
} // End [namespace engine]

//==============================================================================
// [EOF]
