#if !defined(__ITIME_H__)
#define __ITIME_H__

//==============================================================================

namespace engine
{
	//============================================================================
	// ITimeSource
	//============================================================================
	struct ITimeSource
	{
	public:
										ITimeSource(void)
											: m_currentTime(0.0), m_frameTime(0.0), m_frameCount(0), m_referenceCount(0)
																								{																						}
		virtual					~ITimeSource(void)					{	assert(m_referenceCount == 0);						}

		virtual	bool		Tick(void) = 0							{ ++m_frameCount; return true;							}

						float		GetTime(void) const					{ return static_cast<float>(m_currentTime);	}
						double	GetTimePrecise(void) const	{	return m_currentTime;											}

						float		GetFrameTime(void) const		{ return static_cast<float>(m_frameTime);		}
						double	GetFrameTimePrecise(void) const	{	return m_frameTime;										}
						uint32	GetFrameCount(void) const		{ return m_frameCount;											}

						void		AddReference(void)					{ ++m_referenceCount;												}
						uint32	Release(void)								{ return --m_referenceCount;								}

	protected:
		double m_currentTime;
		double m_frameTime;
		uint32 m_frameCount;
		uint32 m_referenceCount;
	}; // End [struct ITimeSource]

	//============================================================================
	// ISystemClock
	//============================================================================
	struct ISystemClock : public ITimeSource 
	{
	public:
		// ITimeSource
		virtual	bool		Tick(void) = 0;
		// ~ITimeSource

		virtual char*		GetLocalDateString(void) const = 0;
		virtual char*		GetLocalTimeString(void) const = 0;
	}; // End [struct ISystemClock : public ITimeSource ]

	//----------------------------------------------------------------------------
	// Returns the system clock
	//----------------------------------------------------------------------------
	const ISystemClock* GetSystemClock(void);

	//============================================================================
	// CTimer
	//============================================================================
	class CTimer : public ITimeSource
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
		// ~ITimeSource

						void		SetScale(double scale)			{	m_scale = scale;													}
						double	GetScale(void) const				{	return m_scale;														}

						void		Pause(bool pause)						{ m_paused = true; m_frameTime = 0.0;				}
						bool		IsPaused(void)							{	return m_paused;													}

						void		Reset(void)									{ m_currentTime = m_frameTime = 0.0;				}

	protected:
		ITimeSource&	m_timeSource;
		double				m_scale;
		double				m_maxFrameTime;
		bool					m_paused;
	}; // End [struct CTimer : public ITimeSource]

	//============================================================================
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__ITIME_H__)]
// [EOF]
