#if !defined(__SYSTEMCLOCK_H__)
#define __SYSTEMCLOCK_H__

//==============================================================================

#include "kernel/time/timesource.h"

//==============================================================================

#pragma warning(push)
#pragma warning(disable:4250) // 'class1' : inherits 'class2::member' via dominance

//==============================================================================

#define SYSTEM_CLOCK_DATE_BUFFER_SIZE (12)
#define SYSTEM_CLOCK_TIME_BUFFER_SIZE (12)

//==============================================================================

namespace engine
{
	//============================================================================
	// CSystemClock
	//============================================================================
	class CSystemClock : public virtual ISystemClock, public CTimeSource
	{
		typedef CTimeSource PARENT;

	public:
										CSystemClock(void)							{	Platform_Initialise();									}
		virtual					~CSystemClock(void)							{	Platform_Uninitialise();								}

		// ITimeSource
		virtual	bool		Tick(void)											{	PARENT::Tick(); return Platform_Tick();	}
		// ~ITimeSource

		// ISystemClock
		virtual TCHAR*	GetLocalDateString(void) const	{	return Platform_GetLocalDateString();		}
		virtual TCHAR*	GetLocalTimeString(void) const	{	return Platform_GetLocalTimeString();		}
		// ~ISystemClock

	protected:
						void		Platform_Initialise(void);
						void		Platform_Uninitialise(void);
						bool		Platform_Tick(void);
						TCHAR*	Platform_GetLocalDateString(void) const;
						TCHAR*	Platform_GetLocalTimeString(void) const;

	protected:
						double	m_secondsPerTick;
		mutable TCHAR		m_localDate[SYSTEM_CLOCK_DATE_BUFFER_SIZE];
		mutable TCHAR		m_localTime[SYSTEM_CLOCK_TIME_BUFFER_SIZE];
	}; // End [class CSystemClock : public ISystemClock]

	//============================================================================

	//----------------------------------------------------------------------------
	// The global instance of the system clock
	//----------------------------------------------------------------------------
	extern CSystemClock g_systemClock;

	//============================================================================
} // End [namespace engine]

//==============================================================================

#pragma warning(pop)

//==============================================================================

#endif // End [!defined(__SYSTEMCLOCK_H__)]
// [EOF]