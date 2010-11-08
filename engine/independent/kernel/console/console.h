#if !defined(__CONSOLE_H__)
#define __CONSOLE_H__

//==============================================================================

#include "common/iconsole.h"
#include "templates/stack_list.h"

//==============================================================================

namespace engine
{
	//============================================================================
	// CConsole
	//============================================================================
	class CConsole : public IConsole
	{
	public:
		enum eInternalConsoleFlags
		{
			ICF_VARIANT					= BIT(16),	// Indicates this is a variant (otherwise it's a command)
			ICF_CREATED_ON_HEAP	= BIT(17)		// Indicates this variant/command was created on the heap (through RegisterVariant() or RegisterCommand())
		};

		//==========================================================================
		// CToken
		//==========================================================================
		class CToken
		{
		public:
			CToken(const char* name, const char* help, uint32 flags)
				: m_name(name), m_help(help), m_flags(flags)					{																								}
			virtual ~CToken(void)																		{																								}

			// IVariant (partial)
			virtual const char*	Name(void)													{	return m_name;																}
			virtual const char*	Help(void)													{	return m_help;																}
			virtual uint32			GetFlags(void)											{	return m_flags;																}
			// ~IVariant

			virtual void				SetFlags(uint32 flags, uint32 mask)	{	m_flags = (m_flags & ~mask) | (flags & mask);	}

		protected:
			const char* m_name;
			const char* m_help;
			uint32 m_flags;

		private:
		}; // End [class CToken]

		//==========================================================================

		//==========================================================================
		// class CVariant
		//==========================================================================
		class CVariant : public CToken, IConsole::IVariant
		{
		public:
			CVariant(const char* name, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback)
				: CToken(name, help, flags | ICF_VARIANT), m_pOnChangedCallback(pOnChangedCallback)	{									}
			virtual ~CVariant(void)																									{																}

			// IVariant (partial - other half in CToken)
			virtual void				Set(int32 value)																		{																}
			virtual void				Set(float value)																		{																}
			virtual void				Set(const char* value)															{																}

			virtual IConsole::OnChangedCallback GetOnChangedCallback(void)					{	return m_pOnChangedCallback;	}
			virtual void				SetOnChangedCallback(IConsole::OnChangedCallback pOnChangedCallback)
																																{	m_pOnChangedCallback = pOnChangedCallback;	}
			// ~IVariant

		protected:
			IConsole::OnChangedCallback m_pOnChangedCallback;

		private:
		}; // End [class CVariant : public CToken, IConsole::IVariant]

		//==========================================================================

		//==========================================================================
		// class CVariantInt32
		//==========================================================================
		class CVariantInt32 : public CVariant
		{
		public:
			CVariantInt32(const char* name, int32& variable, int32 initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback)
				: CVariant(name, flags, help, pOnChangedCallback), m_variable(variable)	{															}
			virtual ~CVariantInt32(void)														{																								}

			// IVariant
			virtual void				Set(int32 value)										{	m_variable = value;														}

			virtual int32				GetAsInt32(void)										{	return m_variable;														}
			virtual float				GetAsFloat(void)										{ return static_cast<float>(m_variable);				}
			virtual const char*	GetAsString(void);
			// ~IVariant

		protected:
			int32& m_variable;

		private:
		}; // End [class CVariantInt32 : public CVariant]

		//==========================================================================

		//==========================================================================
		// class CVariantFloat
		//==========================================================================
		class CVariantFloat : public CVariant
		{
		public:
			CVariantFloat(const char* name, float& variable, float initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback)
				: CVariant(name, flags, help, pOnChangedCallback), m_variable(variable)	{															}
			virtual ~CVariantFloat(void)														{																								}

			// IVariant
			virtual void				Set(float value)										{	m_variable = value;														}

			virtual int32				GetAsInt32(void)										{	return static_cast<int32>(m_variable);				}
			virtual float				GetAsFloat(void)										{ return m_variable;														}
			virtual const char*	GetAsString(void);
			// ~IVariant

		protected:
			float& m_variable;

		private:
		}; // End [class CVariantFloat : public CVariant]

		//==========================================================================

		//==========================================================================
		// class CVariantString
		//==========================================================================
		class CVariantString : public CVariant
		{
		public:
			CVariantString(const char* name, const char*& variable, const char* initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback)
				: CVariant(name, flags, help, pOnChangedCallback), m_variable(variable)	{															}
			virtual ~CVariantString(void)														{																								}

			// IVariant
			virtual void				Set(const char* value)							{	m_variable = value;														}

			virtual int32				GetAsInt32(void)										{	return atoi(m_variable);											}
			virtual float				GetAsFloat(void)										{ return static_cast<float>(atof(m_variable));	}
			virtual const char*	GetAsString(void)										{ return m_variable;														}
			// ~IVariant

		protected:
			const char* m_variable;

		private:
		}; // End [class CVariantString : public CVariant]

		//==========================================================================

		//==========================================================================
		// CCommand
		//==========================================================================
		class CCommand : public CToken
		{
		public:
			CCommand(const char* name, uint32 flags, const char* help, IConsole::OnCommandCallback pOnCommandCallback)
				: CToken(name, help, flags & ~ICF_VARIANT), m_pOnCommandCallback(pOnCommandCallback)	{								}
			~CCommand(void)																													{																}

			virtual void DoCommand(IConsole::IArguments* pArguments)								{ (*m_pOnCommandCallback)(pArguments);	}

		protected:
			IConsole::OnCommandCallback m_pOnCommandCallback;
		private:
		}; // End [class CCommand : public CToken]

		//==========================================================================

		// IConsole
		virtual IVariant* RegisterVariable(const char* name, int32& variable, int32 initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback);
		virtual IVariant* RegisterVariable(const char* name, float& variable, float initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback);
		virtual IVariant* RegisterVariable(const char* name, char*& variable, const char* initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback);
		virtual bool UnregisterVariable(const char* name);

		virtual bool RegisterCommand(const char* name, uint32 flags, const char* help, IConsole::OnCommandCallback pOnCommandCallback);
		virtual bool UnregisterCommand(const char* name);

		virtual IVariant* Find(const char* name);
		// ~IConsole

		//==========================================================================

		CToken* FindToken(const char* name);

	protected:
		typedef TStackList<CToken*> token_list;
		typedef TStackList<CToken*>::TIterator token_list_iterator;
		token_list m_tokenList;

	private:
	}; // End [class CConsole : public IConsole]

	//----------------------------------------------------------------------------
	// The global instance of the console
	//----------------------------------------------------------------------------
	extern CConsole g_console;

	//============================================================================
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__CONSOLE_H__)]
// [EOF]
