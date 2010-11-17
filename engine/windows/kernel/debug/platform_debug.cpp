#include "stdafx.h"

#include "kernel/debug/debug.h"

//==============================================================================

namespace engine
{
	//============================================================================

	void CDebug::OutputToDebugger(const wchar_t* output)
	{
		OutputDebugStringW(output);
	}

	//============================================================================

	void CDebug::OutputToDebugger(const char* output)
	{
		OutputDebugStringA(output);
	}

	//============================================================================
} // End [namespace engine]

//==============================================================================
// [EOF]
