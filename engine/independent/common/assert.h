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
		LogAssert((_TEXT("%s(%i): [Assert] condition (%s)"), _TEXT(__FILE__), __LINE__, _TEXT(#_condition_))); \
		DebugBreak(); \
	}

#define assertf(_condition_, _reason_) \
	if (!(_condition_)) \
	{ \
		LogAssert((_TEXT("%s(%i): [Assert] condition (%s)"), _TEXT(__FILE__), __LINE__, _TEXT(#_condition_))); \
		bool usesName = engine::g_AssertLog->IsBehaviourOn(engine::ILogFile::eBF_Name); \
		engine::g_AssertLog->TurnOnBehaviours(engine::ILogFile::eBF_Name); \
		LogAssert(_reason_); \
		if (!usesName) \
		{ \
			engine::g_AssertLog->TurnOffBehaviours(engine::ILogFile::eBF_Name); \
		} \
		DebugBreak(); \
	}
#endif

	//============================================================================
} // End [namespace engine]

//==============================================================================
#endif // End [!defined(__ASSERT_H__)]
// [EOF]
