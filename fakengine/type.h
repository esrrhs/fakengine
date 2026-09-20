#pragma once

#if defined(WIN32)
typedef DWORD t_id;
#else
typedef pthread_t t_id;
#endif

typedef	int32_t socket_t;
#if defined(WIN32)
#if !defined(__MINGW32__) && !defined(_WS2TCPIP_H)
typedef int32_t socklen_t;
#endif
#else
#endif

#if defined(WIN32)
typedef DWORD tls_key;
#else
typedef pthread_key_t tls_key;
#endif

#if defined(WIN32)
typedef void* shm_handle;
typedef int shm_key;
#define INVALID_SHM_HANDLE ((shm_handle)0)
#define IS_VALID_SHM_HANDLE(h) ((h) != NULL)
#else
typedef int shm_handle;
typedef key_t shm_key;
#define INVALID_SHM_HANDLE (-1)
#define IS_VALID_SHM_HANDLE(h) ((h) >= 0)
#endif

union marshall_float 
{
	uint32_t i;
	float f;
};

union marshall_poiner
{
	uint32_t i;
	void * p;
};

