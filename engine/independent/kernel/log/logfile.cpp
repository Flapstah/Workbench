#include "stdafx.h"

#include "common/ilogfile.h"
#include "kernel/file/filename.h"

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
		Write(WIDEN("[EOF]\n"));
	}

	void CLogFile::Write(const TCHAR* format, ...)
	{
		va_list arguments;
		va_start(arguments, format);

		// TODO: Some sort of print here...
		//m_file.Print()
	}
}

//==============================================================================
// [EOF]
