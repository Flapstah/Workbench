
#include "timesource.h"

//==============================================================================

namespace engine
{
	//============================================================================

	TimeSource::TimeSource(void)
		: m_currentTime(0.0)
		, m_frameTime(0.0)
		, m_frameCount(0)
	{
	}

	//============================================================================

	TimeSource::~TimeSource(void)
	{
	}

	//============================================================================

	bool TimeSource::Tick(void)
	{
		++m_frameCount;
		return true;
	}

	//============================================================================

	float TimeSource::GetCurrentTime(void)
	{
		return static_cast<float>(m_currentTime);
	}

	//============================================================================

	double TimeSource::GetCurrentTimePrecise(void)
	{
		return m_currentTime;
	}

	//============================================================================

	float TimeSource::GetFrameTime(void)
	{
		return static_cast<float>(m_frameTime);
	}

	//============================================================================

	double TimeSource::GetFrameTimePrecise(void)
	{
		return m_frameTime;
	}

	//============================================================================

	uint32 TimeSource::GetFrameCount(void)
	{
		return m_frameCount;
	}

	//============================================================================
} // End [namespace engine]

//==============================================================================
// [EOF]
