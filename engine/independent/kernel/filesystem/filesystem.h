#if !defined(__FILESYSTEM_H__)
#define __FILESYSTEM_H__

//==============================================================================

#include "common/ifilesystem.h"

//==============================================================================

#define MAX_OPEN_FILES (8)

//==============================================================================

namespace engine
{
	//============================================================================
	// CFileSystem
	//============================================================================
	class CFileSystem : public IFileSystem
	{
	public:
		CFileSystem(void);
		virtual ~CFileSystem(void);

		// IFileSystem
		virtual eFileSystemError	DirectoryExists(char* path);
		virtual eFileSystemError	CreateDirectory(char* path);
#if defined(FILE_SYSTEM_EXTENDED)
		virtual eFileSystemError	CopyDirectory(char* sourcePath, char* destinationPath);
		virtual eFileSystemError	MoveDirectory(char* sourcePath, char* destinationPath);
		virtual eFileSystemError	DeleteDirectory(char* path, bool force);
#endif

		virtual eFileSystemError	FileExists(char* name);
		virtual eFileSystemHandle	OpenFile(char* name, char* mode);
#if defined(FILE_SYSTEM_EXTENDED)
		virtual eFileSystemError	CopyFile(char* sourceName, char* destinationName);
		virtual eFileSystemError	MoveFile(char* sourceName, char* destinationName);
		virtual eFileSystemError	DeleteFile(char* name, bool force);
#endif
		virtual size_t						Read(eFileSystemHandle handle, void* pBuffer, size_t bufferSize, size_t itemSize, size_t itemCount);
		virtual size_t						Write(eFileSystemHandle handle, const void* pBuffer, size_t itemSize, size_t itemCount);
		virtual size_t						Print(eFileSystemHandle handle, const char* format, ...);
		virtual eFileSystemError	Flush(eFileSystemHandle handle);
		virtual eFileSystemError	CloseFile(eFileSystemHandle& handle);
		virtual eFileSystemError	CloseAllFiles(void);

		virtual eFileSystemError	CreatePath(char* pBuffer, size_t bufferSize, const char* name, eFileType fileType, bool createIfNecessary);
		// ~IFileSystem

	protected:
		virtual eFileSystemError	Platform_CreatePath(char* pBuffer, size_t bufferSize, const char* name, eFileType fileType, bool createIfNecessary);

	protected:
		struct FileHandle
		{
			FILE*		m_systemHandle;
		}	m_handle[MAX_OPEN_FILES];
	}; // End [class CFileSystem : public IFileSystem]

	//============================================================================
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__FILESYSTEM_H__)]
// [EOF]
