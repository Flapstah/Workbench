#if !defined(__IMEMORY_H__)
#define __IMEMORY_H__

//==============================================================================

namespace engine
{
	//============================================================================
	// IMemory
	//============================================================================

	struct IMemory
	{
		virtual void* malloc(size_t size, const char* file, uint32 line) = 0;
		virtual void* calloc(size_t size, const char* file, uint32 line) = 0;
		virtual void* realloc(void* pMemory, size_t size, const char* file, uint32 line) = 0;
		virtual void free(void* pMemory, const char* file, uint32 line) = 0;
	}; // End [struct IMemory]

	//============================================================================
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__IMEMORY_H__)]
// [EOF]
