#if !defined(__ICONSOLE_H__)
#define __ICONSOLE_H__

//==============================================================================

#define REGISTER_CVAR(_name_, _variable_, _initial_, _flags_, _help_, _pOnChangedCallback_) 
//==============================================================================

namespace engine
{
	//============================================================================
	// IConsole
	//============================================================================
	struct IConsole
	{
		//==========================================================================
		// IVaraint
		//==========================================================================
		struct IVariant
		{
			virtual int32				GetAsInt32(void) = 0;
			virtual float				GetAsFloat(void) = 0;
			virtual const char*	GetAsString(void) = 0;

			virtual void				Set(int32 value) = 0;
			virtual void				Set(float value) = 0;
			virtual void				Set(const char* value) = 0;

			virtual const char*	Name(void) = 0;
			virtual const char*	Help(void) = 0;
			virtual uint32			GetFlags(void) = 0;
			virtual void				SetFlags(uint32 flags, uint32 mask) = 0;
		}; // End [struct IVariant]

		//==========================================================================

		//==========================================================================
		// IArguments
		//==========================================================================
		struct IArguments
		{
			virtual uint32 GetCount(void) = 0;
			virtual const char* GetArgument(uint32 index) = 0;
		}; // End [struct IArguments]

		//==========================================================================

		enum eVariantFlags
		{
			VF_WRITE_TO_CONFIG,
			VF_SERVER_CONTROLLED
		};

		// Variables
		typedef void (*OnChangedCallback)(IConsole::IVariant& variant);
		virtual IVariant* RegisterVariable(const char* name, int32& variable, int32 initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback) = 0;
		virtual IVariant* RegisterVariable(const char* name, float& variable, float initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback) = 0;
		virtual IVariant* RegisterVariable(const char* name, char*& variable, const char* initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback) = 0;

		virtual IVariant* RegisterVariable(const char* name, int32 initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback) = 0;
		virtual IVariant* RegisterVariable(const char* name, float initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback) = 0;
		virtual IVariant* RegisterVariable(const char* name, const char* initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback) = 0;

		virtual bool UnregisterVariable(const char* name) = 0;
		virtual bool UnregisterVariable(IConsole::IVariant* pVariant) = 0;

		virtual IVariant* Find(const char* name) = 0;

		// Commands
		typedef void (*OnCommandCallback)(const IConsole::IArguments* pArguments);
		virtual bool RegisterCommand(const char* name, const char* help, IConsole::OnCommandCallback pOnCommandCallback) = 0;

		virtual bool UnregisterCommand(const char* name) = 0;
	}; // End [struct IConsole]

	//----------------------------------------------------------------------------
	// Returns the system console
	//----------------------------------------------------------------------------
	IConsole* GetConsole(void);

	//============================================================================
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__ICONSOLE_H__)]
// [EOF]
