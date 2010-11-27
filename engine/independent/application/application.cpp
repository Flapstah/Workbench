#include "stdafx.h"

#include "common/ilogfile.h"
#include "common/itime.h"

#include "application/application.h"

//==============================================================================

namespace engine
{
	//============================================================================

	CApplication::CApplication(uint32 desiredFPS)
		: m_elapsedTime(0.1f)
		, m_fpsBufferIndex(0)
		, m_desiredFPS(desiredFPS)
		, m_flags(0)
	{
		memset(m_fpsBuffer, 0, sizeof(m_fpsBuffer));
		m_fpsBuffer[m_fpsBufferIndex] = m_elapsedTime;
	}

	//============================================================================

	CApplication::~CApplication(void)
	{
		ShutDown();
		Uninitialise();
	}

	//============================================================================

	bool CApplication::Update(void)
	{
		ISystemClock* pSystemClock = GetSystemClock();
		pSystemClock->Tick();

		ITimer* pGameClock = GetGameClock();
		pGameClock->Tick();

		m_elapsedTime -= m_fpsBuffer[m_fpsBufferIndex];
		m_fpsBuffer[++m_fpsBufferIndex & (APPLICATION_FPS_BUFFER_SIZE - 1)] = pGameClock->GetFrameTime();
		m_elapsedTime += m_fpsBuffer[m_fpsBufferIndex];

		return Update(pGameClock->GetFrameTimePrecise(), pGameClock->GetFrameCount());
	}

	//============================================================================

	bool CApplication::Pause(bool pause)
	{
		bool canPause = ((m_flags & eF_Quit) != eF_Quit);

		if (canPause)
		{
			ITimer* pGameClock = GetGameClock();
			pGameClock->Pause(pause);
		}

		return canPause;
	}

	//============================================================================

	bool CApplication::Quit(bool immediate)
	{
		bool canQuit = ((m_flags & eF_Quit) != eF_Quit);

		if (canQuit)
		{
			m_flags |= eF_Quit;

			if (immediate)
			{
				m_flags |= eF_QuitImmediate;

				ShutDown();
				Uninitialise();
			}
		}

		return canQuit;
	}

	//============================================================================

	bool CApplication::Initialise(void)
	{
		Log("Initialising CApplication");
		return true;
	}

	//============================================================================

	bool CApplication::StartUp(void)
	{
		bool canStartUp = ((m_flags & eF_Initialised) == eF_Initialised);

		if (canStartUp)
		{
			Log("Starting up CApplication");
		}
		else
		{
			LogError("Unable to start up - CApplication has not been initialised");
		}

		return canStartUp;
	}

	//============================================================================

	bool CApplication::Update(double frameTime, uint32 frameCount)
	{
		//TODO: Here's where we need to update all the kernel bits
		return true;
	}

	//============================================================================

	bool CApplication::ShutDown(void)
	{
		bool canShutDown = ((m_flags & eF_StartedUp) == eF_StartedUp);

		return canShutDown;
	}

	//============================================================================

	bool CApplication::Uninitialise(void)
	{
		bool canUninitialise = ((m_flags & (eF_StartedUp || eF_Initialised)) == eF_Initialised);
		return canUninitialise;
	}

	//============================================================================
} // End [namespace engine]

//==============================================================================
// [EOF]