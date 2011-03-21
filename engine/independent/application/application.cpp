#include "stdafx.h"

#include "common/ilogfile.h"
#include "common/itime.h"

#include "application/application.h"

//==============================================================================

namespace engine
{
	//============================================================================

	CApplication::CApplication(void)
		: m_argv(NULL)
		, m_frameTimeAccumulator(0.0f)
		, m_frameTimeBufferIndex(0)
		, m_desiredFPS(0)
		, m_state(eS_Uninitialised)
		, m_argc(0)
	{
		memset(m_frameTimeBuffer, 0, sizeof(m_frameTimeBuffer));
		m_frameTimeAccumulator = m_frameTimeBuffer[1] = 0.1f;
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

		return true;
	}

	//============================================================================

	bool CApplication::StartUp(void)
	{
		m_state = eS_Running;

		return true;
	}

	//============================================================================

	bool CApplication::Update(void)
	{
		GetRealTimeClock()->Tick();
		ITimer* pGameClock = GetGameClock();
		uint32 waitTime = 0;

		if (pGameClock->Tick())
		{
			double frameTime = pGameClock->GetFrameTimePrecise();
			m_frameTimeAccumulator -= m_frameTimeBuffer[m_frameTimeBufferIndex];
			m_frameTimeBufferIndex = ++m_frameTimeBufferIndex & (APPLICATION_FPS_BUFFER_SIZE - 1);
			m_frameTimeBuffer[m_frameTimeBufferIndex] = frameTime;
			m_frameTimeAccumulator += m_frameTimeBuffer[m_frameTimeBufferIndex];

			if (m_desiredFPS > 0)
			{
				double averageFrameTime = 1.0f / static_cast<double>(m_desiredFPS);
				if (frameTime < averageFrameTime)
				{
					waitTime = static_cast<uint32>((averageFrameTime - frameTime) * 1000.0);
				}
			}
		}

		bool ret = Update(pGameClock->GetFrameTimePrecise(), pGameClock->GetFrameCount());
		SLEEP(waitTime);
		return ret;
	}

	//============================================================================

	bool CApplication::ShutDown(void)
	{
		m_state = es_ShutDown;

		return true;
	}

	//============================================================================

	bool CApplication::Uninitialise(void)
	{
		m_argc = 0;
		m_argv = NULL;

		m_state = eS_Uninitialised;

		return true;
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

	float CApplication::GetFrameRate(bool smoothed) const
	{
		double frameTime = 0.01;

		if (smoothed)
		{
			frameTime = m_frameTimeAccumulator / static_cast<double>(APPLICATION_FPS_BUFFER_SIZE);
		}
		else
		{
			frameTime = GetGameClock()->GetFrameTimePrecise();
		}

		float fps = static_cast<float>(1.0 / frameTime);
		return fps;
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