#include "stdafx.h"

#include "common/itime.h"

#include "kernel/log/logfile.h"
#include "kernel/debug/debug.h"
#include "kernel/filesystem/filesystem.h"

//------------------------------------------------------------------------------
// Good Unicode resource page: http://www.i18nguy.com/unicode/c-unicode.html
//------------------------------------------------------------------------------

//==============================================================================

namespace engine
{
	//============================================================================

	uint32 CLogFile::s_lineCount = 0;

	//============================================================================

	//----------------------------------------------------------------------------
	// The global instances of logs
	//----------------------------------------------------------------------------
	static SLogFileBuffer gs_MainLogBuffer;
	static CLogFile gs_MainLog(_TEXT("Main"), NULL, static_cast<ILogFile::eBehaviourFlag>(DEFAULT_LOG_BEHAVIOUR), &gs_MainLogBuffer);
	static CLogFile gs_ErrorLog(_TEXT("Error"), &gs_MainLog, static_cast<ILogFile::eBehaviourFlag>(DEFAULT_LOG_BEHAVIOUR), NULL);
	static CLogFile gs_WarningLog(_TEXT("Warning"), &gs_MainLog, static_cast<ILogFile::eBehaviourFlag>(DEFAULT_LOG_BEHAVIOUR), NULL);
	static CLogFile gs_AssertLog(_TEXT("Assert"), &gs_MainLog, static_cast<ILogFile::eBehaviourFlag>(DEFAULT_LOG_BEHAVIOUR), NULL);
	static CLogFile gs_ToDoLog(_TEXT("ToDo"), &gs_MainLog, static_cast<ILogFile::eBehaviourFlag>(DEFAULT_LOG_BEHAVIOUR), NULL);
	static CLogFile gs_PerformanceLog(_TEXT("Performance"), &gs_MainLog, static_cast<ILogFile::eBehaviourFlag>(DEFAULT_LOG_BEHAVIOUR), NULL);

	//============================================================================

	ILogFile* g_MainLog = &gs_MainLog;
	ILogFile* g_ErrorLog = &gs_ErrorLog;
	ILogFile* g_WarningLog = &gs_WarningLog;
	ILogFile* g_AssertLog = &gs_AssertLog;
	ILogFile* g_ToDoLog = &gs_ToDoLog;
	ILogFile* g_PerformanceLog = &gs_PerformanceLog;

	//============================================================================

	CLogFile::CLogFile(const wchar_t* name, CLogFile* pParent, eBehaviourFlag initialBehaviour, SLogFileBuffer* pBuffer)
		: m_pParent(pParent)
		, m_pBuffer(pBuffer)
		, m_behaviours(initialBehaviour | eIBF_Unicode)
	{
		Initialise();

		wcscpy_s(m_name.m_UTF16, LOGFILE_NAME_SIZE, name);
	}

	//============================================================================

	CLogFile::CLogFile(const char* name, CLogFile* pParent, eBehaviourFlag initialBehaviour, SLogFileBuffer* pBuffer)
		: m_pParent(pParent)
		, m_pBuffer(pBuffer)
		, m_behaviours(initialBehaviour)
	{
		Initialise();

		strcpy_s(m_name.m_UTF8, LOGFILE_NAME_SIZE, name);
	}
	//============================================================================

	CLogFile::~CLogFile(void)
	{
		Close();
	}

	//============================================================================

