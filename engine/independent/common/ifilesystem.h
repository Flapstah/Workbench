#if !defined(__IFILESYSTEM_H__)
#define __IFILESYSTEM_H__

//==============================================================================

#define COMPANY_NAME "MyCompanyName"
#define PROJECT_NAME "MyProjectName"

//==============================================================================

namespace engine
{
	struct IFileSystem
	{
		//==========================================================================

		enum eFileSystemHandle
		{
			eFSH_INVALID = 0xffffffff
		}; // End [enum eFileSystemHandle]

		//==========================================================================

		enum eFileSystemError
		{
			// General
			eFSE_SUCCESS = 0,
			eFSE_ERROR,
			eFSE_DOES_NOT_EXIST,
			eFSE_INVALID,
			eFSE_INVALID_PRIVILEGES,
			eFSE_READ_ONLY,
			eFSE_LOCKED,

			eFSE_GENERAL_RESULT_MASK	= 0x0000ffff,

			// Qualifier`
			eFSE_DIRECTORY						= 0x00010000,
			eFSE_FILE									= 0x00020000,
			eFSE_PATH									= 0x00040000
		}; // End [enum eFileSystemError]

		//==========================================================================

		enum eFileType
		{
			eFT_LogFile,

			eFT_Max
		}; // End [enum eFileType]

		//==========================================================================

		virtual eFileSystemError	DirectoryExists(TCHAR* path) = 0;
		virtual eFileSystemError	CreateDirectory(TCHAR* path) = 0;
		virtual eFileSystemError	DeleteDirectory(TCHAR* path, bool force) = 0;

		//==========================================================================

		virtual eFileSystemError	FileExists(TCHAR* name) = 0;
		virtual eFileSystemHandle	OpenFile(TCHAR* name, TCHAR* mode) = 0;
		virtual size_t						Read(eFileSystemHandle handle, void* pBuffer, size_t bufferSize, size_t itemSize, size_t itemCount) = 0;
		virtual size_t						Write(eFileSystemHandle handle, const void* pBuffer, size_t itemSize, size_t itemCount) = 0;
		virtual size_t						Print(eFileSystemHandle handle, const TCHAR* format, ...) = 0;
		virtual eFileSystemError	CloseFile(eFileSystemHandle handle) = 0;
		virtual eFileSystemError	CloseAllFiles(void) = 0;

		//==========================================================================

		virtual eFileSystemError	CreatePath(TCHAR* pBuffer, size_t bufferSize, const TCHAR* name, eFileType fileType) = 0;

		//==========================================================================
	}; // End [struct IFileSystem]

	IFileSystem* GetFileSystem(void);
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__IFILESYSTEM_H__)]
// [EOF]
