#include "stdafx.h"

#include "kernel/console/console.h"

//==============================================================================

namespace engine
{
	//============================================================================

	const char* CConsole::CVariantInt32::GetAsString(void)
	{
		// TODO:
		return NULL;
	}

	//============================================================================

	const char* CConsole::CVariantFloat::GetAsString(void)
	{
		// TODO:
		return NULL;
	}

	//============================================================================

	IConsole::IVariant* CConsole::RegisterVariable(const char* name, int32& variable, int32 initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback)
	{

	}
	//----------------------------------------------------------------------------
	// The global instance of the console
	//----------------------------------------------------------------------------
	CConsole g_console;

	//============================================================================

	IConsole* GetConsole(void)
	{
		return &g_console;
	}

	//============================================================================
} // End [namespace engine]

//==============================================================================
// [EOF]
