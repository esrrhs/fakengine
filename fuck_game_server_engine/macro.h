#pragma once

#define SAFE_TEST_RET_VAL(EXPRESSION, TESTVALUE, RETURNVALUE) if((EXPRESSION) == (TESTVALUE)) return RETURNVALUE;
#define SAFE_TEST_RET(EXPRESSION, TESTVALUE) if((EXPRESSION) == (TESTVALUE)) return;
#define SAFE_TEST_CONTINUE(EXPRESSION, TESTVALUE) if((EXPRESSION) == (TESTVALUE)) continue;
#define SAFE_TEST_BREAK(EXPRESSION, TESTVALUE) if((EXPRESSION) == (TESTVALUE)) break;

#define SAFE_DIFFER_TEST_RET_VAL(EXPRESSION, TESTVALUE, RETURNVALUE) if((EXPRESSION) != (TESTVALUE)) return RETURNVALUE;
#define SAFE_DIFFER_TEST_RET(EXPRESSION, TESTVALUE) if((EXPRESSION) != (TESTVALUE)) return;
#define SAFE_DIFFER_TEST_CONTINUE(EXPRESSION, TESTVALUE) if((EXPRESSION) != (TESTVALUE)) continue;
#define SAFE_DIFFER_TEST_BREAK(EXPRESSION, TESTVALUE) if((EXPRESSION) != (TESTVALUE)) break;

#define SAFE_DELETE(T, p) if (p) { fdelete<T>(p); p = 0; }
#define SAFE_FREE(p) if (p) { FFREE(p); p = 0; }

#ifdef USE_DEFAULT_ASSERT
	#define FASSERT(x) assert(x)
#endif

#ifdef USE_DEFAULT_ALLOC
	#define FALLOC(size) sys_alloc(size)
	#define FFREE(p) sys_free(p)
#endif

#ifdef USE_FORCE_INLINE
	#ifdef WIN32
		#define force_inline __forceinline
	#else
		#define force_inline __inline__ __attribute__((always_inline))
	#endif
#else
	#define force_inline
#endif

#if defined(WIN32)
	#define tvsnprintf  _vsnprintf
	#define tstricmp    _stricmp
	#define tsnprintf   _snprintf
#else
	#define tvsnprintf  vsnprintf
	#define tstricmp    strcasecmp
	#define tsnprintf   snprintf
#endif

#ifdef USE_DEFAULT_PRINTF
	#define FPRINTF printf
#endif

#ifdef WIN32
	#define GET_NET_ERROR WSAGetLastError()
	#define NET_BLOCK_ERROR WSAEWOULDBLOCK
	#define NET_BLOCK_ERROR_EX WSAEWOULDBLOCK
#else
	#define GET_NET_ERROR errno
	#define NET_BLOCK_ERROR EWOULDBLOCK
	#define NET_BLOCK_ERROR_EX EAGAIN
#endif

#ifdef WIN32
	#define F64d "I64d"
#else
	#define F64d "lld"
#endif

#define ARRAY_SIZE(a)	\
	((sizeof(a) / sizeof((*a))) /	\
	((size_t)!(sizeof(a) % sizeof((*a)))))

#ifdef WIN32
	#define THREAD_LOCAL_VALUE __declspec(thread)
#else
	#define THREAD_LOCAL_VALUE __thread
#endif

#ifdef USE_PROFILE
	#define	PERF(manager, name) fperf_sample _fperf_sample((manager), (name))
	#define PERF_FUNC(manager) PERF(manager, __FUNCTION__)
	#define PERF_DEFAULT(name) PERF(g_fperf_manager, name)
	#define PERF_DEFAULT_FUNC() PERF(g_fperf_manager, __FUNCTION__)
	#define PERF_INI() g_perf_ini()
	#define PERF_EXIT() g_perf_exit()
	#define PERF_OUTPUT() g_perf_output()
#else
	#define	PERF(manager, name)
	#define PERF_FUNC(manager)
	#define PERF_DEFAULT(name)
	#define PERF_DEFAULT_FUNC()
	#define	PERF_INI()
	#define PERF_EXIT()
	#define PERF_OUTPUT()
#endif

#ifndef XML_STATIC
#define XML_STATIC
#endif
