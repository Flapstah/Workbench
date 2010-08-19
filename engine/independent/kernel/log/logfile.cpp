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

	CLogFile::CLogFile(const TCHAR* name, CLogFile* pParent)
		: m_pParent(pParent)
		, m_handle(IFileSystem::eFSH_INVALID)
		, m_channels(eCF_ALL)
		, m_behaviours(eBF_ALL)
	{
		if (m_pParent != NULL)
		{
			m_handle = pParent->m_handle;
		}

		_tcscpy_s(m_name, sizeof(m_name) / sizeof(TCHAR), name);
	}

	//============================================================================

	CLogFile::~CLogFile(void)
	{
		Write(eCF_ALL, _TEXT("[EOF]\r\n"));
		if (m_pParent == NULL)
		{
			GetFileSystem()->CloseFile(m_handle);
			m_handle = IFileSystem::eFSH_INVALID;
		}
	}

	//============================================================================

	void CLogFile::Write(eChannelFlag channel, const TCHAR* format, ...)
	{
		// Only process if active and the channel(s) requested are on
		if (IsActive() && ((m_channels & channel) == channel))
		{
			IFileSystem* pFileSystem = GetFileSystem();

			if (m_handle == IFileSystem::eFSH_INVALID)
			{
				TCHAR fileName[MAX_PATH];
				if (pFileSystem->CreatePath(fileName, sizeof(fileName) / sizeof(TCHAR), m_name, IFileSystem::eFT_LogFile, true) == IFileSystem::eFSE_SUCCESS)
				{
					m_handle = pFileSystem->OpenFile(fileName, _TEXT("w"));
					uint16 bom = 0xfeff;
					pFileSystem->Write(m_handle, &bom, sizeof(uint16), 1);
				}
			}

			int32 count = 0;

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

			if (_tcscmp(&m_buffer[count - 2], _TEXT("\r\n")))
			{
				if (_tcscat_s(&m_buffer[count], (sizeof(m_buffer) / sizeof(TCHAR)) - count, _TEXT("\r\n")) == 0)
				{
					count += _tcslen(_TEXT("\r\n"));
				}
			}

			if (m_handle != IFileSystem::eFSH_INVALID)
			{
				pFileSystem->Write(m_handle, m_buffer, count * sizeof(TCHAR), 1);
			}

			if (m_behaviours & eBF_OutputToDebugger)
			{
				CDebug::OutputToDebugger(m_buffer);
			}
		}
	}

	//============================================================================

} // End [namespace engine]

//==============================================================================
// [EOF]
