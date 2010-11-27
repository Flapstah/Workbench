#include "stdafx.h"

#include "kernel/filesystem/filesystem.h"

#include <shlobj.h>
#include <shlwapi.h>

//==============================================================================

namespace engine
{
	//============================================================================

	CFileSystem::eFileSystemError CFileSystem::Platform_CreatePath(char* pBuffer, size_t bufferSize, const char* name, eFileType fileType, bool createIfNecessary)
	{
		pBuffer[0] = _TEXT('\0');
		BOOL appended = FALSE;
		eFileSystemError error = eFSE_SUCCESS;

#if defined(RELEASE)
		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, buffer)))
#endif
		{
			char* pFolder = NULL;
			char* pExtension = NULL;

#if defined(RELEASE)
			appended = PathAppendA(pBuffer, COMPANY_NAME);
			if (appended == TRUE)
			{
				appended = PathAppendA(pBuffer, PROJECT_NAME);
			}

			if (appended = TRUE)
#else
			appended = TRUE;
#endif
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
				appended = PathAppendA(pBuffer, pFolder);

				if (DirectoryExists(pBuffer) == eFSE_DOES_NOT_EXIST)
				{
					error = CreateDirectory(pBuffer);
				}
			}

			if (error == eFSE_SUCCESS)
			{
				if (appended == TRUE)
				{
					appended = PathAppendA(pBuffer, name);
				}

				if ((appended == TRUE) && (pExtension != NULL))
				{
					appended = (strcat_s(pBuffer, bufferSize, pExtension) == 0);
				}

				if (appended == FALSE)
				{
					error = eFSE_INVALID_FILENAME; 
				}
			}
		}

		return error;
	}

	//============================================================================
} // End [namespace engine]

//==============================================================================
// [EOF]
