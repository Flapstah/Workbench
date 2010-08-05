#if !defined(__FILENAME_H__)
#define __FILENAME_H__

//==============================================================================

namespace engine
{
	//============================================================================
	// CFileName
	//============================================================================
	class CFileName
	{
	public:
		enum eFileType
		{
			eFT_LogFile,

			eFT_Max
		};

		static bool Create(TCHAR* pBuffer, size_t bufferSize, const TCHAR* name, eFileType fileType);

	protected:
		static bool Platform_Create(TCHAR* pBuffer, size_t bufferSize, const TCHAR* name, eFileType fileType);
	}; // End [class CFileName]
} // End [namespace engine]

//==============================================================================
#endif // End [!defined(__FILENAME_H__)]
// [EOF]
