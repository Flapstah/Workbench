#if !defined(__ICONFIGURATION_H__)
#define __ICONFIGURATION_H__

//==============================================================================

namespace engine
{
	struct SConfigurationVariables
	{

	};

	struct IConfigurationSet
	{
		virtual bool	Initialise(SConfigurationVariables& variables, const char* pPrefix);
		virtual bool	Push(void);
		virtual bool	Pop(void);
		virtual SConfigurationVariables& Get(void);

		virtual bool	Load(const char* pConfigurationFile);
		virtual bool	Save(const char* pConfigurationFile);
	};

	struct IConfiguration 
	{
		virtual bool Add(IConfigurationSet& configurationSet);
		virtual bool 
	};
}

//==============================================================================
#endif // End [!defined(__ICONFIGURATION_H__)]
// [EOF]
