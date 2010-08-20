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
		CLogFile(const TCHAR* name, CLogFile* pParent, eBehaviourFlag initialBehaviour = eBF_ALL, eChannelFlag initialChannels = eCF_ALL);
		~CLogFile(void);

		// ILogFile
		virtual void SetActive(bool active)													{ active ? TurnOnBehaviours(eBF_Active) : TurnOffBehaviours(eBF_Active);					}
		virtual bool IsActive(eChannelFlag channels) const
		{
#if defined LOGS_HAVE_SCOPED_CHANNELS
			bool active = (m_pParent != NULL) ? m_pParent->IsBehaviourOn(eBF_Active) : IsBehaviourOn(eBF_Active);
#else
			bool active = (m_pParent != NULL) ? m_pParent->IsActive(channels) : IsBehaviourOn(eBF_Active);
#endif
			return active ? (m_channels & channels) != 0 : false;
		}

		virtual void TurnOnBehaviours(eBehaviourFlag behaviours)		{ m_behaviours |= behaviours;											}
		virtual void TurnOffBehaviours(eBehaviourFlag behaviours)		{ m_behaviours &= ~behaviours;										}
		virtual bool IsBehaviourOn(eBehaviourFlag behaviour) const	{ return (m_behaviours & behaviour) == behaviour;	}

		virtual void TurnOnChannels(eChannelFlag channels)					{ m_channels |= channels;													}
		virtual void TurnOffChannels(eChannelFlag channels)					{ m_channels &= ~channels;												}
		virtual bool IsChannelOn(eChannelFlag channel) const				{ return (m_channels & channel) == channel;				}

		virtual bool Write(const TCHAR* format, ...);
		// ~ILogFile

	protected:
		IFileSystem::eFileSystemHandle m_handle;
		CLogFile* m_pParent;
		TCHAR m_name[LOGFILE_NAME_SIZE];
		TCHAR m_buffer[LOGFILE_BUFFER_SIZE];
		uint32 m_behaviours;
		uint32 m_channels;

	private:
	}; // End [class CLogFile : public ILogFile]
}
//==============================================================================
#endif // End [!defined(__LOGFILE_H__)]
// [EOF]
