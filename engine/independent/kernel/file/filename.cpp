
//==============================================================================

#include "stdafx.h"
#include "kernel/file/filename.h"

//==============================================================================

namespace engine
{
	//============================================================================

	bool CFileName::Create(TCHAR* pBuffer, size_t bufferSize, const TCHAR* name, eFileType fileType)
	{
		return Platform_Create(pBuffer, bufferSize, name, fileType);
	}

	//============================================================================

} // End [namespace engine]
//==============================================================================
// [EOF]
