#include "stdafx.h"

#include "ifile.h"

//==============================================================================

namespace engine
{
	CFile::CFile(const TCHAR* name, const TCHAR* mode)
	{
		_tfopen_s();

	}

	CFile::~CFile(void)
	{

	}

	int32 CFile::Read(void* pBuffer, uint32 itemSize, uint32 itemCount)
	{

	}

	int32 CFile::Write(const void* pBuffer, uint32 itemSize, uint32 itemCount)
	{

	}

	int32 CFile::Print(const TCHAR* format, ...)
	{

	}
}

//==============================================================================
// [EOF]
