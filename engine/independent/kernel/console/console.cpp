#include "stdafx.h"

#include "kernel/console/console.h"

//==============================================================================

namespace engine
{
	CConsole::CVariant(const char* name, uint32 flags, const char* help, OnChangedCallback pOnChangedCallback)
		: m_pOnChangedCallback(pOnChangedCallback)
		, m_name(name)
		, m_help(help)
		, m_flags(flags)
	{
	}

	CConsole::~CVariant(void)
	{
	}

	const char* CConsole::CVariant::Name(void)
	{
		return m_name;
	}

	const char* CConsole::CVariant::Help(void)
	{
		return m_help;
	}

	uint32 CConsole::CVariant::GetFlags(void)
	{
		return m_flags;
	}

	void CConsole::CVariant::SetFlags(uint32 flags, uint32 mask)
	{
		m_flags = (m_flags & ~mask) | (flags & mask);
	}

	CConsole::CVariantInt32(const char* name, int32& variable, int32 initial, uint32 flags, const char* help, OnChangedCallback pOnChangedCallback)
		: CVariant(name, flags, help, pOnChangedCallback)
		, m_variable(variable)
	{
		m_variable = initial;
	}

	int32 CConsole::CVariantInt32::GetAsInt32(void)
	{
		return m_variable;
	}

	float CConsole::CVariantInt32::GetAsFloat(void)
	{
		return static_cast<float>(m_variable);
	}

	const char* CConsole::CVariantInt32::GetAsString(void)
	{
		// TODO:
		return NULL;
	}

	CConsole::CVariantFloat(const char* name, float& variable, float initial, uint32 flags, const char* help, OnChangedCallback pOnChangedCallback)
		: CVariant(name, flags, help, pOnChangedCallback)
		, m_variable(variable)
	{
		m_variable = initial;
	}

	int32 CConsole::CVariantFloat::GetAsInt32(void)
	{
		return static_cast<int32>(m_variable);
	}

	float CConsole::CVariantFloat::GetAsFloat(void)
	{
		return m_variable;
	}

	const char* CConsole::CVariantFloat::GetAsString(void)
	{
		// TODO:
		return NULL;
	}

	CConsole::CVariantString(const char* name, char*& variable, const char* initial, uint32 flags, const char* help, OnChangedCallback pOnChangedCallback)
		: CVariant(name, flags, help, pOnChangedCallback)
		, m_variable(variable)
	{
		m_variable = initial;
	}

	int32 CConsole::CVariantString::GetAsInt32(void)
	{
		// TODO:
		return 0;
	}

	float CConsole::CVariantString::GetAsFloat(void)
	{
		// TODO:
		return 0.0f;
	}

	const char* CConsole::CVariantString::GetAsString(void)
	{
		return m_variable;
	}
}
//==============================================================================
// [EOF]
