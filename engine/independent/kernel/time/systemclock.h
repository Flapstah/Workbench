#if !defined(__SYSTEMCLOCK_H__)
#define __SYSTEMCLOCK_H__

//==============================================================================

#include "itime.h"

//==============================================================================

namespace engine
{
	//============================================================================
	// CSystemClock
	//============================================================================
	class CSystemClock : public ISystemClock
	{
		typedef ISystemClock PARENT;

	public:
										CSystemClock(void)	{};
		virtual					~CSystemClock(void)	{};

		// ITimeSource
		virtual	bool		Tick(void);
		// ~ITimeSource

		// ISystemClock
		virtual char*		GetLocalDateString(void);
		virtual char*		GetLocalTimeString(void);
		// ~ISystemClock
	}; // End [class CSystemClock : public ISystemClock]
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__SYSTEMCLOCK_H__)]
// [EOF]