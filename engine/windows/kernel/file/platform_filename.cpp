
//==============================================================================

#include "stdafx.h"
#include "filename.h"

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

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, buffer)))
		{
			TCHAR* pFolder = NULL;
			TCHAR* pExtension = NULL;

			BOOL appended = PathAppend(buffer, COMPANY_NAME);
			if (appended == TRUE)
			{
				appended = PathAppend(buffer, PROJECT_NAME);
			}

			if (appended = TRUE)
			{
				switch (fileType)
				{
				case eFT_LogFile:
					pFolder = "Logs";
					pExtension = ".log";
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
