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
		virtual void	SetFrameRate(uint32 framesPerSecond)	{ m_desiredFPS = framesPerSecond;																						}
		virtual float	GetFrameRate(void) const							{ return m_elapsedTime / (float)APPLICATION_FPS_BUFFER_SIZE;								}

		virtual bool	Pause(bool pause);
		virtual bool	Quit(bool immediate);
		// ~IApplication

		virtual bool	Update(void);

	protected:
		virtual bool	Initialise(void) = 0;
		virtual bool	StartUp(void) = 0;
		virtual bool	Update(double frameTime, uint32 frameCount);
		virtual bool	ShutDown(void)	= 0;
		virtual bool	Uninitialise(void) = 0;

						bool	CanInitialise(void)										{	return ((m_flags & eF_Initialised) == 0);																	}
						bool	CanStartup(void)											{	return ((m_flags & eF_Initialised) == eF_Initialised);										}
						bool	CanShutDown(void)											{	return ((m_flags & eF_StartedUp) == eF_StartedUp);												}
						bool	CanUninitialise(void)									{	return ((m_flags & (eF_StartedUp || eF_Initialised)) == eF_Initialised);	}

	protected:
		enum eFlags
		{
			eF_InitialState		= 0,

			eF_Quit						= BIT(0),
			eF_QuitImmediate	= BIT(1),
			eF_Initialised		= BIT(2),
			eF_StartedUp			= BIT(3),
			eF_Running				= BIT(4)
		};

		float		m_fpsBuffer[APPLICATION_FPS_BUFFER_SIZE];
		float		m_elapsedTime;
		uint32	m_fpsBufferIndex;
		uint32	m_desiredFPS;
		uint32	m_flags;

	private:
	}; // End [class CApplication : public IApplication]

	//============================================================================
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__APPLICATION_H__)]
// [EOF]