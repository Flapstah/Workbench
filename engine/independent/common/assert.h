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
		engine::g_AssertLog->TurnOnBehaviours(static_cast<engine::ILogFile::eBehaviourFlag>(engine::ILogFile::eBF_SuspendOutputHeader | engine::ILogFile::eBF_SuspendOutputFooter)); \
		LogAssert("%s(%i): ", __FILE__, __LINE__, #_condition_); \
		engine::g_AssertLog->TurnOffBehaviours(static_cast<engine::ILogFile::eBehaviourFlag>(engine::ILogFile::eBF_SuspendOutputHeader | engine::ILogFile::eBF_SuspendOutputFooter)); \
		LogAssert("condition (%s)", #_condition_); \
		DebugBreak(); \
	}

#define assertf(_condition_, ...) \
	if (!(_condition_)) \
	{ \
		engine::g_AssertLog->TurnOnBehaviours(static_cast<engine::ILogFile::eBehaviourFlag>(engine::ILogFile::eBF_SuspendOutputHeader | engine::ILogFile::eBF_SuspendOutputFooter)); \
		LogAssert("%s(%i): ", __FILE__, __LINE__, #_condition_); \
		engine::g_AssertLog->TurnOffBehaviours(engine::ILogFile::eBF_SuspendOutputHeader); \
		LogAssert("condition (%s) : ", #_condition_); \
		engine::g_AssertLog->TurnOnBehaviours(engine::ILogFile::eBF_SuspendOutputHeader); \
		engine::g_AssertLog->TurnOffBehaviours(engine::ILogFile::eBF_SuspendOutputFooter); \
		LogAssert(__VA_ARGS__); \
		engine::g_AssertLog->TurnOffBehaviours(engine::ILogFile::eBF_SuspendOutputHeader); \
		DebugBreak(); \
	}
#endif

	//============================================================================
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__ASSERT_H__)]
// [EOF]
