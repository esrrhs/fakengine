#pragma once

class fclock : public singleton< fclock >
{
public:
	fclock()
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
private:
	force_inline void tick()
	{
		time(&m_now);
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
	tm m_nowtm;
	int8_t m_nowstr[c_ClockStringBuffer];
};

