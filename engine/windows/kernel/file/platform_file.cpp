#include "stdafx.h"

#include "ifile.h"

//==============================================================================

namespace engine
{
	TCHAR* CFile::Platform_GetPath(const TCHAR* name)
	{
		WCHAR buffer[MAX_PATH];

		// Older than Vista
//		if (SUCCEEDED(SHGetFolderPath(NULL, )))
//		{
//		}

		// Vista and later
		PWSTR	pPath = NULL;
		if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Profile, 0, NULL, &pPath)))
		{
			wcscpy_s(buffer, MAX_PATH, pPath);
			CoTaskMemFree(pPath);

			if (PathAppendW(buffer, COMPANY_NAME) == TRUE)
			{
				if (PathAppendW(buffer, PROJECT_NAME) == TRUE)
				{
					if (!PathFileExists(buffer))
					{
						if (SHCreateDirectoryEx(NULL, buffer, NULL) == ERROR_SUCCESS)
						{
							// TODO: Need to widen the name if necessary
							if (PathAppendW(buffer, name))
							{
								return buffer;
							}
						}
					}
				}
			}
		}
	}
}



//==============================================================================
// [EOF]
