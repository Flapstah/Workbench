#if !defined(__LOGFILE_H__)
#define __LOGFILE_H__

//==============================================================================

#include "common/ilogfile.h"
#include "common/ifilesystem.h"

//==============================================================================

#define LOGFILE_BUFFER_SIZE (1024)
#define LOGFILE_NAME_SIZE (32)

//==============================================================================

namespace engine
{
	//============================================================================
	// CLogFile
	//============================================================================
	class CLogFile : public ILogFile
	{
	public:
		CLogFile(const TCHAR* name, CLogFile* pParent, eBehaviourFlag initialBehaviour = eBF_ALL);
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
		IFileSystem::eFileSystemHandle GetFileHandle(void)					{ return (m_pParent != NULL) ? m_handle = m_pParent->GetFileHandle() : m_handle;	}
		IFileSystem::eFileSystemHandle Open(void);
		bool Flush(void);
		void Close(void);

	protected:
		IFileSystem::eFileSystemHandle m_handle;
		CLogFile* m_pParent;
		TCHAR m_name[LOGFILE_NAME_SIZE];
		TCHAR m_buffer[LOGFILE_BUFFER_SIZE];
		uint32 m_size;
		uint32 m_behaviours;

	private:
	}; // End [class CLogFile : public ILogFile]
}
//==============================================================================
#endif // End [!defined(__LOGFILE_H__)]
// [EOF]
