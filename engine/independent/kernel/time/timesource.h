#if !defined(__TIMESOURCE_H__)
#define __TIMESOURCE_H__

//==============================================================================

#include "common/itime.h"

//==============================================================================

namespace engine
{
	//============================================================================
	// CTimeSource
	//============================================================================
	class CTimeSource : public ITimeSource
	{
		typedef ITimeSource PARENT;

	public:
										CTimeSource(void)		{}
		virtual					~CTimeSource(void)	{}

		// ITimeSource
		virtual	bool		Tick(void);
		// ~ITimeSource
	}; // End [class CTimeSource : public ITimeSource]
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__TIMESOURCE_H__)]
// [EOF]
