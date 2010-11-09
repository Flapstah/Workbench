#if !defined(__ICONSOLE_H__)
#define __ICONSOLE_H__

//==============================================================================

#include "common/macros.h"

//==============================================================================

namespace engine
{
	//============================================================================
	// IConsole
	//============================================================================
	struct IConsole
	{
		struct IVariant;
		struct IArguments;
		typedef void (*OnChangedCallback)(IConsole::IVariant& variant);
		typedef void (*OnCommandCallback)(const IConsole::IArguments* pArguments);

		//==========================================================================
		// IVariant
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

			virtual IConsole::OnChangedCallback GetOnChangedCallback(void) = 0;
			virtual void				SetOnChangedCallback(IConsole::OnChangedCallback pOnChangedCallback) = 0;
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

		enum eConsoleFlags
		{
			CF_WRITE_TO_CONFIG		=	BIT(0),	// Write this variant to the config file
			CF_SERVER_CONTROLLED	= BIT(1),	// Server controlled variant - clients cannot modify
			CF_CHEAT							= BIT(2),	// Modification of this variant is deemed cheating
			CF_DEBUG							= BIT(3),	// This variant can only be modified in non-release builds
			CF_READ_ONLY					= BIT(4),	// This variant can only be set when initialised
			CF_HIDDEN							= BIT(5),	// This variant doesn't show up in any tab-completion lists

			CF_EXTERNAL_BITS			= 0x0000ffff
		};

		// Variants
		virtual IVariant* RegisterVariable(const char* name, int32& variable, int32 initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback) = 0;
		virtual IVariant* RegisterVariable(const char* name, float& variable, float initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback) = 0;
		virtual IVariant* RegisterVariable(const char* name, char*& variable, const char* initial, uint32 flags, const char* help, IConsole::OnChangedCallback pOnChangedCallback) = 0;
		virtual bool UnregisterVariable(const char* name) = 0;

		// Commands
		virtual bool RegisterCommand(const char* name, uint32 flags, const char* help, IConsole::OnCommandCallback pOnCommandCallback) = 0;
		virtual bool UnregisterCommand(const char* name) = 0;

		// Common
		virtual IVariant* Find(const char* name) = 0;
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
