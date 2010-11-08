#include "stdafx.h"

#include "common/ilogfile.h"

#include "kernel/console/console.h"

//==============================================================================

namespace engine
{
	//============================================================================

	const char* CConsole::CVariantInt32::GetAsString(void)
	{
		char buffer[64];
		sprintf_s(buffer, sizeof(buffer), "%i", m_variable);
		return buffer;
	}

	//============================================================================

	const char* CConsole::CVariantFloat::GetAsString(void)
	{
		char buffer[64];
		sprintf_s(buffer, sizeof(buffer), "%f", m_variable);
		return buffer;
	}

	//============================================================================

	IConsole::IVariant* CConsole::RegisterVariable(const char* name, int32& variable, int32 initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback)
	{
		CVariantInt32* pVariant = NULL;
		CToken* pToken = FindToken(name);

		if (pToken == NULL)
		{
			pVariant = new CVariantInt32(name, variable, initial, flags | ICF_CREATED_ON_HEAP, help, pOnChangedCallback);
			if (pVariant != NULL)
			{
				m_tokenList.Add(pVariant);
			}

			Log(_TEXT("[Console]: '%s' registered as CVariantInt32"), name);
		}
		else
		{
			LogError(_TEXT("[Console]: '%s' is already registered"), name);
		}

		return pVariant;
	}

	//============================================================================

	IConsole::IVariant* CConsole::RegisterVariable(const char* name, float& variable, float initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback)
	{
		CVariantFloat* pVariant = NULL;
		CToken* pToken = FindToken(name);

		if (pToken == NULL)
		{
			pVariant = new CVariantFloat(name, variable, initial, flags | ICF_CREATED_ON_HEAP, help, pOnChangedCallback);
			if (pVariant != NULL)
			{
				m_tokenList.Add(pVariant);
			}

			Log(_TEXT("[Console]: '%s' registered as CVariantFloat"), name);
		}
		else
		{
			LogError(_TEXT("[Console]: '%s' is already registered"), name);
		}

		return pVariant;
	}

	//============================================================================

	IConsole::IVariant* CConsole::RegisterVariable(const char* name, char*& variable, const char* initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback)
	{
		CVariantString* pVariant = NULL;
		CToken* pToken = FindToken(name);

		if (pToken == NULL)
		{
			pVariant = new CVariantString(name, variable, initial, flags | ICF_CREATED_ON_HEAP, help, pOnChangedCallback);
			if (pVariant != NULL)
			{
				m_tokenList.Add(pVariant);
			}

			Log(_TEXT("[Console]: '%s' registered as CVariantString"), name);
		}
		else
		{
			LogError(_TEXT("[Console]: '%s' is already registered"), name);
		}

		return pVariant;
	}

	//============================================================================

	bool CConsole::UnregisterVariable(const char* name)
	{
		CToken* pToken = FindToken(name);
		bool unregistered = false;

		if (pToken != NULL)
		{
			if (pToken->GetFlags() & ICF_VARIANT)
			{
				m_tokenList.Remove(*pToken);
				if (pToken->GetFlags() & ICF_CREATED_ON_HEAP)
				{
					delete pToken;
				}

				unregistered = true;
				Log(_TEXT("[Console]: '%s' unregistered"), name);
			}
			else
			{
				LogError(_TEXT("[Console]: '%s' not registered as a variable"), name);
			}
		}
		else
		{
			LogError(_TEXT("[Console]: '%s' not registered"), name);
		}

		return unregistered;
	}

	//============================================================================

	bool CConsole::RegisterCommand(const char* name, uint32 flags, const char* help, IConsole::OnCommandCallback pOnCommandCallback)
	{
		CCommand* pCommand = NULL;
		CToken* pToken = FindToken(name);
		bool registered = false;

		if (pToken == NULL)
		{
			pCommand = new CCommand(name, flags | ICF_CREATED_ON_HEAP, help, pOnCommandCallback);
			if (pCommand != NULL)
			{
				m_tokenList.Add(pCommand);
			}

			registered = true;
			Log(_TEXT("[Console]: '%s' registered as CCommand"), name);
		}
		else
		{
			LogError(_TEXT("[Console]: '%s' is already registered"), name);
		}

		return registered;
	}

	//============================================================================

	bool CConsole::UnregisterCommand(const char* name)
	{
		CToken* pToken = FindToken(name);
		bool unregistered = false;

		if (pToken != NULL)
		{
			if (!(pToken->GetFlags() & ICF_VARIANT))
			{
				m_tokenList.Remove(*pToken);
				if (pToken->GetFlags() & ICF_CREATED_ON_HEAP)
				{
					delete pToken;
				}

				unregistered = true;
				Log(_TEXT("[Console]: '%s' unregistered"), name);
			}
			else
			{
				LogError(_TEXT("[Console]: '%s' not registered as a command"), name);
			}
		}
		else
		{
			LogError(_TEXT("[Console]: '%s' not registered"), name);
		}

		return unregistered;
	}

	//============================================================================

	IConsole::IVariant* CConsole::Find(const char* name)
	{
		IConsole::IVariant* pVariant = NULL;
		CToken* pToken = FindToken(name);
		if (pToken->GetFlags() & ICF_VARIANT)
		{
			pVariant = pToken;
		}

		return pVariant;
	}

	//============================================================================

	CToken* CConsole::FindToken(const char* name)
	{
		CToken* pToken = NULL;
		for (token_list_iterator iterator(m_tokenList); (pToken == NULL) && (iterator != m_tokenList.Tail()); ++iterator)
		{
			if (strcmp(iterator.m_name, name) == 0)
			{
				pToken = iterator;
			}
		}

		return pToken;
	}

	//============================================================================

	//----------------------------------------------------------------------------
	// The global instance of the console
	//----------------------------------------------------------------------------
	CConsole g_console;

	//============================================================================

	IConsole* GetConsole(void)
	{
		return &g_console;
	}

	//============================================================================
} // End [namespace engine]

//==============================================================================
// [EOF]
