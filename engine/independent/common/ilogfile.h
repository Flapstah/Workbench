#if !defined(__ILOGFILE_H__)
#define __ILOGFILE_H__

//==============================================================================

#define LOGS_ENABLED (1)									// Enable/disable logging
#define LOGS_HAVE_SCOPED_CHANNELS (1)			// Channels are scoped, i.e. child log can have active warning channel even if parent warning channel is disabled
#define LOGS_FORCE_INSERT_NEWLINE (1)			// Force a newline to be inserted at the end of every separate log output (if it doesn't already exist)

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

	ILogFile* GetMainLog(void);

#if defined LOGS_ENABLED
#define _WriteLog(_pILogFile_, _channels_, _output_) ((_pILogFile_->IsActive(_channels_)) && (_pILogFile_ _output_))
#else
#define _WriteLog(_pILogFile_, _channels_, _output_)
#endif

#define WriteLog(_pILogFile_, _channels_, _output_) _WriteLog(_pILogFile_, _channels_, _output_)
#define WriteLogAlways(_pILogFile_, _output_) (_pILogFile_->Write _output_)
#define WriteLogError(_pILogFile_, _output_) _WriteLog(_pILogFile_, ILogFile::eCF_Error, _output_)
#define WriteLogAssert(_pILogFile_, _output_) _WriteLog(_pILogFile_, ILogFile::eCF_Assert, _output_)
#define WriteLogWarning(_pILogFile_, _output_) _WriteLog(_pILogFile_, ILogFile::eCF_Warning, _output_)
#define WriteLogImportant(_pILogFile_, _output_) _WriteLog(_pILogFile_, ILogFile::eCF_Important, _output_)
#define WriteLogInformation(_pILogFile_, _output_) _WriteLog(_pILogFile_, ILogFile::eCF_Information, _output_)
#define WriteLogToDo(_pILogFile_, _output_) _WriteLog(_pILogFile_, ILogFile::eCF_ToDo, _output_)
#define WriteLogHack(_pILogFile_, _output_) _WriteLog(_pILogFile_, ILogFile::eCF_Hack, _output_)
#define WriteLogUser(_pILogFile_, _output_) _WriteLog(_pILogFile_, ILogFile::eCF_User, _output_)

#define Log(_channels_, _output_) _WriteLog(engine::GetMainLog(), _channels_, _output_)
#define LogAlways(_output_) (engine::GetMainLog()->Write _output_)
#define LogError(_output_) _WriteLog(engine::GetMainLog(), ILogFile::eCF_Error, _output_)
#define LogAssert(_output_) _WriteLog(engine::GetMainLog(), ILogFile::eCF_Assert, _output_)
#define LogWarning(_output_) _WriteLog(engine::GetMainLog(), ILogFile::eCF_Warning, _output_)
#define LogImportant(_output_) _WriteLog(engine::GetMainLog(), ILogFile::eCF_Important, _output_)
#define LogInformation(_output_) _WriteLog(engine::GetMainLog(), ILogFile::eCF_Information, _output_)
#define LogToDo(_output_) _WriteLog(engine::GetMainLog(), ILogFile::eCF_ToDo, _output_)
#define LogHack(_output_) _WriteLog(engine::GetMainLog(), ILogFile::eCF_Hack, _output_)
#define LogUser(_output_) _WriteLog(engine::GetMainLog(), ILogFile::eCF_User, _output_)
} // End [namespace engine]

//==============================================================================
#endif // End [!defined(__ILOGFILE_H__)]
// [EOF]
