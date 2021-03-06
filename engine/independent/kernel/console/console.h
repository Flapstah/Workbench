#if !defined(__CONSOLE_H__)
#define __CONSOLE_H__

//==============================================================================

#include "common/iconsole.h"

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
				: m_name(name), m_help(help), m_flags(flags), m_pNext(NULL), m_pPrevious(NULL)	{											}
			virtual ~CToken(void)																		{	Unlink();																			}

			virtual const char*	Name(void)													{	return m_name;																}
			virtual const char*	Help(void)													{	return m_help;																}
			virtual uint32			GetFlags(void)											{	return m_flags;																}

			void								SetFlags(uint32 flags, uint32 mask)	{	m_flags = (m_flags & ~mask) | (flags & mask);	}

			CToken*							Next(void)													{ return m_pNext;																}
			CToken*							Previous(void)											{ return m_pPrevious;														}

			void								Append(CToken& token);
			void								Prepend(CToken& token);
			void								Unlink(void);

		protected:

			const char* m_name;
			const char* m_help;
			uint32 m_flags;

			CToken* m_pNext;
			CToken* m_pPrevious;

		private:
      PREVENT_CLASS_COPY(CToken);
		}; // End [class CToken]

		//==========================================================================

		//==========================================================================
		// class CVariant
		//==========================================================================
		class CVariant : public CToken, public IConsole::IVariant
		{
		public:
			CVariant(const char* name, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback)
				: CToken(name, help, flags | ICF_VARIANT), m_pOnChangedCallback(pOnChangedCallback)	{									}
			virtual ~CVariant(void)																									{																}

			// IVariant
			virtual void				Set(int32 value)																		{	IGNORE_PARAMETER(value);			}
			virtual void				Set(float value)																		{	IGNORE_PARAMETER(value);			}
			virtual void				Set(const char* value)															{	IGNORE_PARAMETER(value);			}

			virtual const char*	Name(void)																					{	return CToken::Name();				}
			virtual const char*	Help(void)																					{	return CToken::Help();				}
			virtual uint32			GetFlags(void)																			{	return CToken::GetFlags();		}

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
				: CVariant(name, flags, help, pOnChangedCallback), m_variable(variable)	{	Set(initial);								}
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
				: CVariant(name, flags, help, pOnChangedCallback), m_variable(variable)	{	Set(initial);								}
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
			CVariantString(const char* name, char*& variable, const char* initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback)
				: CVariant(name, flags, help, pOnChangedCallback), m_variable(variable)	{	Set(initial);								}
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
      PREVENT_CLASS_COPY(CVariantString);
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

		CConsole(void) : m_pHead(NULL)	{}
		virtual ~CConsole(void) {}

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

		void Add(CToken& token);
		void Remove(CToken& token);

	protected:
		CToken* FindToken(const char* name);

		CToken* m_pHead;

	private:
    PREVENT_CLASS_COPY(CConsole);
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
