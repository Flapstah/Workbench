#include "stdafx.h"

#include "kernel/console/console.h"

//==============================================================================

namespace engine
{
	template<typename _type> CConsole::CVariable<_type>::CVariable(const char* name, _type& variable, const _type default, const _type minimum, const _type maximum, const char* help, OnChangedCallback pOnChangedCallback)
		: m_pCallback(pOnChangedCallback)
		, m_name(name)
		, m_help(help)
		, m_variable(variable)
		, m_minimum(minimum)
		, m_maximum(maximum)
	{
		if (m_variable < m_minimum)
		{
			m_variable = m_minimum;
		}

		if (m_variable > m_maximum)
		{
			m_variable = m_maximum;
		}
	}

	template<typename _type> CConsole::CVariable<_type>::~CVariable()
	{
	}

	template<typename _type> const _type CConsole::CVariable<_type>::Get(void)
	{
		return m_variable;
	}

	template<typename _type> void CConsole::CVariable<_type>::Set(const _type value)
	{
		m_variable = value;
	}
}
//==============================================================================
// [EOF]
