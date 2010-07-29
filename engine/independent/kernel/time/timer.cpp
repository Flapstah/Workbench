#include "stdafx.h"

#include "common/itime.h"
#include "kernel/time/systemclock.h"

//==============================================================================

namespace engine
{
	//============================================================================

	bool CTimer::Tick(void)
	{
		bool ticked = false;

		if ((!m_paused) && (m_frameCount < m_timeSource.GetFrameCount()))
		{
			double frameTime = m_timeSource.GetFrameTimePrecise();

			if ((m_maxFrameTime > 0.0) && (frameTime > m_maxFrameTime))
			{
				frameTime = m_maxFrameTime;
			}

			m_frameTime = frameTime * m_scale;
			m_currentTime += m_frameTime;
			m_frameCount = m_timeSource.GetFrameCount();
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
	CTimer g_gameClock(g_systemClock, 1.0, 0.1);

	//============================================================================

	const CTimer* GetGameClock(void)
	{
		return &g_gameClock;
	}

} // End [namespace engine]

//==============================================================================

// [EOF]
