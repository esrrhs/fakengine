#pragma once

static force_inline time_t get_s_tick()
{
	return ::time(0);
}

static force_inline uint32_t get_ms_tick()
{
#ifdef WIN32
	return ::GetTickCount();
#else
	struct timeval tv;
	if(::gettimeofday(&tv, 0) == 0)
	{
		uint64_t t = tv.tv_sec * 1000;
		t += tv.tv_usec / 1000;
		return t &  0xffffffff;
	}
	return 0;
#endif
}

static force_inline uint64_t get_ns_tick()
{
#ifdef WIN32
	uint32_t dwLow, dwHigh;
	__asm
	{
		rdtsc
		mov dwLow, eax;
		mov dwHigh, edx;
	}
	return ((uint64_t)dwHigh << 32) | (uint64_t)dwLow;
#else
	uint32_t dwLow, dwHigh;
	__asm__ __volatile__("rdtsc" : "=a" (dwLow), "=d" (dwHigh));
	return ((uint64_t)dwHigh << 32) | (uint64_t)dwLow;
#endif
}

