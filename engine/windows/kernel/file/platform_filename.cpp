
//==============================================================================

#include "stdafx.h"

#include "common/ifile.h"
#include "kernel/file/filename.h"

#include <shlobj.h>
#include <shlwapi.h>

//==============================================================================

namespace engine
{
	//============================================================================

	const TCHAR* CFileName::Platform_Create(const TCHAR* name, eFileType fileType)
	{
		TCHAR buffer[MAX_PATH];
		TCHAR* pPath = NULL;

#if defined(RELEASE)
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, buffer)))
#endif
		{
			TCHAR* pFolder = NULL;
			TCHAR* pExtension = NULL;

#if defined(RELEASE)
			BOOL appended = PathAppend(buffer, WIDEN(COMPANY_NAME));
			if (appended == TRUE)
			{
				appended = PathAppend(buffer, WIDEN(PROJECT_NAME));
			}

			if (appended = TRUE)
#else
			BOOL appended = TRUE;
#endif
			{
				switch (fileType)
				{
				case eFT_LogFile:
					pFolder = WIDEN("Logs");
					pExtension = WIDEN(".log");
					break;
				default:
					break;
				}
			}

			if (pFolder != NULL)
			{
				appended = PathAppend(buffer, pFolder);
			}

			if (appended == TRUE)
			{
				appended = PathAppend(buffer, name);
			}

			if ((appended == TRUE) && (pExtension != NULL))
			{
				appended = PathAppend(buffer, pExtension);
			}

			if (appended == TRUE)
			{
				pPath = buffer;
			}
		}

		return pPath;
	}

	//============================================================================

} // End [namespace engine]
//==============================================================================
// [EOF]
