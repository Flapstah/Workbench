#include "stdafx.h"

#include "common/ithread.h"

//==============================================================================

namespace engine
{
	//============================================================================

	//----------------------------------------------------------------------------
	// There needs to be an explicit declaration of this typed mutex in order that
	// the platform function definitions can be separated from the declarations in
	// the header.  Implicit instantiation would require the function definitions
	// to be with the declaration. 
	//----------------------------------------------------------------------------
	template class CMutex<HANDLE>;

	//============================================================================

	template<typename _mutex> void CMutex<_mutex>::Platform_CreateMutex(void)
	{
		m_mutex = CreateMutex(NULL, FALSE, NULL);
	}

	//============================================================================

	template<typename _mutex> void CMutex<_mutex>::Platform_DestroyMutex(void)
	{
		CloseHandle(m_mutex);
	}

	//============================================================================

	template<typename _mutex> bool CMutex<_mutex>::Platform_WaitForMutex(uint32 timeout)
	{
		return (WaitForSingleObject(m_mutex, timeout) != WAIT_TIMEOUT);
	}

	//============================================================================

	template<typename _mutex> void CMutex<_mutex>::Platform_ReleaseMutex(void)
	{
		ReleaseMutex(m_mutex);
	}

	//============================================================================
} // End [namespace engine]

//==============================================================================
// [EOF]
