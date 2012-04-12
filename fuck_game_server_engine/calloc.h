#pragma once

extern "C" static force_inline void * sys_alloc(size_t size)
{
	return malloc(size);
}

extern "C" static force_inline void sys_free(void * p)
{
	FASSERT(p);
	free(p);
}