	bool CLogFile::Write(const wchar_t* format, ...)
	{
		bool writtenToFile = false;
		if (m_pBuffer != NULL)
		{
			CAutoMutexLock(m_pBuffer->m_mutex);
			m_pBuffer->m_previousSize = m_pBuffer->m_size;

			if (!(m_behaviours & eBF_SuspendOutputHeader))
			{
				if (m_behaviours & eBF_DateStamp)
				{
					InsertDateStampUTF16();
				}

				if (m_behaviours & eBF_LineCount)
				{
					InsertLineCountUTF16();
				}

				const ITimer* pGameClock = GetGameClock();
				if (m_behaviours & eBF_FrameCount)
				{
					InsertFrameCountUTF16(pGameClock);
				}

				if (m_behaviours & eBF_TimeStamp)
				{
					InsertTimeStampUTF16(pGameClock);
				}

				if (m_behaviours & eBF_Name)
				{
					InsertNameUTF16();
				}

				if (m_pBuffer->m_previousSize != m_pBuffer->m_size)
				{
					if (wcscat_s(&m_pBuffer->m_buffer[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, _TEXT(" ")) == 0)
					{
						++m_pBuffer->m_size;
					}
				}
			}

			va_list arguments;
			va_start(arguments, format);
			m_pBuffer->m_size += vswprintf_s(&m_pBuffer->m_buffer[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, format, arguments);

			if (!(m_behaviours & eBF_SuspendOutputFooter))
			{
				if (m_behaviours & eBF_ForceInsertNewline)
				{
					ForceInsertNewlineUTF16();
				}
			}

			if (m_behaviours & eBF_OutputToDebugger)
			{
				CDebug::OutputToDebugger(&m_pBuffer->m_buffer[m_pBuffer->m_previousSize]);
			}

			uint32 bufferUsedCapacity = (m_pBuffer->m_size << 4) / LOGFILE_BUFFER_SIZE;
			if ((m_behaviours & eBF_FlushEachWrite) || (bufferUsedCapacity >= LOGS_FORCE_FLUSH_THRESHOLD))
			{
				writtenToFile = Flush();
			}
		}

		return writtenToFile;
	}

	//============================================================================

	bool CLogFile::Write(const char* format, ...)
	{
		bool writtenToFile = false;
		if (m_pBuffer != NULL)
		{
			CAutoMutexLock(m_pBuffer->m_mutex);
			m_pBuffer->m_previousSize = m_pBuffer->m_size;

			if (!(m_behaviours & eBF_SuspendOutputHeader))
			{
				if (m_behaviours & eBF_DateStamp)
				{
					InsertDateStampUTF8();
				}

				if (m_behaviours & eBF_LineCount)
				{
					InsertLineCountUTF8();
				}

				const ITimer* pGameClock = GetGameClock();
				if (m_behaviours & eBF_FrameCount)
				{
					InsertFrameCountUTF8(pGameClock);
				}

				if (m_behaviours & eBF_TimeStamp)
				{
					InsertTimeStampUTF8(pGameClock);
				}

				if (m_behaviours & eBF_Name)
				{
					InsertNameUTF8();
				}

				if (m_pBuffer->m_previousSize != m_pBuffer->m_size)
				{
					if (strcat_s(&m_pBuffer->m_buffer[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, " ") == 0)
					{
						++m_pBuffer->m_size;
					}
				}
			}

			va_list arguments;
			va_start(arguments, format);
			m_pBuffer->m_size += sprintf_s(&m_pBuffer->m_buffer[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, format, arguments);

			if (!(m_behaviours & eBF_SuspendOutputFooter))
			{
				if (m_behaviours & eBF_ForceInsertNewline)
				{
					ForceInsertNewlineUTF8();
				}
			}

			if (m_behaviours & eBF_OutputToDebugger)
			{
				CDebug::OutputToDebugger(&m_pBuffer->m_buffer[m_pBuffer->m_previousSize]);
			}

			uint32 bufferUsedCapacity = (m_pBuffer->m_size << 4) / LOGFILE_BUFFER_SIZE;
			if ((m_behaviours & eBF_FlushEachWrite) || (bufferUsedCapacity >= LOGS_FORCE_FLUSH_THRESHOLD))
			{
				writtenToFile = Flush();
			}
		}

		return writtenToFile;
	}

	//============================================================================

	void CLogFile::Initialise(void)
	{
		if (m_pBuffer == NULL)
		{
			if (m_pParent != NULL)
			{
				m_pBuffer = m_pParent->m_pBuffer;
			}
			else
			{
				m_behaviours |= eIBF_AllocatedBuffer | eIBF_SeparateFile;
				m_pBuffer = new SLogFileBuffer();
			}
		}
		else
		{
			m_behaviours |= eIBF_SeparateFile;
		}
	}

	//============================================================================

	IFileSystem::eFileSystemHandle CLogFile::Open(void)
	{
		IFileSystem::eFileSystemHandle handle = (m_pBuffer != NULL) ? m_pBuffer->m_handle : IFileSystem::eFSH_INVALID;;

		if (handle == IFileSystem::eFSH_INVALID)
		{
			if (m_pBuffer != NULL)
			{
				CAutoMutexLock(m_pBuffer->m_mutex);

				IFileSystem* pFileSystem = GetFileSystem();
				mchar_t<MAX_PATH> fileName;

				IFileSystem::eFileSystemError error = IFileSystem::eFSE_ERROR;
				if (m_behaviours & eIBF_Unicode)
				{
					error = pFileSystem->CreatePath(fileName.m_UTF16, MAX_PATH, m_name, IFileSystem::eFT_LogFile, true);
				}
				else
				{
					error = pFileSystem->CreatePath(fileName.m_UTF8, MAX_PATH, m_name.m_UTF16, IFileSystem::eFT_LogFile, true);
				}

				if (error == IFileSystem::eFSE_SUCCESS)
				{
					// N.B. opened as binary otherwise \n gets mangled...
					if (m_behaviours & eIBF_Unicode)
					{
						handle = pFileSystem->OpenFile(fileName.m_UTF16, _TEXT("wb")); 

						if (handle != IFileSystem::eFSH_INVALID)
						{
							// Insert the Byte-Order-Mark for UTF-16
							uint16 bom = 0xfeff;
							pFileSystem->Write(handle, &bom, sizeof(uint16), 1);
						}
					}
					else
					{
						handle = pFileSystem->OpenFile(fileName.m_UTF8, "wb");
					}

					m_pBuffer->m_handle = handle;
				}
			}
		}

		return handle;
	}

	//============================================================================

	void CLogFile::InsertNameUTF16(void)
	{
		m_pBuffer->m_size += swprintf_s(&m_pBuffer->m_buffer.m_UTF16[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, _TEXT("[%s]"), m_name.m_UTF16);
	}

	//============================================================================

	void CLogFile::InsertNameUTF8(void)
	{
		m_pBuffer->m_size += sprintf_s(&m_pBuffer->m_buffer.m_UTF8[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, "[%s]", m_name.m_UTF8);
	}

	//============================================================================

	void CLogFile::InsertDateStampUTF16(void)
	{
		const ISystemClock* pSystemClock = GetSystemClock();
		m_pBuffer->m_size += swprintf_s(&m_pBuffer->m_buffer.m_UTF16[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, _TEXT("[%s %s]"), pSystemClock->GetLocalDateString(), pSystemClock->GetLocalTimeString());
	}

	//============================================================================

	void CLogFile::InsertDateStampUTF8(void)
	{
		const ISystemClock* pSystemClock = GetSystemClock();
		m_pBuffer->m_size += sprintf_s(&m_pBuffer->m_buffer.m_UTF8[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, "[%s %s]", pSystemClock->GetLocalDateString(), pSystemClock->GetLocalTimeString());
	}

	//============================================================================

	void CLogFile::InsertLineCountUTF16(void)
	{
		m_pBuffer->m_size += swprintf_s(&m_pBuffer->m_buffer.m_UTF16[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, _TEXT("[%i]"), ++s_lineCount);
	}

	//============================================================================

	void CLogFile::InsertLineCountUTF8(void)
	{
		m_pBuffer->m_size += sprintf_s(&m_pBuffer->m_buffer.m_UTF8[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, "[%i]", ++s_lineCount);
	}

	//============================================================================

	void CLogFile::InsertFrameCountUTF16(const ITimer* pTimer)
	{
		m_pBuffer->m_size += swprintf_s(&m_pBuffer->m_buffer.m_UTF16[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, _TEXT("[%i]"), pTimer->GetFrameCount());
	}

	//============================================================================

	void CLogFile::InsertFrameCountUTF8(const ITimer* pTimer)
	{
		m_pBuffer->m_size += sprintf_s(&m_pBuffer->m_buffer.m_UTF8[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, "[%i]", pTimer->GetFrameCount());
	}

	//============================================================================

	void CLogFile::InsertTimeStampUTF16(const ITimer* pTimer)
	{
		m_pBuffer->m_size += swprintf_s(&m_pBuffer->m_buffer.m_UTF16[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, _TEXT("[%8.03f]"), pTimer->GetTime());
	}

	//============================================================================

	void CLogFile::InsertTimeStampUTF8(const ITimer* pTimer)
	{
		m_pBuffer->m_size += sprintf_s(&m_pBuffer->m_buffer.m_UTF8[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, "[%8.03f]", pTimer->GetTime());
	}

	//============================================================================

	void CLogFile::ForceInsertNewlineUTF16(void)
	{
		wchar_t* pNewline = _TEXT("\r\n");
		uint32 sizeof_newline = wcslen(pNewline);

		if (wcscmp(&m_pBuffer->m_buffer.m_UTF16[m_pBuffer->m_size - sizeof_newline], pNewline))
		{
			if (wcscat_s(&m_pBuffer->m_buffer.m_UTF16[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, pNewline) == 0)
			{
				m_pBuffer->m_size += sizeof_newline;
			}
		}
	}

	//============================================================================

	void CLogFile::ForceInsertNewlineUTF8()
	{
		char* pNewline = "\r\n";
		uint32 sizeof_newline = strlen(pNewline);

		if (strcmp(&m_pBuffer->m_buffer.m_UTF8[m_pBuffer->m_size - sizeof_newline], pNewline))
		{
			if (strcat_s(&m_pBuffer->m_buffer.m_UTF8[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, pNewline) == 0)
			{
				m_pBuffer->m_size += sizeof_newline;
			}
		}
	}

	//============================================================================

	bool CLogFile::Flush(void)
	{
		bool writtenToFile = false;

		if (m_pBuffer != NULL)
		{
			if (m_pBuffer->m_size > 0)
			{
				CAutoMutexLock(m_pBuffer->m_mutex);

				if (m_pBuffer->m_handle == IFileSystem::eFSH_INVALID)
				{
					Open();
				}

				if (m_pBuffer->m_handle != IFileSystem::eFSH_INVALID)
				{
					if (m_behaviours & eIBF_Unicode)
					{
						writtenToFile = (GetFileSystem()->Write(m_pBuffer->m_handle, m_pBuffer->m_buffer.m_UTF16, m_pBuffer->m_size * sizeof(wchar_t), 1) == 1);
					}
					else
					{
						writtenToFile = (GetFileSystem()->Write(m_pBuffer->m_handle, m_pBuffer->m_buffer.m_UTF8, m_pBuffer->m_size, 1) == 1);
					}

					m_pBuffer->m_buffer.m_UTF16[0] = 0;
					m_pBuffer->m_size = 0;
				}
			}
		}

		return writtenToFile;
	}

	//============================================================================

	void CLogFile::Close(void)
	{
		if (m_pBuffer != NULL)
		{
			CAutoMutexLock(m_pBuffer->m_mutex);

			Flush();

			if (m_behaviours & eIBF_SeparateFile)
			{
				GetFileSystem()->CloseFile(m_pBuffer->m_handle);
			}

			if (m_behaviours & eIBF_AllocatedBuffer)
			{
				delete m_pBuffer;
				m_pBuffer = NULL;
			}
		}
	}

	//============================================================================
} // End [namespace engine]

//==============================================================================
// [EOF]
