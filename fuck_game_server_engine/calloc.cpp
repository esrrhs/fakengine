#include "fuck_game_server_engine.h"

#ifndef WIN32

/* Prototypes for our hooks.  */
static void my_init_hook(void);

/* Variables to save original hooks. */
void* (* old_malloc_hook)(size_t, const void*) = NULL;
void* (* old_realloc_hook)(void*, size_t, const void*) = NULL;
void (* old_free_hook)(void*, const void*) = NULL;
void* (* old_memalign_hook)(size_t,size_t, const void*) = NULL;

/* Override initializing hook from the C library. */
void (*__malloc_initialize_hook) (void) = my_init_hook;

#endif

#ifdef WIN32
extern "C" void * sys_alloc(size_t size)
{
	return malloc(size);
}

extern "C" void sys_free(void * p)
{
	FASSERT(p);
	free(p);
}
#else
extern "C" void * sys_alloc(size_t size)
{
#ifndef USE_ALLOC_HOOK
	return malloc(size);
#else
    /* Restore all old hooks */
    __malloc_hook = old_malloc_hook;
    __realloc_hook = old_realloc_hook;
    __free_hook = old_free_hook;
    __memalign_hook = old_memalign_hook;
    
	void *result = malloc(size);
	
    /* Save underlying hooks */
    old_malloc_hook = __malloc_hook;
    old_realloc_hook = __realloc_hook;
    old_free_hook = __free_hook;
    old_memalign_hook = __memalign_hook;
    
    /* Restore our own hooks */
    __malloc_hook = glibc_override_malloc;
    __realloc_hook = glibc_override_realloc;
    __free_hook = glibc_override_free;
    __memalign_hook = glibc_override_memalign;

    return result;
#endif
}

extern "C" void sys_free(void * p)
{
#ifndef USE_ALLOC_HOOK
	FASSERT(p);
	free(p);
#else
    /* Restore all old hooks */
    __malloc_hook = old_malloc_hook;
    __realloc_hook = old_realloc_hook;
    __free_hook = old_free_hook;
    __memalign_hook = old_memalign_hook;
    
	free(p);
	
    /* Save underlying hooks */
    old_malloc_hook = __malloc_hook;
    old_realloc_hook = __realloc_hook;
    old_free_hook = __free_hook;
    old_memalign_hook = __memalign_hook;
    
    /* Restore our own hooks */
    __malloc_hook = glibc_override_malloc;
    __realloc_hook = glibc_override_realloc;
    __free_hook = glibc_override_free;
    __memalign_hook = glibc_override_memalign;

#endif
}
#endif

#ifndef WIN32
#ifdef USE_ALLOC_HOOK

extern "C" void* glibc_override_malloc(size_t size, const void *caller) 
{
    return falloc(size);
}
extern "C" void* glibc_override_realloc(void *ptr, size_t size, const void *caller) 
{
    return frealloc(ptr, size);
}
extern "C" void glibc_override_free(void *ptr, const void *caller) 
{
    ffree(ptr);
}
extern "C" void* glibc_override_memalign(size_t align, size_t size, const void *caller) 
{
    return fmemalign(align, size);
}

static void my_init_hook(void)
{
    old_malloc_hook = __malloc_hook;
    __malloc_hook = glibc_override_malloc;
    
    old_realloc_hook = __realloc_hook;
    __realloc_hook = glibc_override_realloc;
    
    old_free_hook = __free_hook;
    __free_hook = glibc_override_free;
    
    old_memalign_hook = __memalign_hook;
    __memalign_hook = glibc_override_memalign;
}

#endif
#endif


