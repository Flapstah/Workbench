#include "timer.h"

//==============================================================================

namespace engine
{
	//============================================================================

	CTimer::CTimer(ITimeSource* pSource, float scale /* = 1.0f */)
		: m_pSource(pSource)
		, m_scale(scale)
		, m_paused(false)
	{
		m_frameCount = 0;
		Tick();
	}

	//============================================================================

	CTimer::~Timer(void)
	{
	}

	//============================================================================

	bool CTimer::Tick(void)
	{
		bool ticked = false;

		if ((!m_paused) && (m_frameCount < m_pSource->m_frameCount))
		{
			double frameTime = m_pSource->m_frameTime;

#if TIMER_FRAME_CLAMPING
			if (frameTime > TIMER_MAXIMUM_FRAME_TIME)
			{
				frameTime = TIMER_MAXIMUM_FRAME_TIME;
			}
#endif

			m_frameTime = frameTime * m_scale;
			m_currentTime += m_frameTime;
			m_frameCount = m_pSource->m_frameCount;
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
