#if !defined(__CONSOLE_H__)
#define __CONSOLE_H__

//==============================================================================

#include "common/iconsole.h"

//==============================================================================

namespace engine
{
	class CConsole : public IConsole
	{
	public:
		template<typename _type> class CVariable : public IVariable<_type>
		{
		public:
			virtual const _type Get(void);
			virtual void Set(const _type);

			CVariable(const char* name, _type& variable, const _type default, const _type minimum, const _type maximum, const char* help, OnChangedCallback pOnChangedCallback);
			~CVariable();

		protected:
			OnChangedCallback m_pCallback;
			const char* m_name;
			const char* m_help;
			_type& m_variable;
			_type m_minimum;
			_type m_maximum;

		private:
		};

	protected:
	private:
	};

}

//==============================================================================

#endif // End [!defined(__CONSOLE_H__)]
// [EOF]
