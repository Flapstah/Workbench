#if !defined(__PLATFORM_STDAFX_H__)
#define __PLATFORM_STDAFX_H__

//==============================================================================

#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

//==============================================================================

#define LINUX

//==============================================================================

#define DEBUG_BREAK raise(SIGTRAP)
#define SLEEP(microseconds) usleep(microseconds * 1000)

//==============================================================================

#endif // End [!defined(__PLATFORM_STDAFX_H__)]
// [EOF]
