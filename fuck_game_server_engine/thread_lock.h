#pragma once

#if defined(WIN32)
class thread_lock
{
	CRITICAL_SECTION m_lock ;
public :
	thread_lock() { InitializeCriticalSection(&m_lock); }
	~thread_lock() { DeleteCriticalSection(&m_lock); }
	force_inline void lock() { EnterCriticalSection(&m_lock); }
	force_inline void unlock() { LeaveCriticalSection(&m_lock); }
};
#else
class thread_lock
{
	pthread_mutex_t m_mutex; 
public :
	thread_lock() { pthread_mutex_init( &m_mutex , NULL); }
	~thread_lock() { pthread_mutex_destroy( &m_mutex); }
	force_inline void lock() { pthread_mutex_lock(&m_mutex); }
	force_inline void unlock() { pthread_mutex_unlock(&m_mutex); }
};
#endif



