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
	struct ITimer;

	//============================================================================
	// SLogFileBuffer
	//============================================================================
	struct SLogFileBuffer
	{
		SLogFileBuffer(void) : m_handle(IFileSystem::eFSH_INVALID), m_size(0), m_previousSize(0) { m_buffer[0] = 0; }

		IFileSystem::eFileSystemHandle m_handle;
		mutex m_mutex;
		uint16 m_size;
		uint16 m_previousSize;
		char m_buffer[LOGFILE_BUFFER_SIZE];
	}; // End [struct SLogFileBuffer]

	//============================================================================
	// CLogFile
	//============================================================================
	class CLogFile : public ILogFile
	{
	public:
		CLogFile(const char* name, CLogFile* pParent, eBehaviourFlag initialBehaviour, SLogFileBuffer* pBuffer);
		~CLogFile(void);

		// ILogFile
		virtual void	SetActive(bool active)												{ (active) ? TurnOnBehaviours(eBF_Active) : TurnOffBehaviours(eBF_Active);																	}
		virtual bool	IsActive(void) const													{ return (m_pParent != NULL) ? m_pParent->IsActive() : IsBehaviourOn(eBF_Active);														}

		virtual void	TurnOnBehaviours(eBehaviourFlag behaviours)		{ m_behaviours |= (behaviours & eBF_Mask);																																	}
		virtual void	TurnOffBehaviours(eBehaviourFlag behaviours)	{ m_behaviours &= ~(behaviours & eBF_Mask);																																	}
		virtual bool	IsBehaviourOn(eBehaviourFlag behaviour) const	{ return ((m_behaviours & eBF_Mask) & behaviour) == behaviour;																							}
		virtual bool	PushBehaviours(void)													{ return (m_behavioursStack == 0) ? m_behavioursStack = m_behaviours, true : false;													}
		virtual bool  PopBehaviours(void)														{ return (m_behavioursStack != 0) ? m_behaviours = m_behavioursStack, m_behavioursStack = 0, true : false;	}

		virtual bool	Write(const char* format, ...);
		// ~ILogFile

	protected:
		enum eInternalBehaviourFlag
		{
			eIBF_SeparateFile			= BIT(14),
			eIBF_AllocatedBuffer	= BIT(15),

			eIBF_Mask							= eIBF_SeparateFile | eIBF_AllocatedBuffer
		}; // End [enum eInternalBehaviourFlag]

		void														Initialise(void);
		IFileSystem::eFileSystemHandle	Open(void);
		void														InsertName(void);
		void														InsertDateStamp(void);
		void														InsertLineCount(void);
		void														InsertFrameCount(const ITimer* pTimer);
		void														InsertTimeStamp(const ITimer* pTimer);
		void														OutputToDebugger(void);
		void														ForceInsertNewline(void);
		bool														Flush(void);
		void														Close(void);

	protected:
		char m_name[LOGFILE_NAME_SIZE];
		CLogFile* m_pParent;
		SLogFileBuffer* m_pBuffer;
		uint16 m_behaviours;
		uint16 m_behavioursStack;

		static uint32 s_lineCount;

	private:
	}; // End [class CLogFile : public ILogFile]

	//============================================================================
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__LOGFILE_H__)]
// [EOF]
