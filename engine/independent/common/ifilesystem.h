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

    virtual                   ~IFileSystem(void) {};

		virtual eFileSystemError	DirectoryExists(const char* path) = 0;
		virtual eFileSystemError	CreateDirectory(const char* path) = 0;
#if defined(FILE_SYSTEM_EXTENDED)
		virtual eFileSystemError	CopyDirectory(const char* sourcePath, const char* destinationPath) = 0;
		virtual eFileSystemError	MoveDirectory(const char* sourcePath, const char* destinationPath) = 0;
		virtual eFileSystemError	DeleteDirectory(const char* path, const bool force) = 0;
#endif

		//==========================================================================

		virtual eFileSystemError	FileExists(const char* name) = 0;
		virtual eFileSystemHandle	OpenFile(const char* name, const char* mode) = 0;
#if defined(FILE_SYSTEM_EXTENDED)
		virtual eFileSystemError	CopyFile(const char* sourceName, const char* destinationName) = 0;
		virtual eFileSystemError	MoveFile(const char* sourceName, const char* destinationName) = 0;
		virtual eFileSystemError	DeleteFile(const char* name, bool force) = 0;
#endif
		virtual size_t						Read(eFileSystemHandle handle, void* pBuffer, size_t itemSize, size_t itemCount) = 0;
		virtual size_t						Write(eFileSystemHandle handle, const void* pBuffer, size_t itemSize, size_t itemCount) = 0;
		virtual size_t						Print(eFileSystemHandle handle, const char* format, ...) = 0;
		virtual eFileSystemError	Flush(eFileSystemHandle handle) = 0;
		virtual eFileSystemError	CloseFile(eFileSystemHandle& handle) = 0;
		virtual eFileSystemError	CloseAllFiles(void) = 0;

		//==========================================================================

		virtual eFileSystemError	CreatePath(char* pBuffer, size_t bufferSize, const char* name, eFileType fileType, bool createIfNecessary) = 0;

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
