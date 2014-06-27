#pragma once

// 帮助
#define SAFE_TEST_RET_VAL(EXPRESSION, TESTVALUE, RETURNVALUE) if((EXPRESSION) == (TESTVALUE)) return RETURNVALUE;
#define SAFE_TEST_RET(EXPRESSION, TESTVALUE) if((EXPRESSION) == (TESTVALUE)) return;
#define SAFE_TEST_CONTINUE(EXPRESSION, TESTVALUE) if((EXPRESSION) == (TESTVALUE)) continue;
#define SAFE_TEST_BREAK(EXPRESSION, TESTVALUE) if((EXPRESSION) == (TESTVALUE)) break;

#define SAFE_TEST_INDEX(EXPRESSION, TESTVALUE) if((EXPRESSION) < 0 || ((int)EXPRESSION) >= ((int)TESTVALUE)) return;
#define SAFE_TEST_INDEX_VAL(EXPRESSION, TESTVALUE, RETURNVALUE) if((EXPRESSION) < 0 || ((int)EXPRESSION) >= ((int)TESTVALUE)) return RETURNVALUE;

#define SAFE_DIFFER_TEST_RET_VAL(EXPRESSION, TESTVALUE, RETURNVALUE) if((EXPRESSION) != (TESTVALUE)) return RETURNVALUE;
#define SAFE_DIFFER_TEST_RET(EXPRESSION, TESTVALUE) if((EXPRESSION) != (TESTVALUE)) return;
#define SAFE_DIFFER_TEST_CONTINUE(EXPRESSION, TESTVALUE) if((EXPRESSION) != (TESTVALUE)) continue;
#define SAFE_DIFFER_TEST_BREAK(EXPRESSION, TESTVALUE) if((EXPRESSION) != (TESTVALUE)) break;

#define SAFE_DELETE(T, p) if (p) { fdelete<T>(p); p = 0; }
#define SAFE_FREE(p) if (p) { FFREE(p); p = 0; }
#define SAFE_DELETE_ARRAY(T, p, num) if (p) { fdelete_array<T>(p, num); p = 0; }

// 断言
#ifdef USE_DEFAULT_ASSERT
	#define FASSERT(x) assert(x)
#endif

// 分配
#ifdef USE_FUCK_ALLOC
	#define FALLOC(size) falloc(size)
	#define FFREE(p) ffree(p)
#else
    #define FALLOC(size) sys_alloc(size)
	#define FFREE(p) sys_free(p)
#endif

// 内联
#ifdef USE_FORCE_INLINE
	#ifdef WIN32
		#define force_inline __forceinline
	#else
		#define force_inline __inline__ __attribute__((always_inline))
	#endif
#else
	#define force_inline
#endif

// 格式化
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

// socket
#ifdef WIN32
	#define GET_NET_ERROR WSAGetLastError()
	#define NET_BLOCK_ERROR WSAEWOULDBLOCK
	#define NET_BLOCK_ERROR_EX WSAEWOULDBLOCK
#else
	#define GET_NET_ERROR errno
	#define NET_BLOCK_ERROR EWOULDBLOCK
	#define NET_BLOCK_ERROR_EX EAGAIN
#endif

// printf
#ifdef WIN32
	#define F64d "I64d"
	#define F64u "I64u"
#else
	#define F64d "lld"
	#define F64u "llu"
#endif

// 数组大小
#define ARRAY_SIZE(a)	\
	((sizeof(a) / sizeof((*a))) /	\
	((size_t)!(sizeof(a) % sizeof((*a)))))

// 线程局部
#ifdef WIN32
	#define THREAD_LOCAL_VALUE __declspec(thread)
#else
	#define THREAD_LOCAL_VALUE __thread
#endif

#ifndef XML_STATIC
	#define XML_STATIC
#endif

// log
#ifdef WIN32
	#define FLOG(ftype, format, ...) flog_system::ptr()->write(ftype, __FILE__, __LINE__, __FUNCTION__, format, __VA_ARGS__)
	#define LOG_DEBUG(format, ...) FLOG(FLOGT_DEBUG, format, __VA_ARGS__)
	#define LOG_ERROR(format, ...) FLOG(FLOGT_ERROR, format, __VA_ARGS__)
	#define LOG_SYS(format, ...) FLOG(FLOGT_SYSTEM, format, __VA_ARGS__)
#else
	#define FLOG(ftype, format, ...) flog_system::ptr()->write(ftype, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
	#define LOG_DEBUG(format, ...) flog_system::ptr()->write(FLOGT_DEBUG, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
	#define LOG_ERROR(format, ...) flog_system::ptr()->write(FLOGT_ERROR, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
	#define LOG_SYS(format, ...) flog_system::ptr()->write(FLOGT_SYSTEM, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
#endif

// 去除警告
template <typename T>
void FUSE(T t) {}

// 大小端
#define F_LITTLE_ENDIAN   1234 /* byte 0 is least significant (i386) */
#define F_BIG_ENDIAN      4321 /* byte 0 is most significant (mc68k) */

#ifdef WIN32
#if (('1234' >> 24) == '1')
#  define FPLATFORM_BYTE_ORDER F_LITTLE_ENDIAN
#elif (('4321' >> 24) == '1')
#  define FPLATFORM_BYTE_ORDER F_BIG_ENDIAN
#endif
#else
// 有警告，手动指定吧
#  define FPLATFORM_BYTE_ORDER F_LITTLE_ENDIAN
#endif

#if !defined(FPLATFORM_BYTE_ORDER)
#  error Please set undetermined byte order.
#endif

// 打印TODO
#ifndef WIN32 
#define DO_PRAGMA(x) _Pragma (#x)
#define TODO(x) DO_PRAGMA(message ("TODO - " #x))
#else
#define TODO(x)
#endif

// hook
#ifdef WIN32
#define JMP_CODE_LEN 5
#else
#define JMP_CODE_LEN 12
#endif

