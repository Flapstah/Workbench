#include "stdafx.h"

#include "common/ilogfile.h"

#include "kernel/console/console.h"

//==============================================================================

namespace engine
{
	//============================================================================

	void CConsole::CToken::Append(CToken& token)
	{
		token.Unlink();
		token.m_pPrevious = this;

		if (m_pNext != NULL)
		{
			m_pNext->m_pPrevious = &token;
			token.m_pNext = m_pNext;
		}

		m_pNext = &token;
	}

	//============================================================================

	void CConsole::CToken::Prepend(CToken& token)
	{
		token.Unlink();
		token.m_pNext = this;

		if (m_pPrevious != NULL)
		{
			m_pPrevious->m_pNext = &token;
			token.m_pPrevious = m_pPrevious;
		}

		m_pPrevious = &token;
	}

	//============================================================================

	void CConsole::CToken::Unlink(void)
	{
		if (m_pNext != NULL)
		{
			m_pNext->m_pPrevious = m_pPrevious;
		}

		if (m_pPrevious != NULL)
		{
			m_pPrevious->m_pNext = m_pNext;
		}
	}

	//============================================================================

	const char* CConsole::CVariantInt32::GetAsString(void)
	{
		static char buffer[64];
		sprintf_s(buffer, sizeof(buffer), "%i", m_variable);
		return buffer;
	}

	//============================================================================

	const char* CConsole::CVariantFloat::GetAsString(void)
	{
		static char buffer[64];
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
				Add(*pVariant);
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
				Add(*pVariant);
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
				Add(*pVariant);
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
				Remove(*pToken);
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
				Add(*pCommand);
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
				Remove(*pToken);
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
			pVariant = reinterpret_cast<IVariant*>(pToken);
		}

		return pVariant;
	}

	//============================================================================

	void CConsole::Add(CToken& token)
	{
		if (m_pHead != NULL)
		{
			CToken* pHere = m_pHead;
			bool added = false;
			while (!added)
			{
				if (_stricmp(pHere->Name(), token.Name()) < 0)
				{
					pHere->Prepend(token);
					added = true;

					if (pHere == m_pHead)
					{
						m_pHead = &token;
					}
				}
				else
				{
					if (pHere->Next() != NULL)
					{
						pHere = pHere->Next();
					}
					else
					{
						pHere->Append(token);
						added = true;
					}
				}
			}
		}
		else
		{
			m_pHead = &token;
		}
	}

	//============================================================================

	void CConsole::Remove(CToken& token)
	{
		if (m_pHead != NULL)
		{
			CToken* pToken = FindToken(token.Name());

			if (pToken == m_pHead)
			{
				m_pHead = m_pHead->Next();
			}

			pToken->Unlink();
		}
	}

	//============================================================================

	CConsole::CToken* CConsole::FindToken(const char* name)
	{
		CToken* pToken = NULL;
		CToken* pHere = m_pHead;

		while (pHere != NULL)
		{
			if (_stricmp(pHere->Name(), name) == 0)
			{
				pToken = pHere;
				pHere = NULL;
			}

			pHere = pHere->Next();
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
