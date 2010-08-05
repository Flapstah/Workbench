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
		CFile(void);
		CFile(const TCHAR* name, const TCHAR* mode);
		~CFile(void);

		bool Open(const TCHAR* name, const TCHAR* mode);
		void Close(void);

		bool IsOpen(void) const { return (m_pFile != NULL); }

		size_t Read(void* pBuffer, size_t bufferSize, size_t itemSize, size_t itemCount);
		size_t Write(const void* pBuffer, size_t itemSize, size_t itemCount);
		size_t Print(const TCHAR* format, ...);

	protected:
		bool Platform_EnsurePathExists(const TCHAR* name);

	protected:
		FILE* m_pFile;
	}; // End [class CFile]
} // End [namespace engine]

//==============================================================================
#endif // End [!defined(__IFILE_H__)]
// [EOF]
