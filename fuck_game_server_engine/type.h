#pragma once

#if defined(WIN32)
typedef DWORD t_id;
#else
typedef pthread_t t_id;
#endif

typedef	int32_t socket_t;

