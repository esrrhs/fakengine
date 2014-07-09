#pragma once

template <typename Link, size_t N>
class fproxyclient
{
public:
	static const int32_t INVALID_IDX = -1;
public:
    fproxyclient()
    {
        m_LastLink = 0;
    }
    ~fproxyclient()
    {
    }

    template<typename _param>
	force_inline bool ini(const _param & param)
	{
	    bool ret;
	    for (int32_t i = 0; i < (int32_t)N; i++)
	    {
	        ret &= m_Link[i].ini(param);
	    }
		return ret;
	}
	
	force_inline void tick()
	{
	    for (int32_t i = 0; i < (int32_t)N; i++)
	    {
	        m_Link[i].tick();
	    }
	}
	
    template<typename _msg>
	force_inline bool send_msg(const _msg & msg)
	{
	    int32_t i = choose_link();
	    SAFE_TEST_RET_VAL(i, INVALID_IDX, false);
	    m_Link[i].send_msg(msg)
		return false;
	}
	
private:
    force_inline int32_t choose_link()
    {
	    for (int32_t i = 0; i < (int32_t)N; i++)
	    {
	        if (m_Link[i].isconnect())
	        {
	            return i;
	        }
	        m_LastLink++;
	        if (m_LastLink >= N)
	        {
	            m_LastLink = N - 1;
	        }
	    }
        return INVALID_IDX;
    }
private:
    Link m_Link[N];
    int32_t m_LastLink;
};

