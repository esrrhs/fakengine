#include "fuck_game_server_engine.h"

extern "C" void * sys_alloc(size_t size)
{
	return malloc(size);
}

extern "C" void sys_free(void * p)
{
	FASSERT(p);
	free(p);
}
