#if !defined(__SYSTEMCLOCK_H__)
#define __SYSTEMCLOCK_H__

//==============================================================================

#include "itime.h"

//==============================================================================

#define SYSTEM_CLOCK_DATE_BUFFER_SIZE (12)
#define SYSTEM_CLOCK_TIME_BUFFER_SIZE (9)
#define SYSTEM_CLOCK_ALIGNMENT (4)
#define SYSTEM_CLOCK_ALIGNED_BUFFER_SIZE ((SYSTEM_CLOCK_DATE_BUFFER_SIZE + SYSTEM_CLOCK_TIME_BUFFER_SIZE + SYSTEM_CLOCK_ALIGNMENT - 1) / SYSTEM_CLOCK_ALIGNMENT)

namespace engine
{
	//============================================================================
	// CSystemClock
	//============================================================================
	class CSystemClock : public ISystemClock
	{
		typedef ISystemClock PARENT;

	public:
										CSystemClock(void)				{	Platform_Initialise();									}
		virtual					~CSystemClock(void)				{	Platform_Uninitialise();								}

		// ITimeSource
		virtual	bool		Tick(void)								{	PARENT::Tick(); return Platform_Tick();	}
		// ~ITimeSource

		// ISystemClock
		virtual char*		GetLocalDateString(void)	{	return Platform_GetLocalDateString();		}
		virtual char*		GetLocalTimeString(void)	{	return Platform_GetLocalTimeString();		}
		// ~ISystemClock

	protected:
						void		Platform_Initialise(void);
						void		Platform_Uninitialise(void);
						bool		Platform_Tick(void);
						char*		Platform_GetLocalDateString(void);
						char*		Platform_GetLocalTimeString(void);

	protected:
		double	m_secondsPerTick;
		char		m_textBuffer[SYSTEM_CLOCK_ALIGNED_BUFFER_SIZE];
	}; // End [class CSystemClock : public ISystemClock]

	//----------------------------------------------------------------------------
	// This sytem clock is available everywhere
	//----------------------------------------------------------------------------
	extern CSystemClock g_systemClock;

} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__SYSTEMCLOCK_H__)]
// [EOF]