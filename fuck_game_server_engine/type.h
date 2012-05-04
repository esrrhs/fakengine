#pragma once

#if defined(WIN32)
typedef DWORD t_id;
#else
typedef pthread_t t_id;
#endif

typedef	int32_t socket_t;
#if defined(WIN32)
typedef int32_t socklen_t;
#else
#endif

#if defined(WIN32)
typedef DWORD tls_key;
#else
typedef pthread_key_t tls_key;
#endif


