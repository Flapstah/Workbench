#if !defined(__OSMEMORY_H__)
#define __OSMEMORY_H__

//==============================================================================

#include "common/imemory.h"

//==============================================================================

namespace engine
{
	//============================================================================
	// COSMemory
	//============================================================================
	class COSMemory : public IMemory
	{
	public:
		// IMemory
		virtual void* malloc(size_t size, const char* file, uint32 line);
		virtual void* calloc(size_t size, const char* file, uint32 line);
		virtual void* realloc(void* pMemory, size_t size, const char* file, uint32 line);
		virtual void free(void* pMemory, const char* file, uint32 line);
		// ~IMemory

		typedef uint32 GUARD;

	protected:

		struct Node 
		{
			Node(size_t size, const char* file, uint32 line);

			Node*					m_pNext;
			Node*					m_pPrevious;

			size_t				m_size;
			const char*		m_file;
			uint32				m_line;
			uint32				m_id;

			static uint32	s_id;
		}; // End [struct Node]

		static Node*	m_pAllocatedList;

		size_t				m_allocatedBytes;
		size_t				m_allocatedNodes;

	private:
	}; // End [class COSMemory : public IMemory]

	//============================================================================
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__OSMEMORY_H__)]
// [EOF]
