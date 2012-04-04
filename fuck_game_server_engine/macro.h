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
#else
	#define FASSERT(x) 
#endif

#ifdef USE_DEFAULT_ALLOC
	#define FALLOC(size) sys_alloc(size)
	#define FFREE(p) sys_free(p)
#endif

// TODO:Ìí¼Óforceinlineºê
#ifdef USE_FORCE_INLINE
	#ifdef WIN32
		#define fore_inline __forceinline
	#else
		#define fore_inline
	#endif
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
