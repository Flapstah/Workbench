#include "stdafx.h"

#include "common/ifile.h"

//==============================================================================

namespace engine
{
	//============================================================================

	CFile::CFile(const TCHAR* name, const TCHAR* mode)
	{
		if (_tfopen_s(&m_pFile, name, mode))
		{
			m_pFile = NULL;
		}
	}

	//============================================================================

	CFile::~CFile(void)
	{
		if (m_pFile != NULL)
		{
			fclose(m_pFile);
			m_pFile = NULL;
		}
	}

	//============================================================================

	size_t CFile::Read(void* pBuffer, size_t bufferSize, size_t itemSize, size_t itemCount)
	{
		size_t itemsRead = 0;
		if (m_pFile != NULL)
		{
			itemsRead = fread_s(pBuffer, bufferSize, itemSize, itemCount, m_pFile);
		}

		return itemsRead;
	}

	//============================================================================

	size_t CFile::Write(const void* pBuffer, size_t itemSize, size_t itemCount)
	{
		size_t itemsWritten = 0;
		if (m_pFile != NULL)
		{
			itemsWritten = fwrite(pBuffer, itemSize, itemCount, m_pFile);
		}

		return itemsWritten;
	}

	//============================================================================

	size_t CFile::Print(const TCHAR* format, ...)
	{
		va_list arguments;
		va_start(arguments, format);

		int32 charactersWritten = 0;
		if (m_pFile != NULL)
		{
			charactersWritten = _vftprintf_s(m_pFile, format, arguments);
		}

		return charactersWritten;
	}

	//============================================================================

} // End [namespace engine]

//==============================================================================
// [EOF]
