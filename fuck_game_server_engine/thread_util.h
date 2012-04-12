#pragma once

static force_inline void fsleep(int32_t millionseconds)
{
#if defined(WIN32)
	Sleep(millionseconds);
#else
	usleep(millionseconds * 1000);
#endif
}

static force_inline t_id fget_current_thread_id()
{
#if defined(WIN32)
	return GetCurrentThreadId();
#else
	return pthread_self();
#endif
}

