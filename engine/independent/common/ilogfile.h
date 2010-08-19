#if !defined(__ILOGFILE_H__)
#define __ILOGFILE_H__

//==============================================================================


//==============================================================================

namespace engine
{
	struct ILogFile 
	{
		enum eBehaviourFlag
		{
			eBF_Active						= 1 << 0,
			eBF_Name							= 1 << 1,
			eBF_TimeStamp					= 1 << 2,
			eBF_OutputToDebugger	= 1 << 3,

			eBF_ALL								= eBF_Active | eBF_Name | eBF_TimeStamp | eBF_OutputToDebugger
		};

		virtual void SetActive(bool active) = 0;
		virtual bool IsActive(void) const = 0;

		virtual void TurnOnBehaviours(eBehaviourFlag behaviours) = 0;
		virtual void TurnOffBehaviours(eBehaviourFlag behaviours) = 0;
		virtual bool IsBehaviourOn(eBehaviourFlag behaviour) const = 0;

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

		virtual void TurnOnChannels(eChannelFlag channels) = 0;
		virtual void TurnOffChannels(eChannelFlag channels) = 0;
		virtual bool IsChannelOn(eChannelFlag channel) const = 0;

		virtual void Write(eChannelFlag channel, const TCHAR* format, ...) = 0;
	}; // End [struct ILogFile ]

	ILogFile* CreateLog(const TCHAR* name, ILogFile* pParent);

} // End [namespace engine]

//==============================================================================
#endif // End [!defined(__ILOGFILE_H__)]
// [EOF]
