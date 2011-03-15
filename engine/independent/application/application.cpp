#include "stdafx.h"

#include "common/ilogfile.h"
#include "common/itime.h"

#include "application/application.h"

//==============================================================================

namespace engine
{
	//============================================================================

	CApplication::CApplication(void)
		: m_argv = NULL
		, m_frameTimeAccumulator(0.0f)
		, m_frameTimeBufferIndex(0)
		, m_desiredFPS(0)
		, m_state(eS_Uninitialised)
		, m_argc(0)
	{
		memset(m_frameTimeBuffer, 0, sizeof(m_frameTimeBuffer));
	}

	//============================================================================

	CApplication::~CApplication(void)
	{
		ShutDown();
		Uninitialise();
	}

	//============================================================================

	bool CApplication::Initialise(int32 argc, TCHAR* argv[])
	{
		m_argc = argc;
		m_argv = argv;

		m_state = eS_Initialised;
	}

	//============================================================================

	bool CApplication::StartUp(void)
	{
		m_state = eS_Running;
	}

	//============================================================================

	bool CApplication::Update(void)
	{
		IRealTimeClock* pRealTimeClock = GetRealTimeClock();
		pRealTimeClock->Tick();
		float frameTime = pRealTimeClock->GetFrameTime();

		m_frameTimeAccumulator -= m_frameTimeBuffer[m_frameTimeBufferIndex];
		m_frameTimeBuffer[++m_frameTimeBufferIndex & (APPLICATION_FPS_BUFFER_SIZE - 1)] = frameTime;
		m_frameTimeAccumulator += m_frameTimeBuffer[m_frameTimeBufferIndex];

		ITimer* pGameClock = GetGameClock();
		float averageFPS = GetFrameRate(true);
		float desiredFPS = static_cast<float>(m_desiredFPS);

		if (averageFPS <= desiredFPS)
		{
			if (!IsPaused())
			{
				pGameClock->Tick();
			}
		}
		else
		{
			float desiredFrameTime = 1.0f / desiredFPS;
			uint32 waitTime = static_cast<uint32>((desiredFrameTime - frameTime) / 1000.0f);
			SLEEP(waitTime);
		}

		return Update(pGameClock->GetFrameTimePrecise(), pGameClock->GetFrameCount());
	}

	//============================================================================

	bool CApplication::ShutDown(void)
	{
		m_state = es_ShutDown;
	}

	//============================================================================

	bool CApplication::Uninitialise(void)
	{
		m_argc = 0;
		m_argv = NULL;

		m_state = eS_Uninitialised;
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

	float CApplication::GetFrameRate(bool smoothed)
	{
		float frameTime = 0.01f;

		if (smoothed)
		{
			frameTime = m_frameTimeAccumulator / static_cast<float>(APPLICATION_FPS_BUFFER_SIZE);
		}
		else
		{
			frameTime = GetRealTimeClock()->GetFrameTime()
		}

		return 1.0f / frameTime;
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