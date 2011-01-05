#if !defined(__ILOGFILE_H__)
#define __ILOGFILE_H__

//==============================================================================

#include "common/macros.h"

//==============================================================================

#define LOGS_ENABLED								// Enable/disable logging
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
			eBF_Active													= BIT(0),
			eBF_Name														= BIT(1),
			eBF_DateStamp												= BIT(2),
			eBF_LineCount												= BIT(3),
			eBF_FrameCount											= BIT(4),
			eBF_TimeStamp												= BIT(5),
			eBF_FlushEachWrite									= BIT(6),
			eBF_ForceInsertNewline							= BIT(7),
			eBF_OutputHeader										= BIT(8),
			eBF_OutputFooter										= BIT(9),
			eBF_OutputToFile										= BIT(10),
			eBF_OutputToDebugger								= BIT(11),
			eBF_OutputToDebuggerStrippedHeader	= BIT(12),

			eBF_Default													= eBF_Active | eBF_Name | eBF_LineCount | eBF_FrameCount | eBF_TimeStamp | eBF_FlushEachWrite | eBF_ForceInsertNewline | eBF_OutputHeader | eBF_OutputFooter | eBF_OutputToFile | eBF_OutputToDebugger | eBF_OutputToDebuggerStrippedHeader,
			eBF_Mask														= eBF_Active | eBF_Name | eBF_DateStamp | eBF_LineCount | eBF_FrameCount | eBF_TimeStamp | eBF_FlushEachWrite | eBF_ForceInsertNewline | eBF_OutputHeader | eBF_OutputFooter | eBF_OutputToFile | eBF_OutputToDebugger | eBF_OutputToDebuggerStrippedHeader
		}; // End [enum eBehaviourFlag]

		virtual void	SetActive(bool active) = 0;
		virtual bool	IsActive(void) const = 0;

		virtual void	TurnOnBehaviours(eBehaviourFlag behaviours) = 0;
		virtual void	TurnOffBehaviours(eBehaviourFlag behaviours) = 0;
		virtual bool	IsBehaviourOn(eBehaviourFlag behaviour) const = 0;
		virtual bool	PushBehaviours(void) = 0;
		virtual bool	PopBehaviours(void) = 0;

		virtual bool	Write(const char* format, ...) = 0;
	}; // End [struct ILogFile]

	//============================================================================

#if !defined(DEFAULT_LOG_BEHAVIOUR)
#define DEFAULT_LOG_BEHAVIOUR (ILogFile::eBF_Default)
#endif

	//============================================================================

	extern ILogFile* g_MainLog;
	extern ILogFile* g_ErrorLog;
	extern ILogFile* g_WarningLog;
	extern ILogFile* g_AssertLog;
	extern ILogFile* g_ToDoLog;
	extern ILogFile* g_PerformanceLog;

	//----------------------------------------------------------------------------
	// Helper macros for log output
	//----------------------------------------------------------------------------
#if defined LOGS_ENABLED
#define _WriteLog(_pILogFile_, ...) ((_pILogFile_->IsActive()) && (_pILogFile_->Write(__VA_ARGS__)))
#else
#define _WriteLog(_pILogFile_, ...)
#endif

	//----------------------------------------------------------------------------
	// Use LogXXX to output to the logs
	//----------------------------------------------------------------------------
#define Log(...) _WriteLog(engine::g_MainLog, __VA_ARGS__)
#define LogError(...) _WriteLog(engine::g_ErrorLog, __VA_ARGS__)
#define LogWarning(...) _WriteLog(engine::g_WarningLog, __VA_ARGS__)
#define LogAssert(...) _WriteLog(engine::g_AssertLog, __VA_ARGS__)
#define LogToDo(...) _WriteLog(engine::g_ToDoLog, __VA_ARGS__)
#define LogPerformance(...) _WriteLog(engine::g_PerformanceLog, __VA_ARGS__)

	//============================================================================
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__ILOGFILE_H__)]
// [EOF]
