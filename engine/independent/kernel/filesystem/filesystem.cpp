#include "stdafx.h"

#include "kernel/filesystem/filesystem.h"

#include <direct.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

//==============================================================================

namespace engine
{
	//============================================================================

	CFileSystem::CFileSystem(void)
	{
		for (uint32 index = 0; index < MAX_OPEN_FILES; ++index)
		{
			m_handle[index].m_systemHandle = NULL;
		}
	}

	//============================================================================

	CFileSystem::~CFileSystem(void)
	{
		CloseAllFiles();
	}

	//============================================================================

	CFileSystem::eFileSystemError CFileSystem::DirectoryExists(TCHAR* path)
	{
		struct _stat64 info;
		eFileSystemError error = eFSE_ERROR;

		if (_tstat64(path, &info) == 0)
		{
			if (info.st_mode & _S_IFDIR)
			{
				error = eFSE_SUCCESS;
			}
		}
		else
		{
			switch (errno)
			{
			case ENOENT:
				error = eFSE_DOES_NOT_EXIST;
				break;
			}
		}

		return error;
	}

	//============================================================================

	CFileSystem::eFileSystemError CFileSystem::CreateDirectory(TCHAR* path)
	{
		eFileSystemError error = eFSE_ERROR;

		if (_tmkdir(path) == 0)
		{
			error = eFSE_SUCCESS;
		}
		else
		{
			switch (errno)
			{
			case EEXIST:
				error = eFSE_ALREADY_EXISTS;
				break;

			case ENOENT:
				error = eFSE_PATH_NOT_FOUND;
				break;
			}
		}

		return error;
	}

	//============================================================================

#if defined(FILE_SYSTEM_EXTENDED)
	CFileSystem::eFileSystemError	CFileSystem::CopyDirectory(TCHAR* sourcePath, TCHAR* destinationPath)
	{
		// TODO:
		return eFSE_ERROR;
	}

	//============================================================================

	CFileSystem::eFileSystemError	CFileSystem::MoveDirectory(TCHAR* sourcePath, TCHAR* destinationPath)
	{
		// TODO:
		return eFSE_ERROR;
	}

	//============================================================================

	CFileSystem::eFileSystemError	CFileSystem::DeleteDirectory(TCHAR* path, bool force)
	{
		// TODO:
		return eFSE_ERROR;
	}
#endif

	//============================================================================

	CFileSystem::eFileSystemError CFileSystem::FileExists(TCHAR* name)
	{
		return eFSE_ERROR;
	}

	//============================================================================

	CFileSystem::eFileSystemHandle CFileSystem::OpenFile(TCHAR* name, TCHAR* mode)
	{
		eFileSystemHandle handle = eFSH_INVALID;

		for (uint32 index = 0; (index < MAX_OPEN_FILES) && (handle == eFSH_INVALID); ++index)
		{
			if (m_handle[index].m_systemHandle == NULL)
			{
				if (_tfopen_s(&m_handle[index].m_systemHandle, name, mode))
				{
					m_handle[index].m_systemHandle = NULL;
				}
				else
				{
					handle = static_cast<eFileSystemHandle>(index);
				}
			}
		}

		return handle;
	}

	//============================================================================

#if defined(FILE_SYSTEM_EXTENDED)
	CFileSystem::eFileSystemError	CFileSystem::CopyFile(TCHAR* sourceName, TCHAR* destinationName)
	{
		// TODO:
		return eFSE_ERROR;
	}

	//============================================================================

	CFileSystem::eFileSystemError	CFileSystem::MoveFile(TCHAR* sourceName, TCHAR* destinationName)
	{
		// TODO:
		return eFSE_ERROR;
	}

	//============================================================================

	CFileSystem::eFileSystemError	CFileSystem::DeleteFile(TCHAR* name, bool force)
	{
		// TODO:
		return eFSE_ERROR;
	}
#endif

	//============================================================================

	size_t CFileSystem::Read(eFileSystemHandle handle, void* pBuffer, size_t bufferSize, size_t itemSize, size_t itemCount)
	{
		int32 itemsRead = 0;

		if (handle < MAX_OPEN_FILES)
		{
			if (m_handle[handle].m_systemHandle != NULL)
			{
				itemsRead = fread_s(pBuffer, bufferSize, itemSize, itemCount, m_handle[handle].m_systemHandle);

				assert(itemsRead == itemCount);
			}
		}

		return itemsRead;
	}

	//============================================================================

	size_t CFileSystem::Write(eFileSystemHandle handle, const void* pBuffer, size_t itemSize, size_t itemCount)
	{
		size_t itemsWritten = 0;

		if (handle < MAX_OPEN_FILES)
		{
			if (m_handle[handle].m_systemHandle != NULL)
			{
				itemsWritten = fwrite(pBuffer, itemSize, itemCount, m_handle[handle].m_systemHandle);

				assert(itemsWritten == itemCount);
			}
		}

		return itemsWritten;
	}

	//============================================================================

	size_t CFileSystem::Print(eFileSystemHandle handle, const TCHAR* format, ...)
	{
		int32 charsWritten = 0;

		if (handle < MAX_OPEN_FILES)
		{
			va_list arguments;
			va_start(arguments, format);

			if (m_handle[handle].m_systemHandle != NULL)
			{
				charsWritten = _vftprintf_s(m_handle[handle].m_systemHandle, format, arguments);
			}
		}

		return charsWritten;
	}

	//============================================================================

	CFileSystem::eFileSystemError CFileSystem::Flush(eFileSystemHandle handle)
	{
		eFileSystemError rc = eFSE_INVALID_HANDLE;

		if (handle < MAX_OPEN_FILES)
		{
			if (m_handle[handle].m_systemHandle != NULL)
			{
				fflush(m_handle[handle].m_systemHandle);
				rc = eFSE_SUCCESS;
			}
		}

		return rc;
	}

	//============================================================================

	CFileSystem::eFileSystemError CFileSystem::CloseFile(eFileSystemHandle& handle)
	{
		eFileSystemError rc = eFSE_INVALID_HANDLE;

		if (handle < MAX_OPEN_FILES)
		{
			if (m_handle[handle].m_systemHandle != NULL)
			{
				fclose(m_handle[handle].m_systemHandle);
				m_handle[handle].m_systemHandle = NULL;

				handle = eFSH_INVALID;
				rc = eFSE_SUCCESS;
			}
		}

		return rc;
	}

	//============================================================================

	CFileSystem::eFileSystemError CFileSystem::CloseAllFiles(void)
	{
		eFileSystemError rc = eFSE_SUCCESS;

		for (uint32 index = 0; index < MAX_OPEN_FILES; ++index)
		{
			if (m_handle[index].m_systemHandle != NULL)
			{
				eFileSystemHandle handle(static_cast<eFileSystemHandle>(index));
				eFileSystemError closed = CloseFile(handle);
				if (closed != eFSE_SUCCESS)
				{
					rc = closed;
				}
			}
		}

		return rc;
	}

	//============================================================================

	CFileSystem::eFileSystemError CFileSystem::CreatePath(TCHAR* pBuffer, size_t bufferSize, const TCHAR* name, eFileType fileType, bool createIfNecessary)
	{
		return Platform_CreatePath(pBuffer, bufferSize, name, fileType, createIfNecessary);
	}

	//============================================================================

	//----------------------------------------------------------------------------
	// The global instance of the file system
	//----------------------------------------------------------------------------
	CFileSystem g_fileSystem;

	//============================================================================

	IFileSystem* GetFileSystem(void)
	{
		return &g_fileSystem;
	}

	//============================================================================
} // End [namespace engine]

//==============================================================================
// [EOF]
