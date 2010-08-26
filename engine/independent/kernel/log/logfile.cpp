#include "stdafx.h"

#include "common/itime.h"

#include "kernel/log/logfile.h"
#include "kernel/debug/debug.h"
#include "kernel/filesystem/filesystem.h"

//------------------------------------------------------------------------------
// Good unicode resource page: http://www.i18nguy.com/unicode/c-unicode.html
//------------------------------------------------------------------------------

//==============================================================================

namespace engine
{
	//============================================================================

	//----------------------------------------------------------------------------
	// The global instances of logs
	//----------------------------------------------------------------------------
	static CLogFile gs_MainLog(_TEXT("Main"), NULL, static_cast<ILogFile::eBehaviourFlag>(ILogFile::eBF_Active | ILogFile::eBF_Name | ILogFile::eBF_OutputToDebugger | ILogFile::eBF_FlushEachWrite));
	static CLogFile gs_ErrorLog(_TEXT("Error"), &gs_MainLog, static_cast<ILogFile::eBehaviourFlag>(ILogFile::eBF_Active | ILogFile::eBF_Name | ILogFile::eBF_OutputToDebugger | ILogFile::eBF_FlushEachWrite));
	static CLogFile gs_WarningLog(_TEXT("Warning"), &gs_MainLog, static_cast<ILogFile::eBehaviourFlag>(ILogFile::eBF_Active | ILogFile::eBF_Name | ILogFile::eBF_OutputToDebugger | ILogFile::eBF_FlushEachWrite));
	static CLogFile gs_AssertLog(_TEXT("Assert"), &gs_MainLog, static_cast<ILogFile::eBehaviourFlag>(ILogFile::eBF_Active | ILogFile::eBF_Name | ILogFile::eBF_OutputToDebugger | ILogFile::eBF_FlushEachWrite));
	static CLogFile gs_ToDoLog(_TEXT("ToDo"), &gs_MainLog, static_cast<ILogFile::eBehaviourFlag>(ILogFile::eBF_Active | ILogFile::eBF_Name | ILogFile::eBF_OutputToDebugger | ILogFile::eBF_FlushEachWrite));
	static CLogFile gs_PerformanceLog(_TEXT("Performance"), &gs_MainLog, static_cast<ILogFile::eBehaviourFlag>(ILogFile::eBF_Active | ILogFile::eBF_Name | ILogFile::eBF_OutputToDebugger | ILogFile::eBF_FlushEachWrite));

	//============================================================================

	ILogFile* g_MainLog = &gs_MainLog;
	ILogFile* g_ErrorLog = &gs_ErrorLog;
	ILogFile* g_WarningLog = &gs_WarningLog;
	ILogFile* g_AssertLog = &gs_AssertLog;
	ILogFile* g_ToDoLog = &gs_ToDoLog;
	ILogFile* g_PerformanceLog = &gs_PerformanceLog;

	//============================================================================

	CLogFile::CLogFile(const TCHAR* name, CLogFile* pParent, eBehaviourFlag initialBehaviour /* = eBF_ALL */)
#if defined(LOGS_FORCE_SEPARATE_FILES)
		: m_pParent(NULL)
#else
		: m_pParent(pParent)
#endif
		, m_handle(IFileSystem::eFSH_INVALID)
		, m_size(0)
		, m_previousSize(0)
		, m_behaviours(initialBehaviour)
	{
#if defined(LOGS_FORCE_SEPARATE_FILES)
		IGNORE_PARAMETER(pParent);
#endif

		_tcscpy_s(m_name, sizeof(m_name) / sizeof(TCHAR), name);

		if (m_behaviours & eBF_WriteStartAndEnd)
		{
			uint32 old = m_behaviours;
			m_behaviours = eBF_Active | eBF_Name | eBF_DateStamp;
			Write(_TEXT("Start log\r\n"));
			m_behaviours = old;
		}
	}

	//============================================================================

	CLogFile::~CLogFile(void)
	{
		if (m_behaviours & eBF_WriteStartAndEnd)
		{
			m_behaviours = eBF_Active | eBF_Name | eBF_DateStamp | eBF_FlushEachWrite;
			Write(_TEXT("End Log\r\n"));
		}

		Close();
	}

	//============================================================================

