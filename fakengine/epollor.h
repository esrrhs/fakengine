#pragma once

#ifndef WIN32
template <uint32_t N>
class epollor
{
public:
	force_inline epollor()
	{
        m_epollfd = epoll_create(N);
	    clear();
	}
	force_inline ~epollor()
	{
		close(m_epollfd);
	}
public:
	force_inline bool clear()
	{
		m_pollmap.clear();
		return true;
	}
	force_inline bool add(socket_t s)
	{
		epoll_event ev;
		ev.events = EPOLLIN | EPOLLOUT | EPOLLERR;
		ev.data.fd = s;
		if (epoll_ctl(m_epollfd, EPOLL_CTL_ADD, s, &ev) == -1) 
		{
		    FASSERT(0);
    		return false;
		}

		return true;
	}
	force_inline bool del(socket_t s)
	{
		if (epoll_ctl(m_epollfd, EPOLL_CTL_DEL, s, 0) == -1) 
		{
		    FASSERT(0);
    		return false;
		}

		return true;
	}
	force_inline bool select()
	{
		int32_t ret = epoll_wait(m_epollfd, m_events, N, 1);

		if (ret < 0)
		{
			return false;
		}
		
		for (int32_t i = 0; i < ret; i++)
		{
			m_pollmap[m_events[i].data.fd] = i;
		}
		
		return false;
	}
	force_inline bool is_read(socket_t s)
	{
		return m_events[m_pollmap[s]].events & EPOLLIN;
	}
	force_inline bool is_write(socket_t s)
	{
		return m_events[m_pollmap[s]].events & EPOLLOUT;
	}
	force_inline bool is_except(socket_t s)
	{
		return m_events[m_pollmap[s]].events & EPOLLERR;
	}
private:
    force_inline epollor(const epollor<N> & r)
    {
    }
    force_inline epollor<N> & operator =(const epollor<N> & r)
    {
		return *this;
    }
private:
	int32_t m_epollfd;
	epoll_event m_events[N];
	typedef fhashmap<socket_t, int32_t, N> _pollmap;
	_pollmap m_pollmap;
};

#endif

