#pragma once

static force_inline bool ftls_alloc(tls_key & key)
{
#ifdef WIN32
	key = ::TlsAlloc();
	return key != TLS_OUT_OF_INDEXES;
#else
	return pthread_key_create(&key, 0) == 0;
#endif
}

static force_inline void ftls_free(tls_key key)
{
#ifdef WIN32
	::TlsFree(key);
#else
	pthread_key_delete(key);
#endif
}

static force_inline void ftls_set(tls_key key, const void * data)
{
#ifdef WIN32
	::TlsSetValue(key, (LPVOID)data);
#else
	pthread_setspecific(key, data);
#endif
}

static force_inline void * ftls_get(tls_key key)
{
#ifdef WIN32
	return ::TlsGetValue(key);
#else
	return pthread_getspecific(key);
#endif
}

