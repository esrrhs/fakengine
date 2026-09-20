#pragma once

#include <chrono>
#include <ctime>

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
		return (const int8_t *)m_nowstr.c_str();
	}
	force_inline const int8_t * nowdatestr()
	{
		return (const int8_t *)m_nowdatestr.c_str();
	}
	force_inline void heartbeat()
	{
		tick();
	}
	static force_inline uint32_t Cycles()
	{
		using namespace std::chrono;
		return static_cast<uint32_t>(
			steady_clock::now().time_since_epoch().count());
	}
private:
	force_inline void tick()
	{
		m_now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		if (m_last == m_now)
		{
		    return;
		}
		m_last = m_now;
		m_nowtm = *gmtime(&m_now);
		m_nowstr.snprintf("%d-%d-%d,%d:%d:%d", 
			m_nowtm.tm_year + 1900, 
			m_nowtm.tm_mon + 1, 
			m_nowtm.tm_mday, 
			m_nowtm.tm_hour, 
			m_nowtm.tm_min, 
			m_nowtm.tm_sec);
		m_nowdatestr.snprintf("%d-%d-%d",
			m_nowtm.tm_year + 1900,
			m_nowtm.tm_mon + 1,
			m_nowtm.tm_mday);
	}
private:
	time_t m_now;
	time_t m_last;
	tm m_nowtm;
	stringc m_nowstr;
	stringc m_nowdatestr;
};
