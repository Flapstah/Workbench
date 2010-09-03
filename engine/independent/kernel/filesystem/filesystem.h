#if !defined(__FILESYSTEM_H__)
#define __FILESYSTEM_H__

//==============================================================================

#include "common/ifilesystem.h"

//==============================================================================

#define MAX_OPEN_FILES (8)

//==============================================================================

namespace engine
{
	class CFileSystem : public IFileSystem
	{
	public:
		CFileSystem(void);
		virtual ~CFileSystem(void);

		// IFileSystem
		virtual eFileSystemError	DirectoryExists(TCHAR* path);
		virtual eFileSystemError	CreateDirectory(TCHAR* path);
#if defined(FILE_SYSTEM_EXTENDED)
		virtual eFileSystemError	CopyDirectory(TCHAR* sourcePath, TCHAR* destinationPath);
		virtual eFileSystemError	MoveDirectory(TCHAR* sourcePath, TCHAR* destinationPath);
		virtual eFileSystemError	DeleteDirectory(TCHAR* path, bool force);
#endif

		virtual eFileSystemError	FileExists(TCHAR* name);
		virtual eFileSystemHandle	OpenFile(TCHAR* name, TCHAR* mode);
#if defined(FILE_SYSTEM_EXTENDED)
		virtual eFileSystemError	CopyFile(TCHAR* sourceName, TCHAR* destinationName);
		virtual eFileSystemError	MoveFile(TCHAR* sourceName, TCHAR* destinationName);
		virtual eFileSystemError	DeleteFile(TCHAR* name, bool force);
#endif
		virtual size_t						Read(eFileSystemHandle handle, void* pBuffer, size_t bufferSize, size_t itemSize, size_t itemCount);
		virtual size_t						Write(eFileSystemHandle handle, const void* pBuffer, size_t itemSize, size_t itemCount);
		virtual size_t						Print(eFileSystemHandle handle, const TCHAR* format, ...);
		virtual eFileSystemError	Flush(eFileSystemHandle handle);
		virtual eFileSystemError	CloseFile(eFileSystemHandle& handle);
		virtual eFileSystemError	CloseAllFiles(void);

		virtual eFileSystemError	CreatePath(TCHAR* pBuffer, size_t bufferSize, const TCHAR* name, eFileType fileType, bool createIfNecessary);
		// ~IFileSystem

	protected:
		virtual eFileSystemError	Platform_CreatePath(TCHAR* pBuffer, size_t bufferSize, const TCHAR* name, eFileType fileType, bool createIfNecessary);

	protected:
		struct FileHandle
		{
			FILE*		m_systemHandle;
		}	m_handle[MAX_OPEN_FILES];
	};
}

//==============================================================================

#endif // End [!defined(__FILESYSTEM_H__)]
// [EOF]
