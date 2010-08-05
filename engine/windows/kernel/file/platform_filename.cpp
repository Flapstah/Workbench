
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

	bool CFileName::Platform_Create(TCHAR* pBuffer, size_t bufferSize, const TCHAR* name, eFileType fileType)
	{
		pBuffer[0] = WIDEN('\0');
		BOOL appended = FALSE;

#if defined(RELEASE)
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, buffer)))
#endif
		{
			TCHAR* pFolder = NULL;
			TCHAR* pExtension = NULL;

#if defined(RELEASE)
			appended = PathAppend(pBuffer, WIDEN(COMPANY_NAME));
			if (appended == TRUE)
			{
				appended = PathAppend(pBuffer, WIDEN(PROJECT_NAME));
			}

			if (appended = TRUE)
#else
			appended = TRUE;
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
				appended = PathAppend(pBuffer, pFolder);
			}

			if (appended == TRUE)
			{
				appended = PathAppend(pBuffer, name);
			}

			if ((appended == TRUE) && (pExtension != NULL))
			{
				appended = (_tcscat_s(pBuffer, bufferSize, pExtension) == 0);
			}
		}

		return (appended == TRUE);
	}

	//============================================================================

} // End [namespace engine]
//==============================================================================
// [EOF]
