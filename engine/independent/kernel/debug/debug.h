#if !defined(__DEBUG_H__)
#define __DEBUG_H__

//==============================================================================

namespace engine
{
	//============================================================================
	// CDebug
	//============================================================================
	class CDebug
	{
	public:
		static void OutputToDebugger(const wchar_t* output);
		static void OutputToDebugger(const char* output);

	protected:
		CDebug() {}

	private:
	//============================================================================};
	}; // End [class CDebug]
	//============================================================================
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__DEBUG_H__)]
// [EOF]
