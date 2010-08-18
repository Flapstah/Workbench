#include "stdafx.h"

#include "kernel/filesystem/filesystem.h"

#include <shlobj.h>
#include <shlwapi.h>

//==============================================================================

namespace engine
{
	//============================================================================

	CFileSystem::eFileSystemError CFileSystem::Platform_CreatePath(TCHAR* pBuffer, size_t bufferSize, const TCHAR* name, eFileType fileType, bool createIfNecessary)
	{
		pBuffer[0] = WIDEN('\0');
		BOOL appended = FALSE;
		eFileSystemError error = eFSE_SUCCESS;

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

				if (DirectoryExists(pBuffer) == eFSE_DOES_NOT_EXIST)
				{
					error = CreateDirectory(pBuffer);
				}
			}

			if (error == eFSE_SUCCESS)
			{
				if (appended == TRUE)
				{
					appended = PathAppend(pBuffer, name);
				}

				if ((appended == TRUE) && (pExtension != NULL))
				{
					appended = (_tcscat_s(pBuffer, bufferSize, pExtension) == 0);
				}

				if (appended == FALSE)
				{
					error = eFSE_INVALID; 
				}
			}
		}

		return error;
	}

	//============================================================================

} // End [namespace engine]

//==============================================================================
// [EOF]
