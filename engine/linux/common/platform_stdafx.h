#if !defined(__PLATFORM_STDAFX_H__)
#define __PLATFORM_STDAFX_H__

//==============================================================================

#include <signal.h>
#include <unistd.h>

//==============================================================================

#define DEBUG_BREAK raise(SIGTRAP)
#define SLEEP(microseconds) usleep(microseconds * 1000)

//==============================================================================

#endif // End [!defined(__PLATFORM_STDAFX_H__)]
// [EOF]
