#if !defined(__CONSOLEAPPLICATION_H__)
#define __CONSOLEAPPLICATION_H__

//==============================================================================

#include "application/application.h"

//==============================================================================

namespace engine
{
	//============================================================================
	// CConsoleApplication
	//============================================================================
	class CConsoleApplication : public CApplication
	{
		typedef CApplication PARENT;

	public:
		CConsoleApplication(void);
		virtual ~CConsoleApplication(void);

		virtual bool Update(void) { return PARENT::Update(); }

	protected:
		virtual bool	Update(double frameTime);

	private:
	};

} // End [namespace engine]

//==============================================================================
#endif // End [!defined(__CONSOLEAPPLICATION_H__)]
// [EOF]
