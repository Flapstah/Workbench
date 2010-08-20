#if !defined(__IFILESYSTEM_H__)
#define __IFILESYSTEM_H__

//==============================================================================

#define COMPANY_NAME "MyCompanyName"
#define PROJECT_NAME "MyProjectName"

//#define FILE_SYSTEM_EXTENDED
// TODO: Need to flesh out extended file system functions
// TODO: Need some sort of enumerator to walk file/directories

//==============================================================================

namespace engine
{
	//============================================================================
	// IFileSystem
	//============================================================================
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
			eFSE_ALREADY_EXISTS,
			eFSE_DOES_NOT_EXIST,
			eFSE_PATH_NOT_FOUND,
			eFSE_INVALID_FILENAME,
			eFSE_INVALID_HANDLE,
			eFSE_INVALID_PRIVILEGES,
			eFSE_READ_ONLY,
			eFSE_LOCKED

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
#if defined(FILE_SYSTEM_EXTENDED)
		virtual eFileSystemError	CopyDirectory(TCHAR* sourcePath, TCHAR* destinationPath) = 0;
		virtual eFileSystemError	MoveDirectory(TCHAR* sourcePath, TCHAR* destinationPath) = 0;
		virtual eFileSystemError	DeleteDirectory(TCHAR* path, bool force) = 0;
#endif

		//==========================================================================

		virtual eFileSystemError	FileExists(TCHAR* name) = 0;
		virtual eFileSystemHandle	OpenFile(TCHAR* name, TCHAR* mode) = 0;
#if defined(FILE_SYSTEM_EXTENDED)
		virtual eFileSystemError	CopyFile(TCHAR* sourceName, TCHAR* destinationName) = 0;
		virtual eFileSystemError	MoveFile(TCHAR* sourceName, TCHAR* destinationName) = 0;
		virtual eFileSystemError	DeleteFile(TCHAR* name, bool force) = 0;
#endif
		virtual size_t						Read(eFileSystemHandle handle, void* pBuffer, size_t bufferSize, size_t itemSize, size_t itemCount) = 0;
		virtual size_t						Write(eFileSystemHandle handle, const void* pBuffer, size_t itemSize, size_t itemCount) = 0;
		virtual size_t						Print(eFileSystemHandle handle, const TCHAR* format, ...) = 0;
		virtual eFileSystemError	Flush(eFileSystemHandle handle) = 0;
		virtual eFileSystemError	CloseFile(eFileSystemHandle& handle) = 0;
		virtual eFileSystemError	CloseAllFiles(void) = 0;

		//==========================================================================

		virtual eFileSystemError	CreatePath(TCHAR* pBuffer, size_t bufferSize, const TCHAR* name, eFileType fileType, bool createIfNecessary) = 0;

		//==========================================================================
	}; // End [struct IFileSystem]

	//----------------------------------------------------------------------------
	// Returns the file system
	//----------------------------------------------------------------------------
	IFileSystem* GetFileSystem(void);

	//============================================================================
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__IFILESYSTEM_H__)]
// [EOF]
