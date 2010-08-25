#if !defined(__ILOGFILE_H__)
#define __ILOGFILE_H__

//==============================================================================

#define LOGS_ENABLED								// Enable/disable logging
//#define LOGS_FORCE_SEPARATE_FILES		// Force logs into separate files
#define LOGS_FORCE_INSERT_NEWLINE		// Force a newline to be inserted at the end of every separate log output (if it doesn't already exist)
#define LOGS_FORCE_FLUSH_THRESHOLD ((uint32)(0x000f * 0.8f)) // Force a flush when the internal log buffer is 80% or more full (approximate - measured in 15ths)

//==============================================================================

namespace engine
{
	//============================================================================
	// ILogFile
	//============================================================================
	struct ILogFile
	{
		enum eBehaviourFlag
		{
			eBF_Active						= 1 << 0,
			eBF_Name							= 1 << 1,
			eBF_DateStamp					= 1 << 2,
			eBF_TimeStamp					= 1 << 3,
			eBF_OutputToDebugger	= 1 << 4,
			eBF_FlushEachWrite		= 1 << 5,

			eBF_ALL								= eBF_Active | eBF_Name | eBF_DateStamp | eBF_TimeStamp | eBF_OutputToDebugger | eBF_FlushEachWrite
		}; // End [enum eBehaviourFlag]

		virtual void SetActive(bool active) = 0;
		virtual bool IsActive(void) const = 0;

		virtual void TurnOnBehaviours(eBehaviourFlag behaviours) = 0;
		virtual void TurnOffBehaviours(eBehaviourFlag behaviours) = 0;
		virtual bool IsBehaviourOn(eBehaviourFlag behaviour) const = 0;

		virtual bool Write(const TCHAR* format, ...) = 0;
	}; // End [struct ILogFile]

	//============================================================================

	//----------------------------------------------------------------------------
	// Returns the main log file
	//----------------------------------------------------------------------------
	ILogFile* GetMainLog(void);

	//----------------------------------------------------------------------------
	// Returns the error log file
	//----------------------------------------------------------------------------
	ILogFile* GetErrorLog(void);

	//----------------------------------------------------------------------------
	// Returns the warning log file
	//----------------------------------------------------------------------------
	ILogFile* GetWarningLog(void);

	//----------------------------------------------------------------------------
	// Helper macros for log output
	//----------------------------------------------------------------------------
#if defined LOGS_ENABLED
#define _WriteLog(_pILogFile_, _output_) ((_pILogFile_->IsActive()) && (_pILogFile_->Write _output_))
#else
#define _WriteLog(_pILogFile_, _channels_, _output_)
#endif

	//----------------------------------------------------------------------------
	// Use LogXXX to output to the logs
	//----------------------------------------------------------------------------
#define Log(_output_) _WriteLog(engine::GetMainLog(), _output_)
#define LogError(_output_) _WriteLog(engine::GetErrorLog(), _output_)
#define LogWarning(_output_) _WriteLog(engine::GetWarningLog(), _output_)

} // End [namespace engine]

//==============================================================================
#endif // End [!defined(__ILOGFILE_H__)]
// [EOF]
