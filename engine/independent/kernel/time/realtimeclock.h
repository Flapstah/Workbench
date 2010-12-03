#if !defined(__REALTIMECLOCK_H__)
#define __REALTIMECLOCK_H__

//==============================================================================

#include "kernel/time/timesource.h"

//==============================================================================

#pragma warning(push)
#pragma warning(disable:4250) // 'class1' : inherits 'class2::member' via dominance

//==============================================================================

#define REAL_TIME_CLOCK_DATE_BUFFER_SIZE (12)
#define REAL_TIME_CLOCK_TIME_BUFFER_SIZE (12)

//==============================================================================

namespace engine
{
	//============================================================================
	// CRealTimeClock
	//============================================================================
	class CRealTimeClock : public virtual IRealTimeClock, public CTimeSource
	{
		typedef CTimeSource PARENT;

	public:
										CRealTimeClock(void)						{	Platform_Initialise();									}
		virtual					~CRealTimeClock(void)						{	Platform_Uninitialise();								}

		// ITimeSource
		virtual	bool		Tick(void)											{	PARENT::Tick(); return Platform_Tick();	}
		// ~ITimeSource

		// IRealTimeClock
		virtual TCHAR*	GetLocalDateString(void) const	{	return Platform_GetLocalDateString();		}
		virtual TCHAR*	GetLocalTimeString(void) const	{	return Platform_GetLocalTimeString();		}
		// ~IRealTimeClock

	protected:
						void		Platform_Initialise(void);
						void		Platform_Uninitialise(void);
						bool		Platform_Tick(void);
						TCHAR*	Platform_GetLocalDateString(void) const;
						TCHAR*	Platform_GetLocalTimeString(void) const;

	protected:
						double	m_secondsPerTick;
		mutable TCHAR		m_localDate[REAL_TIME_CLOCK_DATE_BUFFER_SIZE];
		mutable TCHAR		m_localTime[REAL_TIME_CLOCK_TIME_BUFFER_SIZE];
	}; // End [class CRealTimeClock : public IRealTimeClock]

	//============================================================================

	//----------------------------------------------------------------------------
	// The global instance of the real time clock
	//----------------------------------------------------------------------------
	extern CRealTimeClock g_realTimeClock;

	//============================================================================
} // End [namespace engine]

//==============================================================================

#pragma warning(pop)

//==============================================================================

#endif // End [!defined(__REALTIMECLOCK_H__)]
// [EOF]