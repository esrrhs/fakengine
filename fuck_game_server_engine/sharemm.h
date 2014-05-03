#pragma once

// 创建ShareMem 内存区
static force_inline shm_handle create_share_mem(shm_key key,size_t size)
{
#if defined(WIN32)
	char keybuf[64] = {0};
	tsnprintf(keybuf, sizeof(keybuf) - 1, "%d", key);
	return CreateFileMapping( (HANDLE)0xFFFFFFFFFFFFFFFF, NULL, PAGE_READWRITE, 0, size, keybuf);
	
#elif
	return shmget(key, size, IPC_CREAT|IPC_EXCL|0666);

#endif

	return shm_handle(-1);
}

// 打开ShareMem 内存区
static force_inline shm_handle open_share_mem(shm_key key,size_t size)
{
#if defined(WIN32)
	char keybuf[64] = {0};
	tsnprintf(keybuf, sizeof(keybuf) - 1, "%d", key);
	return OpenFileMapping( FILE_MAP_ALL_ACCESS, TRUE, keybuf);

#elif
	return shmget(key, size, 0);

#endif

	return shm_handle(-1);
}
	
// 映射ShareMem 内存区
static force_inline int8_t* map_share_mem(shm_handle handle)
{
#if defined(WIN32)
	return (int8_t *)MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);

#elif
	return (int8_t*)shmat(handle, 0, 0);

#endif

	return 0;
}
	
// 关闭映射 ShareMem 内存区
static force_inline void unmap_share_mem(int8_t* memoryptr)
{
#if defined(WIN32)
	 UnmapViewOfFile(memoryptr);

#elif
	shmdt(memoryptr);

#endif
}
	
// 	关闭ShareMem
static force_inline void close_share_mem(shm_handle handle)
{
#if defined(WIN32)
	CloseHandle(handle);

#elif
	shmctl(handle, IPC_RMID, 0); 

#endif
}
