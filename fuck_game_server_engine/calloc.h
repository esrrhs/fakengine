#pragma once

extern "C" force_inline void * sys_alloc(size_t size)
{
	return malloc(size);
}

extern "C" force_inline void sys_free(void * p)
{
	FASSERT(p);
	free(p);
}
