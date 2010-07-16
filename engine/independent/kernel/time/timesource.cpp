#include "stdafx.h"

#include "timesource.h"

//==============================================================================

namespace engine
{
	//============================================================================

	char* CTimeSource::GetTimeString(void) const
	{
		char buffer[32];
		sprintf_s(buffer, sizeof(buffer), "%lf", m_currentTime);

		return buffer;
	}

	//============================================================================
} // End [namespace engine]

//==============================================================================
// [EOF]
