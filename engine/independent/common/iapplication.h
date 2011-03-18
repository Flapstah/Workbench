#if !defined(__IAPPLICATION_H__)
#define __IAPPLICATION_H__

//==============================================================================

namespace engine
{
	//============================================================================
	// IApplication
	//============================================================================
	struct IApplication
	{
		virtual bool	Initialise(int32 argc, TCHAR* argv[]) = 0;
		virtual bool	StartUp(void) = 0;
		virtual bool	Update(void) = 0;
		virtual bool	ShutDown(void)	= 0;
		virtual bool	Uninitialise(void) = 0;
		virtual bool	Quit(void) = 0;

		virtual bool	Pause(bool pause) = 0;
		virtual bool	IsPaused(void) const = 0;

		virtual void	SetFrameRate(uint32 framesPerSecond) = 0;
		virtual float	GetFrameRate(bool smoothed) const = 0;
	}; // End [struct IApplication]

	//============================================================================
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__IAPPLICATION_H__)]
// [EOF]