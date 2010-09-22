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

#define assertf(_condition_, ...) \
	if (!(_condition_)) \
	{ \
		LogAssert((_TEXT("%s(%i): [Assert] condition (%s)"), _TEXT(__FILE__), __LINE__, _TEXT(#_condition_))); \
		LogAssert(__VA_ARGS__); \
		DebugBreak(); \
	}
#endif

	//============================================================================
} // End [namespace engine]

//==============================================================================
#endif // End [!defined(__ASSERT_H__)]
// [EOF]
