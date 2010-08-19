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
		CLogFile(const TCHAR* name, CLogFile* pParent);
		~CLogFile(void);

		// ILogFile
		virtual void SetActive(bool active)													{ active ? TurnOnBehaviours(eBF_Active) : TurnOffBehaviours(eBF_Active);					}
		virtual bool IsActive(void) const														{ return (m_pParent != NULL) ? m_pParent->IsActive() : IsBehaviourOn(eBF_Active);	}

		virtual void TurnOnBehaviours(eBehaviourFlag behaviours)		{ m_behaviours |= behaviours;											}
		virtual void TurnOffBehaviours(eBehaviourFlag behaviours)		{ m_behaviours &= ~behaviours;										}
		virtual bool IsBehaviourOn(eBehaviourFlag behaviour) const	{ return (m_behaviours & behaviour) == behaviour;	}

		virtual void TurnOnChannels(eChannelFlag channels)					{ m_channels |= channels;													}
		virtual void TurnOffChannels(eChannelFlag channels)					{ m_channels &= ~channels;												}
		virtual bool IsChannelOn(eChannelFlag channel) const				{ return (m_channels & channel) == channel;				}

		virtual void Write(eChannelFlag channel, const TCHAR* format, ...);
		// ~ILogFile

	protected:
		IFileSystem::eFileSystemHandle m_handle;
		CLogFile* m_pParent;
		TCHAR m_name[LOGFILE_NAME_SIZE];
		TCHAR m_buffer[LOGFILE_BUFFER_SIZE];
		uint32 m_channels;
		uint32 m_behaviours;

	private:
	}; // End [class CLogFile : public ILogFile]
}
//==============================================================================
#endif // End [!defined(__LOGFILE_H__)]
// [EOF]
