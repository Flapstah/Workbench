#if !defined(__ILOGFILE_H__)
#define __ILOGFILE_H__

//==============================================================================

#include "common/ifile.h"

//==============================================================================

namespace engine
{
	//============================================================================
	// CLogFile
	//============================================================================
	class CLogFile
	{
	public:
		CLogFile(TCHAR* name, CLogFile* pParent);
		~CLogFile(void);

		void Write(const TCHAR* format, ...);

		enum eBehaviourFlag
		{
			eBF_Active						= 1 << 0,
			eBF_TimeStamp					= 1 << 1,
			eBF_OutputToDebugger	= 1 << 2,

			eBF_ALL								= eBF_Active | eBF_TimeStamp | eBF_OutputToDebugger
		};

		void SetActive(bool active)													{ active ? TurnOnBehaviours(eBF_Active) : TurnOffBehaviours(eBF_Active);	}
		bool GetActive(void) const													{ return IsBehaviourOn(eBF_Active);								}

		void TurnOnBehaviours(eBehaviourFlag behaviours)		{ m_behaviours |= behaviours;											}
		void TurnOffBehaviours(eBehaviourFlag behaviours)		{ m_behaviours &= ~behaviours;										}
		bool IsBehaviourOn(eBehaviourFlag behaviour) const	{ return (m_behaviours & behaviour) == behaviour;	}

		enum eChannelFlag
		{
			eCF_Error				= 1 << 0,
			eCF_Assert			= 1 << 1,
			eCF_Important		= 1 << 2,
			eCF_Warning			= 1 << 3,
			eCF_Information	= 1 << 4,
			eCF_Todo				= 1 << 5,
			eCF_Hack				= 1 << 6,
			eCF_User				= 1 << 7,

			eCF_ALL					= eCF_Error | eCF_Assert | eCF_Important | eCF_Warning | eCF_Information | eCF_Todo | eCF_Hack | eCF_User
		};

		void TurnOnChannels(eChannelFlag channels)					{ m_channels |= channels;													}
		void TurnOffChannels(eChannelFlag channels)					{ m_channels &= ~channels;												}
		bool IsChannelOn(eChannelFlag channel) const				{ return (m_channels & channel) == channel;				}

	protected:
		CFile m_file;
		CLogFile* m_pParent;
		TCHAR m_name[32];
		uint32 m_channels;
		uint32 m_behaviours;

	private:
	}; // End [class CLogFile]
} // End [namespace engine]

//==============================================================================
#endif // End [!defined(__ILOGFILE_H__)]
// [EOF]
