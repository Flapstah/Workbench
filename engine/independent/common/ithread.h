#if !defined(__ITHREAD_H__)
#define __ITHREAD_H__

//==============================================================================

namespace engine
{
	//============================================================================
	// CMutex - don't use this templated class directly, use 'mutex' type instead
	//============================================================================
	template<typename _mutex>
	class CMutex
	{
	public:
		CMutex(void)												{	Platform_CreateMutex();									}
		~CMutex(void)												{	Platform_DestroyMutex();								}

		void Lock(void)											{ Platform_WaitForMutex(0);								}
		void Unlock(void)										{	Platform_ReleaseMutex();								}
		bool TryLock(uint32 timeout)				{ return Platform_WaitForMutex(timeout);	}

	protected:
		void Platform_CreateMutex(void);
		void Platform_DestroyMutex(void);

		bool Platform_WaitForMutex(uint32 timeout);
		void Platform_ReleaseMutex(void);

	protected:
		_mutex m_mutex;

	private:
	}; // End [class CMutex]

	//============================================================================

#if defined(WIN32) || defined(WIN64)
	typedef CMutex<HANDLE> mutex;
#elif defined(LINUX)
  // TODO: Need to create platform_thread files for linux
	typedef CMutex<HANDLE> mutex;
#endif

	//============================================================================

	class CAutoMutexLock
	{
	public:
		CAutoMutexLock(mutex& autoMutex) : m_autoMutex(autoMutex)	{	m_autoMutex.Lock();		}
		~CAutoMutexLock(void)																			{	m_autoMutex.Unlock();	}

	private:
		PREVENT_CLASS_COPY(CAutoMutexLock);

	private:
		mutex& m_autoMutex;
	};

	//============================================================================
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__ITHREAD_H__)]
// [EOF]
