#if !defined(__TIMESOURCE_H__)
#define __TIMESOURCE_H__

//==============================================================================

#include "common/itime.h"
//==============================================================================

namespace engine
{
	//============================================================================
	// Timesource
	//============================================================================
	class TimeSource : public ITimeSource
	{
	public:
		TimeSource(void);
		virtual ~TimeSource(void);

		// ITimeSource
		virtual	bool		Tick(void);

		virtual	float		GetCurrentTime(void);
		virtual	double	GetCurrentTimePrecise(void);

		virtual	float		GetFrameTime(void);
		virtual	double	GetFrameTimePrecise(void);

		virtual uint32	GetFrameCount(void);
		// ~ITimeSource

	protected:
		double	m_currentTime;
		double	m_frameTime;
		uint32	m_frameCount;

	private:
	};

}

//==============================================================================

#endif // End [!defined(__TIMESOURCE_H__)]
// [EOF]
