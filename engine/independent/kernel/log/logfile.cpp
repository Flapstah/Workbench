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
	// The global instance of the main, error and warning logs
	//----------------------------------------------------------------------------
	CLogFile g_MainLog(_TEXT("Main"), NULL, static_cast<ILogFile::eBehaviourFlag>(ILogFile::eBF_Active | ILogFile::eBF_Name | ILogFile::eBF_OutputToDebugger));
	CLogFile g_ErrorLog(_TEXT("Error"), &g_MainLog, static_cast<ILogFile::eBehaviourFlag>(ILogFile::eBF_Active | ILogFile::eBF_Name | ILogFile::eBF_OutputToDebugger));
	CLogFile g_WarningLog(_TEXT("Warning"), &g_MainLog, static_cast<ILogFile::eBehaviourFlag>(ILogFile::eBF_Active | ILogFile::eBF_Name | ILogFile::eBF_OutputToDebugger));

	//============================================================================

	ILogFile* GetMainLog(void)
	{
		return &g_MainLog;
	}

	//============================================================================

	ILogFile* GetErrorLog(void)
	{
		return &g_ErrorLog;
	}

	//============================================================================

	ILogFile* GetWarningLog(void)
	{
		return &g_WarningLog;
	}

	//============================================================================

	CLogFile::CLogFile(const TCHAR* name, CLogFile* pParent, eBehaviourFlag initialBehaviour /* = eBF_ALL */)
#if defined(LOGS_FORCE_SEPARATE_FILES)
		: m_pParent(NULL)
#else
		: m_pParent(pParent)
#endif
		, m_handle(IFileSystem::eFSH_INVALID)
		, m_behaviours(initialBehaviour)
		, m_referenceCount(0)
	{
#if !defined(LOGS_FORCE_SEPARATE_FILES)
		IGNORE_PARAMETER(pParent);
#endif

		_tcscpy_s(m_name, sizeof(m_name) / sizeof(TCHAR), name);
	}

	//============================================================================

	CLogFile::~CLogFile(void)
	{
		m_behaviours = eBF_Active | eBF_Name | eBF_DateStamp;
		Write(_TEXT("End Log\r\n"));

		if (m_pParent == NULL)
		{
			m_behaviours = eBF_Active;
			Write(_TEXT("[EOF]\r\n"));
			GetFileSystem()->CloseFile(m_handle);
		}
		else
		{
			--m_pParent->m_referenceCount;
		}
	}

	//============================================================================

	bool CLogFile::Write(const TCHAR* format, ...)
	{
		IFileSystem* pFileSystem = GetFileSystem();

		if (FileHandle() == IFileSystem::eFSH_INVALID)
		{
			TCHAR fileName[MAX_PATH];
			if (pFileSystem->CreatePath(fileName, sizeof(fileName) / sizeof(TCHAR), m_name, IFileSystem::eFT_LogFile, true) == IFileSystem::eFSE_SUCCESS)
			{
				FileHandle() = pFileSystem->OpenFile(fileName, _TEXT("wb")); // N.B. opened as binary otherwise \n gets mangled in unicode output...

#if defined(_UNICODE)
				// Insert the Byte-Order-Mark for UTF-16
				uint16 bom = 0xfeff;
				pFileSystem->Write(FileHandle(), &bom, sizeof(uint16), 1);
#endif

				uint32 old = m_behaviours;
				m_behaviours = eBF_Active | eBF_Name | eBF_DateStamp;
				Write(_TEXT("Start log\r\n"));
				m_behaviours = old;
			}
		}

		int32 count = 0;

		if (m_behaviours & eBF_DateStamp)
		{
			const ISystemClock* pSystemClock = GetSystemClock();
			count += _stprintf_s(&m_buffer[count], (sizeof(m_buffer) / sizeof(TCHAR)) - count, _TEXT("[%s %s] "), pSystemClock->GetLocalDateString(), pSystemClock->GetLocalTimeString());
		}

		if (m_behaviours & eBF_TimeStamp)
		{
			const ITimer* pGameClock = GetGameClock();
			count += _stprintf_s(&m_buffer[count], (sizeof(m_buffer) / sizeof(TCHAR)) - count, _TEXT("[%i][%8.03f] "), pGameClock->GetFrameCount(), pGameClock->GetTime());
		}

		if (m_behaviours & eBF_Name)
		{
			count += _stprintf_s(&m_buffer[count], (sizeof(m_buffer) / sizeof(TCHAR)) - count, _TEXT("[%s] "), m_name);
		}

		va_list arguments;
		va_start(arguments, format);
		count += _vstprintf_s(&m_buffer[count], (sizeof(m_buffer) / sizeof(TCHAR)) - count, format, arguments);

#if defined LOGS_FORCE_INSERT_NEWLINE
		if (_tcscmp(&m_buffer[count - _tcslen(_TEXT("\r\n"))], _TEXT("\r\n")))
		{
			if (_tcscat_s(&m_buffer[count], (sizeof(m_buffer) / sizeof(TCHAR)) - count, _TEXT("\r\n")) == 0)
			{
				count += _tcslen(_TEXT("\r\n"));
			}
		}
#endif

		bool writtenToFile = false;
		if (FileHandle() != IFileSystem::eFSH_INVALID)
		{
			writtenToFile = (pFileSystem->Write(FileHandle(), m_buffer, count * sizeof(TCHAR), 1) == 1);
		}

		if (m_behaviours & eBF_OutputToDebugger)
		{
			CDebug::OutputToDebugger(m_buffer);
		}

		return writtenToFile;
	}

	//============================================================================

} // End [namespace engine]

//==============================================================================
// [EOF]
