#pragma once

force_inline time_t get_s_tick()
{
	return ::time(0);
}

force_inline uint32_t get_ms_tick()
{
#ifdef WIN32
	return ::GetTickCount();
#else
	struct timeval tv;
	if(::gettimeofday(&tv, 0) == 0)
	{
		return tv.tv_sec * 1000 + tv.tv_sec;
	}
	return 0;
#endif
}

force_inline uint64_t get_ns_tick()
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

