#include "stdafx.h"

#include "kernel/time/systemclock.h"

//==============================================================================

namespace engine
{
	//============================================================================

	//----------------------------------------------------------------------------
	// The global instance of the system clock
	//----------------------------------------------------------------------------
	CSystemClock g_systemClock;

	//============================================================================

	const ISystemClock* GetSystemClock(void)
	{
		return &g_systemClock;
	}

	//============================================================================
} // End [namespace engine]

//==============================================================================
// [EOF]
