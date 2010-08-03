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

	char* CSystemClock::Platform_GetLocalDateString(void) const
	{
		::time_t absoluteTime;
		::time(&absoluteTime);

		::tm localTime;
		::localtime_s(&localTime, &absoluteTime);

		memset(m_textBuffer, 0, SYSTEM_CLOCK_DATE_BUFFER_SIZE);
		::strftime(m_textBuffer, SYSTEM_CLOCK_DATE_BUFFER_SIZE, "%d/%m/%Y", &localTime);
		return m_textBuffer;
	}

	//============================================================================

	char* CSystemClock::Platform_GetLocalTimeString(void) const
	{
		::time_t absoluteTime;
		::time(&absoluteTime);

		::tm localTime;
		::localtime_s(&localTime, &absoluteTime);

		memset(&m_textBuffer[SYSTEM_CLOCK_DATE_BUFFER_SIZE], 0, SYSTEM_CLOCK_TIME_BUFFER_SIZE);
		::strftime(&m_textBuffer[SYSTEM_CLOCK_DATE_BUFFER_SIZE], SYSTEM_CLOCK_TIME_BUFFER_SIZE, "%H:%M:%S", &localTime);
		return &m_textBuffer[SYSTEM_CLOCK_DATE_BUFFER_SIZE];
	}

	//============================================================================
} // End [namespace engine]

//==============================================================================
// [EOF]
