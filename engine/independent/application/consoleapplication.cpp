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

	bool CConsoleApplication::Update(double frameTime, uint32 frameCount)
	{
		Log("Frame %i(%.03f), fps %.02f : %0.03f %0.03f %0.03f %0.03f %0.03f %0.03f %0.03f %0.03f %0.03f %0.03f %0.03f %0.03f %0.03f %0.03f %0.03f %0.03f", frameCount, frameTime, GetFrameRate(true), m_frameTimeBuffer[0], m_frameTimeBuffer[1], m_frameTimeBuffer[2], m_frameTimeBuffer[3], m_frameTimeBuffer[4], m_frameTimeBuffer[5], m_frameTimeBuffer[6], m_frameTimeBuffer[7], m_frameTimeBuffer[8], m_frameTimeBuffer[9], m_frameTimeBuffer[10], m_frameTimeBuffer[11], m_frameTimeBuffer[12], m_frameTimeBuffer[13], m_frameTimeBuffer[14], m_frameTimeBuffer[15]);

		return true;
	}

	//============================================================================

	//============================================================================
} // End [namespace engine]

//==============================================================================
// [EOF]
