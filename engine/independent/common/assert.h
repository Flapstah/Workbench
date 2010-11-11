#if !defined(__ASSERT_H__)
#define __ASSERT_H__

//==============================================================================

#include "common/ilogfile.h"

//==============================================================================

namespace engine
{
	//============================================================================

#if defined(NO_ASSERTS)
#define assert(_condition_)
#define assertf(_condition_, _reason_)
#else
#define assert(_condition_) \
	if (!(_condition_)) \
	{ \
		LogAssert(_TEXT("%s(%i): condition (%s)"), _TEXT(__FILE__), __LINE__, _TEXT(#_condition_)); \
		DebugBreak(); \
	}

#define assertf(_condition_, ...) \
	if (!(_condition_)) \
	{ \
		engine::ILogFile::eBehaviourFlag behaviourFlags = engine::g_AssertLog->GetBehaviours(); \
		engine::g_AssertLog->TurnOffBehaviours(engine::ILogFile::eBF_ForceInsertNewline); \
		LogAssert(_TEXT("%s(%i): condition (%s) : "), _TEXT(__FILE__), __LINE__, _TEXT(#_condition_)); \
		engine::g_AssertLog->SetBehaviours(static_cast<engine::ILogFile::eBehaviourFlag>(engine::ILogFile::eBF_OutputToDebugger | engine::ILogFile::eBF_FlushEachWrite)); \
		LogAssert(__VA_ARGS__); \
		engine::g_AssertLog->SetBehaviours(behaviourFlags); \
		DebugBreak(); \
	}
#endif

	//============================================================================
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__ASSERT_H__)]
// [EOF]
