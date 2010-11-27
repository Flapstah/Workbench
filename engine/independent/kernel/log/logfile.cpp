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
	static CLogFile gs_MainLog("Main", NULL, static_cast<ILogFile::eBehaviourFlag>(DEFAULT_LOG_BEHAVIOUR), &gs_MainLogBuffer);
	static CLogFile gs_ErrorLog("Error", &gs_MainLog, static_cast<ILogFile::eBehaviourFlag>(DEFAULT_LOG_BEHAVIOUR), NULL);
	static CLogFile gs_WarningLog("Warning", &gs_MainLog, static_cast<ILogFile::eBehaviourFlag>(DEFAULT_LOG_BEHAVIOUR), NULL);
	static CLogFile gs_AssertLog("Assert", &gs_MainLog, static_cast<ILogFile::eBehaviourFlag>(DEFAULT_LOG_BEHAVIOUR), NULL);
	static CLogFile gs_ToDoLog("ToDo", &gs_MainLog, static_cast<ILogFile::eBehaviourFlag>(DEFAULT_LOG_BEHAVIOUR), NULL);
	static CLogFile gs_PerformanceLog("Performance", &gs_MainLog, static_cast<ILogFile::eBehaviourFlag>(DEFAULT_LOG_BEHAVIOUR), NULL);

	//============================================================================

	ILogFile* g_MainLog = &gs_MainLog;
	ILogFile* g_ErrorLog = &gs_ErrorLog;
	ILogFile* g_WarningLog = &gs_WarningLog;
	ILogFile* g_AssertLog = &gs_AssertLog;
	ILogFile* g_ToDoLog = &gs_ToDoLog;
	ILogFile* g_PerformanceLog = &gs_PerformanceLog;

	//============================================================================

	CLogFile::CLogFile(const char* name, CLogFile* pParent, eBehaviourFlag initialBehaviour, SLogFileBuffer* pBuffer)
		: m_pParent(pParent)
		, m_pBuffer(pBuffer)
		, m_behaviours(initialBehaviour)
	{
		Initialise();

		strcpy_s(m_name, LOGFILE_NAME_SIZE, name);
	}

	//============================================================================

	CLogFile::~CLogFile(void)
	{
		Close();
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
					InsertDateStamp();
				}

				if (m_behaviours & eBF_LineCount)
				{
					InsertLineCount();
				}

				const ITimer* pGameClock = GetGameClock();
				if (m_behaviours & eBF_FrameCount)
				{
					InsertFrameCount(pGameClock);
				}

				if (m_behaviours & eBF_TimeStamp)
				{
					InsertTimeStamp(pGameClock);
				}

				if (m_behaviours & eBF_Name)
				{
					InsertName();
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
			m_pBuffer->m_size += vsprintf_s(&m_pBuffer->m_buffer[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, format, arguments);

			if (!(m_behaviours & eBF_SuspendOutputFooter))
			{
				if (m_behaviours & eBF_ForceInsertNewline)
				{
					ForceInsertNewline();
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
				char fileName[MAX_PATH];

				IFileSystem::eFileSystemError error = IFileSystem::eFSE_ERROR;
				error = pFileSystem->CreatePath(fileName, MAX_PATH, m_name, IFileSystem::eFT_LogFile, true);

				if (error == IFileSystem::eFSE_SUCCESS)
				{
					// N.B. opened as binary otherwise \n gets mangled...
					m_pBuffer->m_handle = handle = pFileSystem->OpenFile(fileName, "wb");
				}
			}
		}

		return handle;
	}

	//============================================================================

	void CLogFile::InsertName(void)
	{
		m_pBuffer->m_size += sprintf_s(&m_pBuffer->m_buffer[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, "[%s]", m_name);
	}

	//============================================================================

	void CLogFile::InsertDateStamp(void)
	{
		const ISystemClock* pSystemClock = GetSystemClock();
		m_pBuffer->m_size += sprintf_s(&m_pBuffer->m_buffer[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, "[%s %s]", pSystemClock->GetLocalDateString(), pSystemClock->GetLocalTimeString());
	}

	//============================================================================

	void CLogFile::InsertLineCount(void)
	{
		m_pBuffer->m_size += sprintf_s(&m_pBuffer->m_buffer[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, "[%i]", ++s_lineCount);
	}

	//============================================================================

	void CLogFile::InsertFrameCount(const ITimer* pTimer)
	{
		m_pBuffer->m_size += sprintf_s(&m_pBuffer->m_buffer[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, "[%i]", pTimer->GetFrameCount());
	}

	//============================================================================

	void CLogFile::InsertTimeStamp(const ITimer* pTimer)
	{
		m_pBuffer->m_size += sprintf_s(&m_pBuffer->m_buffer[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, "[%8.03f]", pTimer->GetTime());
	}

	//============================================================================

	void CLogFile::ForceInsertNewline()
	{
		char* pNewline = "\r\n";
		uint32 sizeof_newline = strlen(pNewline);

		if (strcmp(&m_pBuffer->m_buffer[m_pBuffer->m_size - sizeof_newline], pNewline))
		{
			if (strcat_s(&m_pBuffer->m_buffer[m_pBuffer->m_size], LOGFILE_BUFFER_SIZE - m_pBuffer->m_size, pNewline) == 0)
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
					writtenToFile = (GetFileSystem()->Write(m_pBuffer->m_handle, m_pBuffer->m_buffer, m_pBuffer->m_size, 1) == 1);

					m_pBuffer->m_buffer[0] = 0;
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
