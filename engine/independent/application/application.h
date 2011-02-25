#if !defined(__APPLICATION_H__)
#define __APPLICATION_H__

//==============================================================================

#include "common/iapplication.h"

//==============================================================================

//------------------------------------------------------------------------------
// N.B. Ensure this is a power of 2
//------------------------------------------------------------------------------
#define APPLICATION_FPS_BUFFER_SIZE (1 << 4)

//==============================================================================

namespace engine
{
	//============================================================================
	// CApplication
	//============================================================================
	class CApplication : public IApplication
	{
	public:
		CApplication(void);
		virtual ~CApplication(void);

		// IApplication
		virtual bool	Initialise(int32 argc, char* argv[]);
		virtual bool	StartUp(void);
		virtual bool	Update(void);
		virtual bool	ShutDown(void);
		virtual bool	Uninitialise(void);
		virtual bool	Quit(bool immediate);

		virtual bool	Pause(bool pause);
		virtual bool	IsPaused(void) const									{ return (m_state == eS_Paused);																					}

		virtual void	SetFrameRate(uint32 framesPerSecond)	{ m_desiredFPS = framesPerSecond;																					}
		virtual float	GetFrameRate(void) const							{ return m_elapsedTime / static_cast<float>(APPLICATION_FPS_BUFFER_SIZE);	}
		// ~IApplication

	protected:
		virtual bool	Update(double frameTime, uint32 frameCount);

	protected:
		enum eState
		{
			eS_Uninitialised,
			eS_Initialising,
			eS_Initialised,
			eS_StartingUp,
			eS_StartedUp,
			eS_Running,
			eS_Paused,
			eS_ShuttingDown,
			es_ShutDown,
			eS_Uninitialising,

			eS_Quitting,
			eS_QuittingImmediately
		};

		float		m_fpsBuffer[APPLICATION_FPS_BUFFER_SIZE];
		float		m_elapsedTime;
		uint32	m_fpsBufferIndex;
		uint32	m_desiredFPS;
		uint32	m_state;

	private:
	}; // End [class CApplication : public IApplication]

	//============================================================================
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__APPLICATION_H__)]
// [EOF]