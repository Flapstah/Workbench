#if !defined(__DEBUG_H__)
#define __DEBUG_H__

//==============================================================================

namespace engine
{
	class CDebug
	{
	public:
		static void OutputToDebugger(const TCHAR* output);

	protected:
		CDebug() {}

	private:
	};
}

//==============================================================================
#endif // End [!defined(__DEBUG_H__)]
// [EOF]
