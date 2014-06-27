#include "fuck_game_server_engine.h"

#ifdef WIN32
extern "C" void * sys_alloc(size_t size)
{
#ifndef USE_FUCK_HOOK
	return malloc(size);
#else
	void * ret = VirtualAlloc(0, size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
	return ret;
#endif
}

extern "C" void sys_free(void * p, size_t size)
{
#ifndef USE_FUCK_HOOK
	FASSERT(p);
	free(p);
#else
	 VirtualFree(0, p, 0, MEM_RELEASE);
#endif
}
#else
extern "C" void * sys_alloc(size_t size)
{
#ifndef USE_FUCK_HOOK
	return malloc(size);
#else
	void *result = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    return result;
#endif
}

extern "C" void sys_free(void * p, size_t size)
{
#ifndef USE_FUCK_HOOK
	FASSERT(p);
	free(p);
#else
    munmap(p, size);
#endif
}
#endif


