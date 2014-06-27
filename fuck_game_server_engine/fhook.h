#pragma once

#ifdef WIN32
static force_inline bool fhook_func(uint8_t * target_function, uint8_t * newfuc, uint8_t (&old_mem)[JMP_CODE_LEN])
{
	DWORD old_target_function_protect = 0;  
	BOOL succeeded = ::VirtualProtect(reinterpret_cast<void*>(target_function),  
		32,  
		PAGE_EXECUTE_READWRITE,  
		&old_target_function_protect);  

	if (!succeeded) {  
	    FASSERT(0);
		return false;  
	}  

	// save
	memcpy(old_mem, target_function, JMP_CODE_LEN);

	// hook
	*target_function = 0xE9;  
	int offset = (uint8_t *)&newfuc - target_function - JMP_CODE_LEN;  
	memcpy(target_function + 1, &offset, 4);  

	succeeded = ::VirtualProtect(reinterpret_cast<void*>(target_function),  
		32,  
		old_target_function_protect,  
		&old_target_function_protect);  

	if (!succeeded) {  
	    FASSERT(0);
		return false;  
	}  

	return true;
}

static force_inline bool frestore_func(uint8_t * target_function, uint8_t (&old_mem)[JMP_CODE_LEN])
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
	memcpy(target_function, old_mem, JMP_CODE_LEN);

	succeeded = ::VirtualProtect(reinterpret_cast<void*>(target_function),  
		32,  
		old_target_function_protect,  
		&old_target_function_protect);  

	if (!succeeded) {  
		return false;  
	}  

	return true;
}

#else

static force_inline bool fhook_func(uint8_t * target_function, uint8_t * newfuc, uint8_t (&old_mem)[JMP_CODE_LEN])
{
	uint8_t machine_code[JMP_CODE_LEN] = {
			//movq $0x0, %rax 后面8个字节的0为64位立即数
			0x48, 0xb8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
			//jmpq *%rax
			0xff, 0xe0
		};

	int pagesize = getpagesize();

	uint8_t *mem = (uint8_t *) target_function;
	void *p = (uint8_t*) (mem - ((uint64_t) mem % pagesize));
	if (mprotect(p, pagesize, PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect error");
	    FASSERT(0);
		return false;
	}

	// save
	memcpy(old_mem, mem, JMP_CODE_LEN);
	
	//改写立即数为mock的地址,写入函数入口处
	memcpy(machine_code + 2, &newfuc, sizeof(newfuc));
	memcpy(mem, machine_code, sizeof(machine_code));

	if (mprotect(p, pagesize, PROT_EXEC))
	{
		perror("mprotect error");
	    FASSERT(0);
		return false;
	}

	return true;
}

static force_inline bool frestore_func(uint8_t * target_function, uint8_t (&old_mem)[JMP_CODE_LEN])
{
	int pagesize = getpagesize();

	uint8_t *mem = (uint8_t *) target_function;
	void *p = (uint8_t*) (mem - ((uint64_t) mem % pagesize));
	if (mprotect(p, pagesize, PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect error");
	    FASSERT(0);
		return false;
	}

	memcpy(mem, old_mem, JMP_CODE_LEN);

	if (mprotect(p, pagesize, PROT_EXEC))
	{
		perror("mprotect error");
	    FASSERT(0);
		return false;
	}

	return true;
}

#endif


static force_inline bool fhook_all_func()
{
	fhook_func((uint8_t*)&malloc, (uint8_t*)falloc, g_old_malloc_hook_mem);
	fhook_func((uint8_t*)&free, (uint8_t*)ffree, g_old_free_hook_mem);
	fhook_func((uint8_t*)&realloc, (uint8_t*)frealloc, g_old_realloc_hook_mem);
#ifndef WIN32
	fhook_func((uint8_t*)&memalign, (uint8_t*)fmemalign, g_old_memalign_hook_mem);
#endif
    return true;
}

static force_inline bool frestore_all_func()
{
	frestore_func((uint8_t*)&malloc, g_old_malloc_hook_mem);
	frestore_func((uint8_t*)&free, g_old_free_hook_mem);
	frestore_func((uint8_t*)&realloc, g_old_realloc_hook_mem);
#ifndef WIN32
	frestore_func((uint8_t*)&memalign, g_old_memalign_hook_mem);
#endif
    return true;
}


