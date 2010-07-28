#if !defined(__IFILE_H__)
#define __IFILE_H__

//==============================================================================

#define COMPANY_NAME "MyCompanyName"
#define PROJECT_NAME "MyProjectName"

//==============================================================================

namespace engine
{
	//============================================================================
	// CFile
	//============================================================================
	class CFile
	{
	public:
		CFile(const TCHAR* name, const TCHAR* mode);
		~CFile(void);

		size_t Read(void* pBuffer, size_t bufferSize, size_t itemSize, size_t itemCount);
		size_t Write(const void* pBuffer, size_t itemSize, size_t itemCount);
		size_t Print(const TCHAR* format, ...);

	protected:
		FILE* m_pFile;
	}; // End [class CFile]
} // End [namespace engine]

//==============================================================================
#endif // End [!defined(__IFILE_H__)]
// [EOF]
