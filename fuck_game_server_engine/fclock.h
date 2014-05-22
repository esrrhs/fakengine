#pragma once

class fclock : public singleton< fclock >
{
public:
	fclock() : m_last(0)
	{
		tick();
	}
	~fclock()
	{
	}
	force_inline time_t now()
	{
		return m_now;
	}
	force_inline tm nowtm()
	{
		return m_nowtm;
	}
	force_inline const int8_t * nowstr()
	{
		return m_nowstr;
	}
	force_inline void heartbeat()
	{
		tick();
	}
	static force_inline uint32_t Cycles()
	{
#ifdef WIN32        
		LARGE_INTEGER Cycles;
		QueryPerformanceCounter(&Cycles);
		return Cycles.QuadPart;
#else
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);
		return static_cast<uint32_t>(static_cast<uint64_t>(ts.tv_sec) * 1000000ULL + static_cast<uint64_t>(ts.tv_nsec) / 1000ULL);
#endif
	}
private:
	force_inline void tick()
	{
		time(&m_now);
		if (m_last == m_now)
		{
		    return;
		}
		m_last = m_now;
		m_nowtm = *gmtime(&m_now);
		tsnprintf((char *)m_nowstr, ARRAY_SIZE(m_nowstr) - 1, 
			"%d-%d-%d,%d:%d:%d", 
			m_nowtm.tm_year + 1900, 
			m_nowtm.tm_mon + 1, 
			m_nowtm.tm_mday, 
			m_nowtm.tm_hour, 
			m_nowtm.tm_min, 
			m_nowtm.tm_sec);
	}
private:
	time_t m_now;
	time_t m_last;
	tm m_nowtm;
	int8_t m_nowstr[c_ClockStringBuffer];
};

