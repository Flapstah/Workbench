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

		virtual	float		GetCurrentTime(void) const = 0;
		virtual	double	GetCurrentTimePrecise(void) const = 0;

		virtual	float		GetFrameTime(void) const = 0;
		virtual	double	GetFrameTimePrecise(void) = 0;

		virtual uint32	GetFrameCount(void) const = 0;
	}; // End [struct ITimeSource]

	//============================================================================
	// ISystemClock
	//============================================================================
	struct ISystemClock : public ITimeSource
	{
		virtual char*		GetLocalDateString(void) = 0;
		virtual char*		GetLocalTimeString(void) = 0;
	}; // End [struct ISystemClock : public ITimeSource]

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

} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__ITIME_H__)]
// [EOF]
