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
		virtual	bool		Tick(void) = 0;

		virtual	float		GetTime(void) const = 0;
		virtual	double	GetTimePrecise(void) const = 0;

		virtual	float		GetFrameTime(void) const = 0;
		virtual	double	GetFrameTimePrecise(void) const = 0;

		virtual	uint32	GetFrameCount(void) const = 0;

		virtual	void		AddReference(void) = 0;
		virtual	uint32	Release(void) = 0;
	}; // End [struct ITimeSource]

	//============================================================================
	// ISystemClock
	//============================================================================
	struct ISystemClock : virtual public ITimeSource 
	{
		virtual TCHAR*	GetLocalDateString(void) const = 0;
		virtual TCHAR*	GetLocalTimeString(void) const = 0;
	}; // End [struct ISystemClock : public ITimeSource ]

	//============================================================================
	// ITimer
	//============================================================================
	struct ITimer : virtual public ITimeSource
	{
		virtual void		SetScale(double scale) = 0;
		virtual double	GetScale(void) const = 0;

		virtual void		Pause(bool pause) = 0;
		virtual bool		IsPaused(void) = 0;

		virtual void		Reset(void) = 0;
	}; // End [struct ITimer : public ITimeSource]

	//----------------------------------------------------------------------------
	// Returns the system clock
	//----------------------------------------------------------------------------
	const ISystemClock* GetSystemClock(void);

	//----------------------------------------------------------------------------
	// Returns the game clock
	//----------------------------------------------------------------------------
	const ITimer* GetGameClock(void);

	//============================================================================
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__ITIME_H__)]
// [EOF]
