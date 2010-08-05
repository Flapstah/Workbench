#include "stdafx.h"

#include "common/ilogfile.h"
#include "kernel/file/filename.h"

#include "common/itime.h"
#include "kernel/debug/debug.h"

//==============================================================================

namespace engine
{
	CLogFile::CLogFile(TCHAR* name, CLogFile* pParent)
		: m_pParent(pParent)
		, m_channels(eCF_ALL)
		, m_behaviours(eBF_ALL)
	{
		_tcscpy_s(m_name, sizeof(m_name) / sizeof(TCHAR), name);
	}

	CLogFile::~CLogFile(void)
	{
		Write(eCF_ALL, WIDEN("[EOF]\n"));
	}

	void CLogFile::Write(eChannelFlag channel, const TCHAR* format, ...)
	{
		// Only process if active and the channel(s) requested are on
		if (IsActive() && ((m_channels & channel) == channel))
		{
			if (!m_file.IsOpen())
			{
				TCHAR fileName[MAX_PATH];
				if (CFileName::Create(fileName, sizeof(fileName) / sizeof(TCHAR), m_name, CFileName::eFT_LogFile))
				{
					m_file.Open(fileName, WIDEN("w"));
				}
			}

			int32 count = 0;

			if (m_behaviours & eBF_TimeStamp)
			{
				const ITimer* pGameClock = GetGameClock();
				count += _stprintf_s(&m_buffer[count], sizeof(m_buffer) - count, WIDEN("[%i][%8.03f] "), pGameClock->GetFrameCount(), pGameClock->GetTime());
			}

			if (m_behaviours & eBF_Name)
			{
				count += _stprintf_s(&m_buffer[count], sizeof(m_buffer) - count, WIDEN("[%s] "), m_name);
			}

			va_list arguments;
			va_start(arguments, format);

			count += _stprintf_s(&m_buffer[count], sizeof(m_buffer) - count, format, arguments);
			if (m_file.IsOpen())
			{
				m_file.Write(m_buffer, count, 1);
			}

			if (m_behaviours & eBF_OutputToDebugger)
			{
				CDebug::OutputToDebugger(m_buffer);
			}
		}
	}
}

//==============================================================================
// [EOF]
