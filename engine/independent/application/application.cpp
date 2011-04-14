#include "stdafx.h"

#include "common/ilogfile.h"
#include "common/itime.h"

#include "application/application.h"

//==============================================================================

#define MINIMUM_FRAME_TIME (0.000001)

//==============================================================================

namespace engine
{
	//============================================================================

	CApplication::CApplication(void)
		: m_argv(NULL)
		, m_frameTimeAccumulator(0.0f)
		, m_waitTimeAccumulator(0.0f)
		, m_frameTimeBufferIndex(APPLICATION_FPS_BUFFER_SIZE - 1)
		, m_desiredFPS(0)
		, m_state(eS_Uninitialised)
		, m_argc(0)
	{
		memset(m_frameTimeBuffer, 0, sizeof(m_frameTimeBuffer));
		//m_frameTimeAccumulator = m_frameTimeBuffer[0] = 0.1f;
	}

	//============================================================================

	CApplication::~CApplication(void)
	{
		ShutDown();
		Uninitialise();
	}

	//============================================================================

	bool CApplication::Initialise(int32 argc, char* argv[])
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
		IRealTimeClock* pRealTimeClock = GetRealTimeClock();
		ITimer* pGameClock = GetGameClock();
		double frameTime = 0.0;

		pRealTimeClock->Tick();
		if (pGameClock->Tick())
		{
			frameTime = pGameClock->GetFrameTimePrecise();
		}

		m_frameTimeBufferIndex = ++m_frameTimeBufferIndex & (APPLICATION_FPS_BUFFER_SIZE - 1);
		m_frameTimeAccumulator += (frameTime - m_frameTimeBuffer[m_frameTimeBufferIndex]);
		m_frameTimeBuffer[m_frameTimeBufferIndex] = frameTime;

		bool ret = Update(frameTime);

		uint32 waitTime = 0;
		double elapsedTime = pRealTimeClock->GetRealTimePrecise() - pRealTimeClock->GetTickTimePrecise();

		if (m_desiredFPS > 0)
		{
			double desiredFrameTime = 1.0 / static_cast<double>(m_desiredFPS);
			if (elapsedTime <= desiredFrameTime)
			{
				m_waitTimeAccumulator += (desiredFrameTime - elapsedTime);
				waitTime = static_cast<uint32>(m_waitTimeAccumulator * 1000.0);
				m_waitTimeAccumulator -= (0.001 * waitTime);
				LogDebug("elapsedTime %f, desiredFrameTime %f, slacktime %f, wait %ius (excess %f)", elapsedTime, desiredFrameTime, desiredFrameTime - elapsedTime, waitTime, m_waitTimeAccumulator);
			}
			else
			{
				Log("[CApplication::Update()] unable to sustain desired frame rate of %ifps (elapsedTime %fs, over budget by %fs)", m_desiredFPS, elapsedTime, elapsedTime - desiredFrameTime);
			}
		}

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
		double frameTime = MINIMUM_FRAME_TIME;

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

	bool CApplication::Update(double frameTime)
	{
		//TODO: Here's where we need to update all the kernel bits
		return true;
	}

	//============================================================================
} // End [namespace engine]

//==============================================================================
// [EOF]