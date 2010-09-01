#include "stdafx.h"

#include "kernel/memory/osmemory.h"
#include "kernel/log/logfile.h"

#include <stdlib.h>

//==============================================================================

namespace engine
{
	static CLogFile gs_MemoryLog(_TEXT("Memory"), g_MainLog, static_cast<ILogFile::eBehaviourFlag>(ILogFile::eBF_Active | ILogFile::eBF_Name | ILogFile::eBF_OutputToDebugger | ILogFile::eBF_FlushEachWrite));
	static CLogFile gs_MemoryErrorLog(_TEXT("Memory Error"), &gs_MemoryLog, static_cast<ILogFile::eBehaviourFlag>(ILogFile::eBF_Active | ILogFile::eBF_Name | ILogFile::eBF_OutputToDebugger | ILogFile::eBF_FlushEachWrite));
#define MemoryLog(_output_) _WriteLog(&engine::gs_MemoryLog, _output_)
#define MemoryErrorLog(_output_) _WriteLog(&engine::gs_MemoryErrorLog, _output_)

	static COSMemory::GUARD guard = 0xDEADC0DE;

	uint32 COSMemory::Node::s_id = 0;

	COSMemory::Node::Node(size_t size, TCHAR* file, uint32 line)
		: m_pNext(NULL)
		, m_pPrevious(NULL)
		, m_size(size)
		, m_file(file)
		, m_line(line)
		, m_id(++s_id)
	{

	}

	void* COSMemory::malloc(size_t size, TCHAR* file, uint32 line)
	{
		size_t totalSize = size + sizeof(Node) + (sizeof(GUARD) * 2);

		uint8* pMemory = static_cast<uint8*>(::malloc(totalSize));
		if (pMemory != NULL)
		{
			new(pMemory) Node(totalSize, file, line);
			*(reinterpret_cast<GUARD*>(&pMemory[sizeof(Node)])) = guard;
			*(reinterpret_cast<GUARD*>(&pMemory[sizeof(Node) + sizeof(GUARD) + size])) = guard;

			++m_allocatedNodes;
			m_allocatedBytes += totalSize;

			if (m_pAllocatedList != NULL)
			{
				pMemory->m_pNext = m_pAllocatedList;
				m_pAllocatedList = m_pAllocatedList->m_pPrevious = pMemory;
			}
			else
			{
				m_pAllocatedList = pMemory;
			}

			MemoryLog(("%s(%i): allocated %i(%i) bytes", file, line, size, totalSize));
			pMemory = &pMemory[sizeof(Node) + sizeof(GUARD)];
		}
		else
		{
			MemoryErrorLog(("%s(%i): failed to allocate %i bytes", file, line, size));
		}

		return pMemory;
	}

	void* COSMemory::calloc(size_t size, TCHAR* file, uint32 line)
	{
		uint8* pMemory = malloc(size, file, line);
		if (pMemory != NULL)
		{
			memset(&pMemory[sizeof(Node) + sizeof(GUARD)], 0, size);
		}

		return pMemory;
	}

	void* COSMemory::realloc(void* pMemory, size_t size, TCHAR* file, uint32 line)
	{
		uint8* pNewMemory = malloc(size, file, line);
		if (pNewMemory != NULL)
		{
			memcpy(&pNewMemory[sizeof(Node) + sizeof(GUARD)], pMemory, size);
			free(pMemory, file, line);
		}

		return pNewMemory;
	}

	void COSMemory::free(void* pMemory, TCHAR* file, uint32 line)
	{
		if ((pMemory != NULL) && (m_pAllocatedList != NULL))
		{
			Node* pAllocatedNode = static_cast<uint8*>(pMemory) - sizeof(Node) - sizeof(GUARD);
			Node* pNode = m_pAllocatedList;
			while ((pNode != NULL) && (pNode != pAllocatedNode))
			{
				pNode = pNode->m_pNext;
			}

			if (pNode != NULL)
			{
				if (pNode->m_pPrevious != NULL)
				{
					pNode->m_pPrevious->m_pNext = pNode->m_pNext;
				}

				if (pNode->m_pNext != NULL)
				{
					pNode->m_pNext->m_pPrevious = pNode->m_pPrevious;
				}

				--m_allocatedNodes;
				m_allocatedBytes -= pNode->m_size;

				MemoryLog(("%s(%i): freed %i(%i) bytes", file, line, pNode->m_size - sizeof(Node) - (sizeof(GUARD) * 2), pNode->m_size));
			}
			else
			{
				MemoryErrorLog(("%s(%i): Memory [%p] not allocated by this allocator", file, line, pMemory));
			}
		}
	}

}

//==============================================================================
// [EOF]
