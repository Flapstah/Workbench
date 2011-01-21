#if !defined(__IAPPLICATION_H__)
#define __IAPPLICATION_H__

//==============================================================================

namespace engine
{
	//============================================================================
	// IEngine
	//============================================================================
	struct IEngine 
	{
		virtual void	SetFrameRate(uint32 framesPerSecond);
		virtual float	GetFrameRate(void) const;

		virtual bool	Quit(bool immediate);
	};

	//============================================================================
	// IApplication
	//============================================================================
	struct IApplication
	{
		virtual bool	Initialise(IEngine* pEngine, int32 argc, char* argv[]) = 0;
		virtual bool	StartUp(void) = 0;
		virtual bool	Update(double frameTime, uint32 frameCount);
		virtual bool	ShutDown(void)	= 0;
		virtual bool	Uninitialise(void) = 0;

		virtual bool	Pause(bool pause) = 0;
		virtual bool	IsPaused(void) const = 0;
	}; // End [struct IApplication]

	//----------------------------------------------------------------------------
	// Launch the supplied application with the passed command line parameters
	//----------------------------------------------------------------------------
	int32 LaunchApplication(IApplication* pApplication, int32 argc, char* argv[]);

	//============================================================================
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__IAPPLICATION_H__)]
// [EOF]