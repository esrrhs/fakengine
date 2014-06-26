#include "fuck_game_server_engine.h"

#ifdef USE_ALLOC_HOOK
#ifdef WIN32

bool hook_sys_func(uint8_t * target_function, uint8_t * newfuc, uint8_t (&old_mem)[5])
{
	DWORD old_target_function_protect = 0;  
	BOOL succeeded = ::VirtualProtect(reinterpret_cast<void*>(target_function),  
		32,  
		PAGE_EXECUTE_READWRITE,  
		&old_target_function_protect);  

	if (!succeeded) {  
		return false;  
	}  

	// save
	memcpy(old_mem, target_function, 5);

	// hook
	*target_function = 0xE9;  
	int offset = newfuc - target_function - 5;  
	memcpy(target_function + 1, &offset, 4);  

	succeeded = ::VirtualProtect(reinterpret_cast<void*>(target_function),  
		32,  
		old_target_function_protect,  
		&old_target_function_protect);  

	if (!succeeded) {  
		return false;  
	}  

	return true;
}

bool restore_sys_func(uint8_t * target_function, uint8_t (&old_mem)[5])
{
	DWORD old_target_function_protect = 0;  
	BOOL succeeded = ::VirtualProtect(reinterpret_cast<void*>(target_function),  
		32,  
		PAGE_EXECUTE_READWRITE,  
		&old_target_function_protect);  

	if (!succeeded) {  
		return false;  
	}  

	// save
	memcpy(target_function, old_mem, 5);

	succeeded = ::VirtualProtect(reinterpret_cast<void*>(target_function),  
		32,  
		old_target_function_protect,  
		&old_target_function_protect);  

	if (!succeeded) {  
		return false;  
	}  

	return true;
}

static uint8_t old_malloc_hook_mem[5];
static uint8_t old_free_hook_mem[5];
static uint8_t old_realloc_hook_mem[5];

struct malloc_hook_entry
{
	malloc_hook_entry()
	{
		hook_sys_func((uint8_t*)&malloc, (uint8_t*)&falloc, old_malloc_hook_mem);
		hook_sys_func((uint8_t*)&free, (uint8_t*)&ffree, old_free_hook_mem);
		hook_sys_func((uint8_t*)&realloc, (uint8_t*)&frealloc, old_realloc_hook_mem);
	}
	~malloc_hook_entry()
	{
		restore_sys_func((uint8_t*)&malloc, old_malloc_hook_mem);
		restore_sys_func((uint8_t*)&free, old_free_hook_mem);
		restore_sys_func((uint8_t*)&realloc, old_realloc_hook_mem);
	}
};

static malloc_hook_entry hook_entry;

#else

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
#endif

#ifdef WIN32
extern "C" void * sys_alloc(size_t size)
{
#ifndef USE_ALLOC_HOOK
	return malloc(size);
#else

	restore_sys_func((uint8_t*)&malloc, old_malloc_hook_mem);
	restore_sys_func((uint8_t*)&free, old_free_hook_mem);
	restore_sys_func((uint8_t*)&realloc, old_realloc_hook_mem);

	void * ret = malloc(size);

	hook_sys_func((uint8_t*)&malloc, (uint8_t*)&falloc, old_malloc_hook_mem);
	hook_sys_func((uint8_t*)&free, (uint8_t*)&ffree, old_free_hook_mem);
	hook_sys_func((uint8_t*)&realloc, (uint8_t*)&frealloc, old_realloc_hook_mem);

	return ret;

#endif
}

extern "C" void sys_free(void * p, size_t size)
{
#ifndef USE_ALLOC_HOOK
	FASSERT(p);
	free(p);
#else

	restore_sys_func((uint8_t*)&malloc, old_malloc_hook_mem);
	restore_sys_func((uint8_t*)&free, old_free_hook_mem);
	restore_sys_func((uint8_t*)&realloc, old_realloc_hook_mem);

	free(p);

	hook_sys_func((uint8_t*)&malloc, (uint8_t*)&falloc, old_malloc_hook_mem);
	hook_sys_func((uint8_t*)&free, (uint8_t*)&ffree, old_free_hook_mem);
	hook_sys_func((uint8_t*)&realloc, (uint8_t*)&frealloc, old_realloc_hook_mem);

#endif
}
#else
extern "C" void * sys_alloc(size_t size)
{
#ifndef USE_ALLOC_HOOK
	return malloc(size);
#else
	void *result = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    return result;
#endif
}

extern "C" void sys_free(void * p, size_t size)
{
#ifndef USE_ALLOC_HOOK
	FASSERT(p);
	free(p);
#else
    munmap(p, size);
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


