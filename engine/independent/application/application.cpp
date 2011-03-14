#include "stdafx.h"

#include "common/ilogfile.h"
#include "common/itime.h"

#include "application/application.h"

//==============================================================================

namespace engine
{
	//============================================================================

	CApplication::CApplication(void)
		: m_elapsedTime(0.0f)
		, m_fpsBufferIndex(0)
		, m_desiredFPS(0)
		, m_state(eS_Uninitialised)
	{
		memset(m_fpsBuffer, 0, sizeof(m_fpsBuffer));
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
		IRealTimeClock* pRealTimeClock = GetRealTimeClock();
		pRealTimeClock->Tick();

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
		bool canPause = ((m_state == eS_Running) || (m_state == eS_Paused));

		if (canPause)
		{
			GetGameClock()->Pause(pause);
			Log("%sausing application", (pause) ? "P" : "Unp");
		}

		return canPause;
	}

	//============================================================================

	bool CApplication::Quit(void)
	{
		bool canQuit = ((m_state == eS_Running) || (m_state == eS_Paused));

		if (canQuit)
		{
			ShutDown();
			Uninitialise();
		}

		return canQuit;
	}

	//============================================================================

	bool CApplication::Update(double frameTime, uint32 frameCount)
	{
		//TODO: Here's where we need to update all the kernel bits
		return true;
	}

	//============================================================================
} // End [namespace engine]

//==============================================================================
// [EOF]