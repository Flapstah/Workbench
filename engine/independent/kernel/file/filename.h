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

		static const TCHAR* Create(const TCHAR* name, eFileType fileType);

	protected:
		static const TCHAR* Platform_Create(const TCHAR* name, eFileType fileType);
	};
}

//==============================================================================
#endif // End [!defined(__FILENAME_H__)]
// [EOF]
