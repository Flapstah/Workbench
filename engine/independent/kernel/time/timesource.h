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
	public:
		CTimeSource(void);
		virtual ~CTimeSource(void);

		virtual	bool		Tick(void);

						float		GetTime(void) const;
						double	GetTimePrecise(void) const;
						char*		GetTimeString(void) const;

						void		AddReference(void);
						void		Release(void);

	protected:
		double m_currentTime;
		uint32 m_referenceCount;

	private:
	}; // End [class CTimeSource : public ITimeSource]

	//============================================================================

} // End [namespace engine]
#endif // End [!defined(__TIMESOURCE_H__)]
// [EOF]
