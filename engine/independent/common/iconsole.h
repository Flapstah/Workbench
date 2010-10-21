#if !defined(__ICONSOLE_H__)
#define __ICONSOLE_H__

//==============================================================================

namespace engine
{
	struct IConsole
	{
		template<typename _type> struct IVariable
		{
			virtual const _type Get(void) = 0;
			virtual void Set(const _type value) = 0;
		}; // End [struct IVariable]

		// Variables
		typedef void (*OnChangedCallback)(IConsole::IVariable& variable);
		virtual IVariable* RegisterVariable(const char* name, int32& variable, int32 default, int32 minimum, int32 maximum, const char* help, OnChangedCallback pOnChangedCallback) = 0;
		virtual IVariable* RegisterVariable(const char* name, float& variable, float default, float minimum, float maximum, const char* help, OnChangedCallback pOnChangedCallback) = 0;
		virtual IVariable* RegisterVariable(const char* name, char*& variable, const char* default, const char* help, OnChangedCallback pOnChangedCallback) = 0;

		virtual IVariable* RegisterVariable(const char* name, int32 default, int32 minimum, int32 maximum, const char* help, OnChangedCallback pOnChangedCallback) = 0;
		virtual IVariable* RegisterVariable(const char* name, float default, float minimum, float maximum, const char* help, OnChangedCallback pOnChangedCallback) = 0;
		virtual IVariable* RegisterVariable(const char* name, const char* default, const char* help, OnChangedCallback pOnChangedCallback) = 0;

		virtual bool UnregisterVariable(const char* name) = 0;
		virtual bool UnregisterVariable(IVariable* pVariable) = 0;

		virtual IVariable* Find(const char* name);

		// Commands
		struct IArguments
		{
			virtual uint32 GetCount(void) = 0;
			virtual const char* GetArgument(uint32 index) = 0;
		}; // End [struct IArguments]

		typedef void (*OnCommandCallback)(const IArguments* pArguments);
		virtual bool RegisterCommand(const char* name, const char* help, OnCommandCallback pOnCommandCallback);

		virtual bool UnregisterCommand(const char* name);
	}; // End [struct IConsole]
}

//==============================================================================

#endif // End [!defined(__ICONSOLE_H__)]
// [EOF]
