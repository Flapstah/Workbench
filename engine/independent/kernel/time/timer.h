#if !defined(__TIMER_H__)
#define __TIMER_H__

//==============================================================================

#include "kernel/time/timesource.h"

//==============================================================================

namespace engine
{
	//============================================================================
	// CTimer
	//============================================================================
	class CTimer : public ITimer, public CTimeSource
	{
	protected:
		typedef ITimeSource PARENT;

	public:
										CTimer(ITimeSource& source, double scale, double maxFrameTime)
											: m_timeSource(source), m_scale(0.0), m_maxFrameTime(0.0)
																								{	m_timeSource.AddReference();							}
										~CTimer(void)								{ m_timeSource.Release();										}

		// ITimeSource
		virtual	bool		Tick(void);

		virtual	float		GetTime(void) const;
		virtual	double	GetTimePrecise(void) const;

		virtual	float		GetFrameTime(void) const;
		virtual	double	GetFrameTimePrecise(void);

		virtual	uint32	GetFrameCount(void) const;

		virtual	void		AddReference(void);
		virtual	uint32	Release(void);
		// ~ITimeSource

		// ITimer
		virtual	void		SetScale(double scale)			{	m_scale = scale;													}
		virtual	double	GetScale(void) const				{	return m_scale;														}

		virtual	void		Pause(bool pause)						{ m_paused = true; m_frameTime = 0.0;				}
		virtual	bool		IsPaused(void)							{	return m_paused;													}

		virtual	void		Reset(void)									{ m_currentTime = m_frameTime = 0.0;				}
		// ~ITimer

	protected:
		ITimeSource&	m_timeSource;
		double				m_scale;
		double				m_maxFrameTime;
		bool					m_paused;
	}; // End [struct CTimer : public ITimeSource]

}

//==============================================================================
#endif // End [!defined(__TIMER_H__)]
// [EOF]
