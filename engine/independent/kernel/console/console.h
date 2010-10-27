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
		//==========================================================================
		// class CVariant
		//==========================================================================
		class CVariant : public IConsole::IVariant
		{
		public:
			CVariant(const char* name, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback)
				: m_pOnChangedCallback(pOnChangedCallback), m_name(name), m_help(help), m_flags(flags)
																															{																								}
			~CVariant(void)																					{																								}

			// IVariant
			virtual void				Set(int32 value)										{																								}
			virtual void				Set(float value)										{																								}
			virtual void				Set(const char* value)							{																								}

			virtual const char*	Name(void)													{	return m_name;																}
			virtual const char*	Help(void)													{	return m_help;																}
			virtual uint32			GetFlags(void)											{	return m_flags;																}
			virtual void				SetFlags(uint32 flags, uint32 mask)	{	m_flags = (m_flags & ~mask) | (flags & mask);	}
			// ~IVariant

		protected:
			IConsole::OnChangedCallback m_pOnChangedCallback;
			const char* m_name;
			const char* m_help;
			uint32 m_flags;

		private:
		}; // End [class CVariant : public IVariant]

		//==========================================================================

		//==========================================================================
		// class CVariantInt32
		//==========================================================================
		class CVariantInt32 : public CVariant
		{
		public:
			CVariantInt32(const char* name, int32& variable, int32 initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback)
				: CVariant(name, flags, help, pOnChangedCallback), m_variable(variable)
																															{																								}
			~CVariantInt32(void)																		{																								}

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
				: CVariant(name, flags, help, pOnChangedCallback), m_variable(variable)
																															{																								}
			~CVariantFloat(void)																		{																								}

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
				: CVariant(name, flags, help, pOnChangedCallback), m_variable(variable)
																															{																								}
			~CVariantString(void)																		{																								}

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

	protected:
	private:
	}; // End [class CConsole : public IConsole]

	//============================================================================

	// IConsole
	virtual IVariant* RegisterVariable(const char* name, int32& variable, int32 initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback);
	virtual IVariant* RegisterVariable(const char* name, float& variable, float initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback);
	virtual IVariant* RegisterVariable(const char* name, char*& variable, const char* initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback);

	virtual IVariant* RegisterVariable(const char* name, int32 initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback);
	virtual IVariant* RegisterVariable(const char* name, float initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback);
	virtual IVariant* RegisterVariable(const char* name, const char* initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback);

	virtual bool UnregisterVariable(const char* name);
	virtual bool UnregisterVariable(IConsole::IVariant* pVariant);

	virtual IVariant* Find(const char* name);

	virtual bool RegisterCommand(const char* name, const char* help, IConsole::OnCommandCallback pOnCommandCallback);

	virtual bool UnregisterCommand(const char* name);
	// ~IConsole

	//----------------------------------------------------------------------------
	// The global instance of the console
	//----------------------------------------------------------------------------
	extern CConsole* g_console;

	//============================================================================
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__CONSOLE_H__)]
// [EOF]
