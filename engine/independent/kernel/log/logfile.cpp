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

	CLogFile::CLogFile(const TCHAR* name, CLogFile* pParent, eBehaviourFlag initialBehaviour, SLogFileBuffer* pBuffer)
		: m_pParent(pParent)
		, m_pBuffer(pBuffer)
		, m_behaviours(initialBehaviour)
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

		_tcscpy_s(m_name, sizeof(m_name) / sizeof(TCHAR), name);
	}

	//============================================================================

	CLogFile::~CLogFile(void)
	{
		Close();
	}

	//============================================================================

	bool CLogFile::Write(const TCHAR* format, ...)
	{
		bool writtenToFile = false;
		if (m_pBuffer != NULL)
		{
			CAutoMutexLock(m_pBuffer->m_mutex);
			m_pBuffer->m_previousSize = m_pBuffer->m_size;

			if (!(m_behaviours & eBF_SuspendOutputHeader))
			{
				WriteBufferOutputHeader();
			}

			va_list arguments;
			va_start(arguments, format);
			m_pBuffer->m_size += _vstprintf_s(&m_pBuffer->m_buffer[m_pBuffer->m_size], (sizeof(m_pBuffer->m_buffer) / sizeof(TCHAR)) - m_pBuffer->m_size, format, arguments);

			if (!(m_behaviours & eBF_SuspendOutputFooter))
			{
				WriteBufferOutputFooter();
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

	IFileSystem::eFileSystemHandle CLogFile::Open(void)
	{
		IFileSystem::eFileSystemHandle handle = (m_pBuffer != NULL) ? m_pBuffer->m_handle : IFileSystem::eFSH_INVALID;;

		if (handle == IFileSystem::eFSH_INVALID)
		{
			if (m_pBuffer != NULL)
			{
				CAutoMutexLock(m_pBuffer->m_mutex);

				IFileSystem* pFileSystem = GetFileSystem();
				TCHAR fileName[MAX_PATH];

				if (pFileSystem->CreatePath(fileName, sizeof(fileName) / sizeof(TCHAR), m_name, IFileSystem::eFT_LogFile, true) == IFileSystem::eFSE_SUCCESS)
				{
					if ((m_pBuffer->m_handle = handle = pFileSystem->OpenFile(fileName, _TEXT("wb"))) != IFileSystem::eFSH_INVALID) // N.B. opened as binary otherwise \n gets mangled in unicode output...
					{
#if defined(_UNICODE)
						// Insert the Byte-Order-Mark for UTF-16
						uint16 bom = 0xfeff;
						pFileSystem->Write(handle, &bom, sizeof(uint16), 1);
#endif
					}
				}
			}
		}

		return handle;
	}

	//============================================================================

	void CLogFile::WriteBufferOutputHeader(void)
	{
		if (m_behaviours & eBF_DateStamp)
		{
			const ISystemClock* pSystemClock = GetSystemClock();
			m_pBuffer->m_size += _stprintf_s(&m_pBuffer->m_buffer[m_pBuffer->m_size], (sizeof(m_pBuffer->m_buffer) / sizeof(TCHAR)) - m_pBuffer->m_size, _TEXT("[%s %s]"), pSystemClock->GetLocalDateString(), pSystemClock->GetLocalTimeString());
		}

		if (m_behaviours & eBF_LineCount)
		{
			m_pBuffer->m_size += _stprintf_s(&m_pBuffer->m_buffer[m_pBuffer->m_size], (sizeof(m_pBuffer->m_buffer) / sizeof(TCHAR)) - m_pBuffer->m_size, _TEXT("[%i]"), ++s_lineCount);
		}

		const ITimer* pGameClock = GetGameClock();
		if (m_behaviours & eBF_FrameCount)
		{
			m_pBuffer->m_size += _stprintf_s(&m_pBuffer->m_buffer[m_pBuffer->m_size], (sizeof(m_pBuffer->m_buffer) / sizeof(TCHAR)) - m_pBuffer->m_size, _TEXT("[%i]"), pGameClock->GetFrameCount());
		}

		if (m_behaviours & eBF_TimeStamp)
		{
			m_pBuffer->m_size += _stprintf_s(&m_pBuffer->m_buffer[m_pBuffer->m_size], (sizeof(m_pBuffer->m_buffer) / sizeof(TCHAR)) - m_pBuffer->m_size, _TEXT("[%8.03f]"), pGameClock->GetTime());
		}

		if (m_behaviours & eBF_Name)
		{
			m_pBuffer->m_size += _stprintf_s(&m_pBuffer->m_buffer[m_pBuffer->m_size], (sizeof(m_pBuffer->m_buffer) / sizeof(TCHAR)) - m_pBuffer->m_size, _TEXT("[%s]"), m_name);
		}

		if (m_pBuffer->m_previousSize != m_pBuffer->m_size)
		{
			if (_tcscat_s(&m_pBuffer->m_buffer[m_pBuffer->m_size], (sizeof(m_pBuffer->m_buffer) / sizeof(TCHAR)) - m_pBuffer->m_size, _TEXT(" ")) == 0)
			{
				++m_pBuffer->m_size;
			}
		}
	}

	//============================================================================

	void CLogFile::WriteBufferOutputFooter(void)
	{
		if ((m_behaviours & eBF_ForceInsertNewline) && _tcscmp(&m_pBuffer->m_buffer[m_pBuffer->m_size - _tcslen(_TEXT("\r\n"))], _TEXT("\r\n")))
		{
			if (_tcscat_s(&m_pBuffer->m_buffer[m_pBuffer->m_size], (sizeof(m_pBuffer->m_buffer) / sizeof(TCHAR)) - m_pBuffer->m_size, _TEXT("\r\n")) == 0)
			{
				m_pBuffer->m_size += _tcslen(_TEXT("\r\n"));
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
					writtenToFile = (GetFileSystem()->Write(m_pBuffer->m_handle, m_pBuffer->m_buffer, m_pBuffer->m_size * sizeof(TCHAR), 1) == 1);
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
