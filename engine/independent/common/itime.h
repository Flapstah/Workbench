#if !defined(__ITIME_H__)
#define __ITIME_H__

//==============================================================================

namespace engine
{
	//============================================================================
	// CTimeSource
	//============================================================================
	class CTimeSource
	{
	public:
										CTimeSource(void)						: m_currentTime(0.0), m_referenceCount(0)	 {}
		virtual					~CTimeSource(void)					{	assert(m_referenceCount == 0);						}

		virtual	bool		Tick(void) = 0;

						float		GetTime(void) const					{ return static_cast<float>(m_currentTime);	}
						double	GetTimePrecise(void) const	{ return m_currentTime;											}
		virtual	char*		GetTimeString(void) const;

						void		AddReference(void)					{ ++m_referenceCount;												}
						uint32	Release(void)								{ return --m_referenceCount;								}

	protected:
		double m_currentTime;
		uint32 m_referenceCount;

	private:
	}; // End [class CTimeSource]

	//============================================================================
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__ITIME_H__)]
// [EOF]
