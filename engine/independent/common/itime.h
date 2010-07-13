#if !defined(__ITIME_H__)
#define __ITIME_H__

//==============================================================================

#define TIMER_FRAME_CLAMPING (1)
#define TIMER_MAXIMUM_FRAME_TIME (100)

//==============================================================================

namespace engine
{
	//============================================================================
	// ITimeSource
	//============================================================================
	struct ITimeSource
	{
										ITimeSource(void) : m_currentTime(0.0), m_frameTime(0.0), m_frameCount(0) {}
		virtual					~ITimeSource(void) {}

		virtual	bool		Tick(void) = 0;

						float		GetCurrentTime(void) const				{	return static_cast<float>(m_currentTime);	}
						double	GetCurrentTimePrecise(void) const	{	return m_currentTime;											}

						float		GetFrameTime(void) const					{	return static_cast<float>(m_frameTime);		}
						double	GetFrameTimePrecise(void)					{	return m_frameTime;												}

						uint32	GetFrameCount(void) const					{ return m_frameCount;											}

	protected:
		double	m_currentTime;
		double	m_frameTime;
		uint32	m_frameCount;
	}; // End [struct ITimeSource]

	//============================================================================
	// ISystemClock
	//============================================================================
	struct ISystemClock : public ITimeSource
	{
		virtual char*		GetLocalDateString(void) = 0;
		virtual char*		GetLocalTimeString(void) = 0;
	}; // End [struct ISystemClock : public ITimeSource]

	//----------------------------------------------------------------------------
	// This sytem clock is available everywhere
	//----------------------------------------------------------------------------
	//extern ISystemClock g_systemClock;

	//============================================================================
	// ITimer
	//============================================================================
	struct ITimer : public ITimeSource
	{
		virtual void		SetScale(float scale) = 0;
		virtual float		GetScale(void) const = 0;

		virtual void		Pause(bool pause) = 0;
		virtual bool		IsPaused(void) const = 0;

		virtual void		Reset(void) = 0;
	}; // End [struct ITimer : public ITimeSource]

	//----------------------------------------------------------------------------
	// This game clock is available everywhere
	//----------------------------------------------------------------------------
	//extern ITimer g_gameClock;

} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__ITIME_H__)]
// [EOF]
