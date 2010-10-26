#if !defined(__CONSOLE_H__)
#define __CONSOLE_H__

//==============================================================================

#include "common/iconsole.h"

//==============================================================================

namespace engine
{
	class CConsole : public IConsole
	{
	public:
		class CVariant : public IVariant
		{
		public:
			CVariant(const char* name, uint32 flags, const char* help, OnChangedCallback pOnChangedCallback);
			~CVariant(void);

			// IVariant
			virtual void				Set(int32 value) {};
			virtual void				Set(float value) {};
			virtual void				Set(const char* value) {};

			virtual const char*	Name(void);
			virtual const char*	Help(void);
			virtual uint32			GetFlags(void);
			virtual void				SetFlags(uint32 flags, uint32 mask);
			// ~IVariant

		protected:
			OnChangedCallback m_pOnChangedCallback;
			const char* m_name;
			const char* m_help;
			uint32 m_flags;

		private:
		};

		class CVariantInt32 : public CVariant
		{
		public:
			CVariantInt32(const char* name, int32& variable, int32 initial, uint32 flags, const char* help, OnChangedCallback pOnChangedCallback);
			~CVariantInt32(void);

			// IVariant
			virtual void				Set(int32 value);

			virtual int32				GetAsInt32(void);
			virtual float				GetAsFloat(void);
			virtual const char*	GetAsString(void);
			// ~IVariant

		protected:
			int32& m_variable;

		private:
		};

		class CVariantFloat : public CVariant
		{
		public:
			CVariantFloat(const char* name, float& variable, float initial, uint32 flags, const char* help, OnChangedCallback pOnChangedCallback);
			~CVariantFloat(void);

			// IVariant
			virtual void				Set(float value);

			virtual int32				GetAsInt32(void);
			virtual float				GetAsFloat(void);
			virtual const char*	GetAsString(void);
			// ~IVariant

		protected:
			float& m_variable;

		private:
		};

		class CVariantString : public CVariant
		{
		public:
			CVariantString(const char* name, char*& variable, const char* initial, uint32 flags, const char* help, OnChangedCallback pOnChangedCallback);
			~CVariantString(void);

			// IVariant
			virtual void				Set(const char* value) {};

			virtual int32				GetAsInt32(void);
			virtual float				GetAsFloat(void);
			virtual const char*	GetAsString(void);
			// ~IVariant

		protected:
			char* m_value;

		private:
		};

	protected:
	private:
	};

}

//==============================================================================

#endif // End [!defined(__CONSOLE_H__)]
// [EOF]
