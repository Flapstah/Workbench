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
		CApplication(uint32 desiredFPS);
		virtual ~CApplication(void);

		// IApplication
		virtual void SetFrameRate(uint32 framesPerSecond)	{ m_desiredFPS = framesPerSecond;															}
		virtual float GetFrameRate(void) const						{ return m_elapsedTime / (float)APPLICATION_FPS_BUFFER_SIZE;	}

		virtual bool Update(void);

		virtual bool Pause(bool pause);
		virtual bool Quit(bool immediate);
		// ~IApplication

	protected:
		virtual bool Initialise(void);
		virtual bool StartUp(void);
		virtual bool Update(double frameTime, uint32 frameCount);
		virtual bool ShutDown(void);
		virtual bool Uninitialise(void);

	protected:
		enum eFlags
		{
			eF_Quit						= 1 << 0,
			eF_QuitImmediate	= 1 << 1,
			eF_Initialised		= 1 << 2,
			eF_StartedUp			= 1 << 3,
			eF_Running				= 1 << 4
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