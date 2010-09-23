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
		virtual void SetFrameRate(uint32 framesPerSecond) = 0;
		virtual float GetFrameRate(void) const = 0;

		virtual bool Pause(bool pause) = 0;
		virtual bool Quit(bool immediate) = 0;
	}; // End [struct IApplication]

	//============================================================================
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__IAPPLICATION_H__)]
// [EOF]