	bool CLogFile::Write(const TCHAR* format, ...)
	{
		m_previousSize = m_size;

		if (m_behaviours & eBF_DateStamp)
		{
			const ISystemClock* pSystemClock = GetSystemClock();
			m_size += _stprintf_s(&m_buffer[m_size], (sizeof(m_buffer) / sizeof(TCHAR)) - m_size, _TEXT("[%s %s] "), pSystemClock->GetLocalDateString(), pSystemClock->GetLocalTimeString());
		}

		if (m_behaviours & eBF_TimeStamp)
		{
			const ITimer* pGameClock = GetGameClock();
			m_size += _stprintf_s(&m_buffer[m_size], (sizeof(m_buffer) / sizeof(TCHAR)) - m_size, _TEXT("[%i][%8.03f] "), pGameClock->GetFrameCount(), pGameClock->GetTime());
		}

		if (m_behaviours & eBF_Name)
		{
			m_size += _stprintf_s(&m_buffer[m_size], (sizeof(m_buffer) / sizeof(TCHAR)) - m_size, _TEXT("[%s] "), m_name);
		}

		va_list arguments;
		va_start(arguments, format);
		m_size += _vstprintf_s(&m_buffer[m_size], (sizeof(m_buffer) / sizeof(TCHAR)) - m_size, format, arguments);

#if defined LOGS_FORCE_INSERT_NEWLINE
		if (_tcscmp(&m_buffer[m_size - _tcslen(_TEXT("\r\n"))], _TEXT("\r\n")))
		{
			if (_tcscat_s(&m_buffer[m_size], (sizeof(m_buffer) / sizeof(TCHAR)) - m_size, _TEXT("\r\n")) == 0)
			{
				m_size += _tcslen(_TEXT("\r\n"));
			}
		}
#endif

		if (m_behaviours & eBF_OutputToDebugger)
		{
			CDebug::OutputToDebugger(&m_buffer[m_previousSize]);
		}

		bool writtenToFile = false;
		uint32 bufferUsedCapacity = (m_size << 4) / LOGFILE_BUFFER_SIZE;
		if ((m_behaviours & eBF_FlushEachWrite) || (bufferUsedCapacity >= LOGS_FORCE_FLUSH_THRESHOLD))
		{
			writtenToFile = Flush();
		}

		return writtenToFile;
	}

	//============================================================================

	IFileSystem::eFileSystemHandle CLogFile::Open(void)
	{
		if (GetFileHandle() == IFileSystem::eFSH_INVALID)
		{
			if (m_pParent != NULL)
			{
				m_handle = m_pParent->Open();
			}
		}

		IFileSystem* pFileSystem = GetFileSystem();
		if (m_handle == IFileSystem::eFSH_INVALID)
		{
			TCHAR fileName[MAX_PATH];

			if (pFileSystem->CreatePath(fileName, sizeof(fileName) / sizeof(TCHAR), m_name, IFileSystem::eFT_LogFile, true) == IFileSystem::eFSE_SUCCESS)
			{
				if ((m_handle = pFileSystem->OpenFile(fileName, _TEXT("wb"))) != IFileSystem::eFSH_INVALID) // N.B. opened as binary otherwise \n gets mangled in unicode output...
				{
#if defined(_UNICODE)
					// Insert the Byte-Order-Mark for UTF-16
					uint16 bom = 0xfeff;
					pFileSystem->Write(m_handle, &bom, sizeof(uint16), 1);
#endif
				}
			}
		}
		else
		{
			pFileSystem->AddFileReference(m_handle);
		}

		return m_handle;
	}

	//============================================================================

	bool CLogFile::Flush(void)
	{
		bool writtenToFile = false;

		if (m_handle == IFileSystem::eFSH_INVALID)
		{
			Open();
		}

		if (m_handle != IFileSystem::eFSH_INVALID)
		{
			writtenToFile = (GetFileSystem()->Write(m_handle, m_buffer, m_size * sizeof(TCHAR), 1) == 1);
			m_buffer[0] = 0;
			m_size = 0;
		}

		return writtenToFile;
	}

	//============================================================================

	void CLogFile::Close(void)
	{
		if (m_handle != IFileSystem::eFSH_INVALID)
		{
			IFileSystem* pFileSystem = GetFileSystem();
			if (pFileSystem->GetFileReference(m_handle) == 1)
			{
				m_behaviours = eBF_Active | eBF_FlushEachWrite;
				Write(_TEXT("[EOF]\r\n"));
			}

			pFileSystem->CloseFile(m_handle);
		}
	}

	//============================================================================

} // End [namespace engine]

//==============================================================================
// [EOF]
