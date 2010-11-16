#if !defined(__LOGFILE_H__)
#define __LOGFILE_H__

//==============================================================================

#include "common/ilogfile.h"
#include "common/ifilesystem.h"
#include "common/ithread.h"

//==============================================================================

#define LOGFILE_BUFFER_SIZE (1024)
#define LOGFILE_NAME_SIZE (32)

//==============================================================================

namespace engine
{
	class ITimer;

	//============================================================================
	// SLogFileBuffer
	//============================================================================
	struct SLogFileBuffer
	{
		SLogFileBuffer(void) : m_handle(IFileSystem::eFSH_INVALID), m_size(0), m_previousSize(0) { m_buffer.m_UTF16[0] = 0; }

		IFileSystem::eFileSystemHandle m_handle;
		mutex m_mutex;
		uint16 m_size;
		uint16 m_previousSize;
		mchar_t<LOGFILE_BUFFER_SIZE> m_buffer;
	}; // End [struct SLogFileBuffer]

	//============================================================================
	// CLogFile
	//============================================================================
	class CLogFile : public ILogFile
	{
	public:
		CLogFile(const wchar_t* name, CLogFile* pParent, eBehaviourFlag initialBehaviour, SLogFileBuffer* pBuffer);
		CLogFile(const char* name, CLogFile* pParent, eBehaviourFlag initialBehaviour, SLogFileBuffer* pBuffer);
		~CLogFile(void);

		// ILogFile
		virtual void	SetActive(bool active)												{ (active) ? TurnOnBehaviours(eBF_Active) : TurnOffBehaviours(eBF_Active);				}
		virtual bool	IsActive(void) const													{ return (m_pParent != NULL) ? m_pParent->IsActive() : IsBehaviourOn(eBF_Active);	}

		virtual void	TurnOnBehaviours(eBehaviourFlag behaviours)		{ m_behaviours |= (behaviours & eBF_Mask);											}
		virtual void	TurnOffBehaviours(eBehaviourFlag behaviours)	{ m_behaviours &= ~(behaviours & eBF_Mask);											}
		virtual bool	IsBehaviourOn(eBehaviourFlag behaviour) const	{ return ((m_behaviours & eBF_Mask) & behaviour) == behaviour;	}

		virtual bool	Write(const wchar_t* format, ...);
		virtual bool	Write(const char* format, ...);
		// ~ILogFile

	protected:
		enum eInternalBehaviourFlag
		{
			eIBF_Unicode					= BIT(13),
			eIBF_SeparateFile			= BIT(14),
			eIBF_AllocatedBuffer	= BIT(15),

			eIBF_Mask							= eIBF_Unicode | eIBF_SeparateFile | eIBF_AllocatedBuffer
		}; // End [enum eInternalBehaviourFlag]

		void														Initialise(void);
		IFileSystem::eFileSystemHandle	Open(void);
		void														InsertNameUTF16(void);
		void														InsertNameUTF8(void);
		void														InsertDateStampUTF16(void);
		void														InsertDateStampUTF8(void);
		void														InsertLineCountUTF16(void);
		void														InsertLineCountUTF8(void);
		void														InsertFrameCountUTF16(const ITimer* pTimer);
		void														InsertFrameCountUTF8(const ITimer* pTimer);
		void														InsertTimeStampUTF16(const ITimer* pTimer);
		void														InsertTimeStampUTF8(const ITimer* pTimer);
		void														OutputToDebuggerUTF16(void);
		void														OutputToDebuggerUTF8(void);
		void														ForceInsertNewlineUTF16(void);
		void														ForceInsertNewlineUTF8(void);
		bool														Flush(void);
		void														Close(void);

	protected:
		mchar_t<LOGFILE_NAME_SIZE> m_name;
		CLogFile* m_pParent;
		SLogFileBuffer* m_pBuffer;
		uint16 m_behaviours;

		static uint32 s_lineCount;

	private:
	}; // End [class CLogFile : public ILogFile]

	//============================================================================
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__LOGFILE_H__)]
// [EOF]
