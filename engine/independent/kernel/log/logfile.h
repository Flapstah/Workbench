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
	//============================================================================
	// SLogFileBuffer
	//============================================================================
	struct SLogFileBuffer
	{
		SLogFileBuffer(void) : m_handle(IFileSystem::eFSH_INVALID), m_size(0), m_previousSize(0) { m_buffer[0] = 0; }

		IFileSystem::eFileSystemHandle m_handle;
		TCHAR m_buffer[LOGFILE_BUFFER_SIZE];
		mutex m_mutex;
		uint16 m_size;
		uint16 m_previousSize;
	}; // End [struct SLogFileBuffer]

	//============================================================================
	// CLogFile
	//============================================================================
	class CLogFile : public ILogFile
	{
	public:
		CLogFile(const TCHAR* name, CLogFile* pParent, eBehaviourFlag initialBehaviour, SLogFileBuffer* pBuffer);
		~CLogFile(void);

		// ILogFile
		virtual void SetActive(bool active)													{ (active) ? TurnOnBehaviours(eBF_Active) : TurnOffBehaviours(eBF_Active);				}
		virtual bool IsActive(void) const														{ return (m_pParent != NULL) ? m_pParent->IsActive() : IsBehaviourOn(eBF_Active);	}

		virtual void TurnOnBehaviours(eBehaviourFlag behaviours)		{ m_behaviours |= behaviours;											}
		virtual void TurnOffBehaviours(eBehaviourFlag behaviours)		{ m_behaviours &= ~behaviours;										}
		virtual bool IsBehaviourOn(eBehaviourFlag behaviour) const	{ return (m_behaviours & behaviour) == behaviour;	}

		virtual bool Write(const TCHAR* format, ...);
		// ~ILogFile

	protected:
		enum eInternalBehaviourFlag
		{
			eBF_SeparateFile			= 1 << 14,
			eIBF_AllocatedBuffer	= 1 << 15
		}; // End [enum eInternalBehaviourFlag]

		IFileSystem::eFileSystemHandle Open(void);
		bool Flush(void);
		void Close(void);

	protected:
		TCHAR m_name[LOGFILE_NAME_SIZE];
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
