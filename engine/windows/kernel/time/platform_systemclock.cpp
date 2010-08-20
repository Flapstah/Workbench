#include "stdafx.h"


#include "kernel/time/systemclock.h"

//==============================================================================

namespace engine
{
	//============================================================================

	void CSystemClock::Platform_Initialise(void)
	{
		LARGE_INTEGER frequency;

		::QueryPerformanceFrequency(&frequency);
		m_secondsPerTick = 1.0 / frequency.QuadPart;

		Platform_Tick();
		m_frameTime = 0.0;
	}

	//============================================================================

	void CSystemClock::Platform_Uninitialise(void)
	{
		// Nothing to do
	}

	//============================================================================

	bool CSystemClock::Platform_Tick(void)
	{
		LARGE_INTEGER time;

		::QueryPerformanceCounter(&time);
		double currentTime = time.QuadPart * m_secondsPerTick;
		m_frameTime = currentTime - m_currentTime;
		m_currentTime = currentTime;

		return true;
	}

	//============================================================================

	TCHAR* CSystemClock::Platform_GetLocalDateString(void) const
	{
		::time_t absoluteTime;
		::time(&absoluteTime);

		::tm localTime;
		::localtime_s(&localTime, &absoluteTime);

		memset(m_localDate, 0, sizeof(TCHAR) * SYSTEM_CLOCK_DATE_BUFFER_SIZE);
		::_tcsftime(m_localDate, SYSTEM_CLOCK_DATE_BUFFER_SIZE, _TEXT("%d/%m/%Y"), &localTime);
		return m_localDate;
	}

	//============================================================================

	TCHAR* CSystemClock::Platform_GetLocalTimeString(void) const
	{
		::time_t absoluteTime;
		::time(&absoluteTime);

		::tm localTime;
		::localtime_s(&localTime, &absoluteTime);

		memset(m_localTime, 0, sizeof(TCHAR) * SYSTEM_CLOCK_TIME_BUFFER_SIZE);
		::_tcsftime(m_localTime, SYSTEM_CLOCK_TIME_BUFFER_SIZE, _TEXT("%H:%M:%S"), &localTime);
		return m_localTime;
	}

	//============================================================================
} // End [namespace engine]

//==============================================================================
// [EOF]
