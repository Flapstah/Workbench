#if !defined(__IMEMORY_H__)
#define __IMEMORY_H__

//==============================================================================

namespace engine
{
	struct IMemory
	{
		virtual void* malloc(size_t size, TCHAR* file, uint32 line) = 0;
		virtual void* calloc(size_t size, TCHAR* file, uint32 line) = 0;
		virtual void* realloc(void* pMemory, size_t size, TCHAR* file, uint32 line) = 0;
		virtual void free(void* pMemory, TCHAR* file, uint32 line) = 0;
	};
}

//==============================================================================
#endif // End [!defined(__IMEMORY_H__)]
// [EOF]
