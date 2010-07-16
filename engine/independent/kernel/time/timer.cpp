#include "stdafx.h"

#include "timer.h"

//==============================================================================

namespace engine
{
	//============================================================================

	CTimer::CTimer(ITimeSource* pSource, float scale, float maxFrameTime)
		: m_pSource(pSource)
		, m_scale(scale)
		, m_maxFrameTime(maxFrameTime)
		, m_paused(false)
	{
		if (pSource != NULL)
		{
			Tick();
		}
	}

	//============================================================================

	CTimer::~CTimer(void)
	{
	}

	//============================================================================

	bool CTimer::Tick(void)
	{
		bool ticked = false;

		if ((!m_paused) && (m_frameCount < m_pSource->GetFrameCount()))
		{
			double frameTime = m_pSource->GetFrameTimePrecise();

#if TIMER_FRAME_CLAMPING
			if (frameTime > TIMER_MAXIMUM_FRAME_TIME)
			{
				frameTime = TIMER_MAXIMUM_FRAME_TIME;
			}
#endif

			m_frameTime = frameTime * m_scale;
			m_currentTime = m_pSource->GetCurrentTimePrecise();
			m_frameCount = m_pSource->GetFrameCount();
			ticked = true;
		}

		return ticked;
	}

	//============================================================================

	//----------------------------------------------------------------------------
	// The global instance of the game clock
	//
	// Use the game clock as the root of all timers (rather than the system clock)
	// as it's current time count will be elapsed *game* time, not *real* time.
	//----------------------------------------------------------------------------
	CTimer g_gameClock(g_systemClock);

} // End [namespace engine]

//==============================================================================

// [EOF]
