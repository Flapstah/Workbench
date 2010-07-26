#if !defined(__IFILE_H__)
#define __IFILE_H__

//==============================================================================

#define COMPANY_NAME "MyCompanyName"
#define PROJECT_NAME "MyProjectName"

//==============================================================================

namespace engine
{
	class CFile
	{
	public:
		CFile(const TCHAR* name, const TCHAR* mode);
		~CFile(void);

		int32 Read(void* pBuffer, uint32 itemSize, uint32 itemCount);
		int32 Write(const void* pBuffer, uint32 itemSize, uint32 itemCount);
		int32 Print(const TCHAR* format, ...);

	protected:
		TCHAR* Platform_GetPath(const TCHAR* name);

	protected:
		FILE* m_pFile;
	};
}

//==============================================================================
#endif // End [!defined(__IFILE_H__)]
// [EOF]
