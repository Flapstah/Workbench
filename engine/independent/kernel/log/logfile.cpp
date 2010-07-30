#include "stdafx.h"

#include "common/ilogfile.h"
#include "kernel/file/filename.h"

#include "common/itime.h"
#include "kernel/debug/debug.h"

//==============================================================================

namespace engine
{
	CLogFile::CLogFile(TCHAR* name, CLogFile* pParent)
		: m_file(CFileName::Create(name, CFileName::eFT_LogFile), WIDEN("w"))
		, m_pParent(pParent)
		, m_channels(eCF_ALL)
		, m_behaviours(eBF_ALL)
	{
		_tcscpy_s(m_name, sizeof(m_name), name);
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
			m_file.Write(m_buffer, count, 1);

			if (m_behaviours & eBF_OutputToDebugger)
			{
				CDebug::OutputToDebugger(m_buffer);
			}
		}
	}
}

//==============================================================================
// [EOF]
