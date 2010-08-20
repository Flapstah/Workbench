#if !defined(__ILOGFILE_H__)
#define __ILOGFILE_H__

//==============================================================================

#define LOGS_ENABLED (1)									// Enable/disable logging
#define LOGS_HAVE_SCOPED_CHANNELS (1)			// Channels are scoped, i.e. child log can have active warning channel even if parent warning channel is disabled

//==============================================================================

namespace engine
{
	struct ILogFile 
	{
		enum eBehaviourFlag
		{
			eBF_Active						= 1 << 0,
			eBF_Name							= 1 << 1,
			eBF_DateStamp					= 1 << 2,
			eBF_TimeStamp					= 1 << 3,
			eBF_OutputToDebugger	= 1 << 4,

			eBF_ALL								= eBF_Active | eBF_Name | eBF_DateStamp | eBF_TimeStamp | eBF_OutputToDebugger
		};

		enum eChannelFlag
		{
			eCF_Error				= 1 << 0,
			eCF_Assert			= 1 << 1,
			eCF_Warning			= 1 << 2,
			eCF_Important		= 1 << 3,
			eCF_Information	= 1 << 4,
			eCF_ToDo				= 1 << 5,
			eCF_Hack				= 1 << 6,
			eCF_User				= 1 << 7,

			eCF_ALL					= eCF_Error | eCF_Assert | eCF_Warning | eCF_Important | eCF_Information | eCF_ToDo | eCF_Hack | eCF_User
		};

		virtual void SetActive(bool active) = 0;
		virtual bool IsActive(eChannelFlag channels) const = 0;

		virtual void TurnOnBehaviours(eBehaviourFlag behaviours) = 0;
		virtual void TurnOffBehaviours(eBehaviourFlag behaviours) = 0;
		virtual bool IsBehaviourOn(eBehaviourFlag behaviour) const = 0;

		virtual void TurnOnChannels(eChannelFlag channels) = 0;
		virtual void TurnOffChannels(eChannelFlag channels) = 0;
		virtual bool IsChannelOn(eChannelFlag channel) const = 0;

		virtual bool Write(const TCHAR* format, ...) = 0;
	}; // End [struct ILogFile ]

	ILogFile* CreateLog(const TCHAR* name, ILogFile* pParent);

#if defined LOGS_ENABLED
#define _WriteLog(_pILogFile_, _channels_, _output_) ((_pILogFile_->IsActive(_channels_)) && (_pILogFile_ _output_))
#else
#define _WriteLog(_pILogFile_, _channels_, _output_)
#endif

#define Log(_pILogFile_, _channels_, _output_) _WriteLog(_pILogFile_, _channels_, _output_)
#define LogAlways(_pILogFile_, _output_) (_pILogFile_ _output_)
#define LogError(_pILogFile_, _output_) _WriteLog(_pILogFile_, ILogFile::eCF_Error, _output_)
#define LogAssert(_pILogFile_, _output_) _WriteLog(_pILogFile_, ILogFile::eCF_Assert, _output_)
#define LogWarning(_pILogFile_, _output_) _WriteLog(_pILogFile_, ILogFile::eCF_Warning, _output_)
#define LogImportant(_pILogFile_, _output_) _WriteLog(_pILogFile_, ILogFile::eCF_Important, _output_)
#define LogInformation(_pILogFile_, _output_) _WriteLog(_pILogFile_, ILogFile::eCF_Information, _output_)
#define LogToDo(_pILogFile_, _output_) _WriteLog(_pILogFile_, ILogFile::eCF_ToDo, _output_)
#define LogHack(_pILogFile_, _output_) _WriteLog(_pILogFile_, ILogFile::eCF_Hack, _output_)
#define LogUser(_pILogFile_, _output_) _WriteLog(_pILogFile_, ILogFile::eCF_User, _output_)
} // End [namespace engine]

//==============================================================================
#endif // End [!defined(__ILOGFILE_H__)]
// [EOF]
