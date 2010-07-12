#if !defined(__TIMER_H__)
#define __TIMER_H__

//==============================================================================

namespace engine
{
	class CTimer : public ITimer
	{
		typedef ITimer PARENT;

	public:
										CTimer(ITimeSource* pSource, float scale = 1.0f);
		virtual					~CTimer(void);

		// ITimeSource
		virtual	bool		Tick(void) = 0;
		// ~ITimeSource

		// ITimer
		virtual void		SetScale(float scale)			{ m_scale = scale;										}
		virtual float		GetScale(void) const			{ return m_scale;											}

		virtual void		Pause(bool pause)					{ m_paused = true; m_frameTime = 0.0;	}
		virtual bool		IsPaused(void) const			{ return m_paused;										}

		virtual void		Reset(void)								{ m_currentTime = m_frameTime = 0.0;	}
		// ~ITimer

	protected:
		ITimeSource*	m_pSource;
		float					m_scale;
		bool					m_paused;
	}; // End [class CTimer : public ITimer]
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__TIMER_H__)]
// [EOF]