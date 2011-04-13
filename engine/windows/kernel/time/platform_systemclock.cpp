#include "stdafx.h"

#include "kernel/time/realtimeclock.h"

//==============================================================================

namespace engine
{
	//============================================================================

	void CRealTimeClock::Platform_Initialise(void)
	{
		LARGE_INTEGER frequency;

		::QueryPerformanceFrequency(&frequency);
		m_secondsPerTick = 1.0 / frequency.QuadPart;

		Platform_Tick();
		m_frameTime = 0.0;
	}

	//============================================================================

	void CRealTimeClock::Platform_Uninitialise(void)
	{
		// Nothing to do
	}

	//============================================================================

	bool CRealTimeClock::Platform_Tick(void)
	{
		double currentTime = Platform_GetTimePrecise();
		m_frameTime = currentTime - m_currentTime;
		m_currentTime = currentTime;

		return true;
	}

	//============================================================================

	double CRealTimeClock::Platform_GetTimePrecise(void) const
	{
		LARGE_INTEGER time;

		::QueryPerformanceCounter(&time);
		double currentTime = time.QuadPart * m_secondsPerTick;

		return currentTime;
	}

	//============================================================================

	TCHAR* CRealTimeClock::Platform_GetLocalDateString(void) const
	{
		::time_t absoluteTime;
		::time(&absoluteTime);

		::tm localTime;
		::localtime_s(&localTime, &absoluteTime);

		memset(m_localDate, 0, sizeof(TCHAR) * REAL_TIME_CLOCK_DATE_BUFFER_SIZE);
		::_tcsftime(m_localDate, REAL_TIME_CLOCK_DATE_BUFFER_SIZE, _TEXT("%d/%m/%Y"), &localTime);
		return m_localDate;
	}

	//============================================================================

	TCHAR* CRealTimeClock::Platform_GetLocalTimeString(void) const
	{
		::time_t absoluteTime;
		::time(&absoluteTime);

		::tm localTime;
		::localtime_s(&localTime, &absoluteTime);

		memset(m_localTime, 0, sizeof(TCHAR) * REAL_TIME_CLOCK_TIME_BUFFER_SIZE);
		::_tcsftime(m_localTime, REAL_TIME_CLOCK_TIME_BUFFER_SIZE, _TEXT("%H:%M:%S"), &localTime);
		return m_localTime;
	}

	//============================================================================
} // End [namespace engine]

//==============================================================================
// [EOF]
