#pragma once

#ifdef WIN32
template <uint32_t N>
class epollor
{
public:
	force_inline epollor()
	{
	}
	force_inline ~epollor()
	{
	}
public:
	force_inline bool add(socket_t s, void * p)
	{
		m_epollormap[s] = p;
		return true;
	}
	force_inline bool del(socket_t s)
	{
		m_epollormap.erase(s);
		return true;
	}
	force_inline bool select()
	{
		fd_set readfds;
		fd_set writefds;
		fd_set exceptfds;
		socket_t maxfd = -1;
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_ZERO(&exceptfds);

		for (_epollormap_iter it = m_epollormap.begin(); it != m_epollormap.end(); it++)
		{
			socket_t s = it->first;
			FD_SET(s, &readfds);
			FD_SET(s, &writefds);
			FD_SET(s, &exceptfds);
			maxfd = Max<socket_t>(maxfd, s + 1);
		}

		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;

		int ret = ::select(maxfd + 1,
			&readfds,
			&writefds,
			&exceptfds,
			&timeout);

		if (ret <= 0)
		{
			m_eventsnum = 0;
			return false;
		}

		m_eventsnum = 0;
		for (_epollormap_iter it = m_epollormap.begin(); it != m_epollormap.end(); it++)
		{
			int flag = 0;
			socket_t s = it->first;
			void * p = it->second;
			if (FD_ISSET(s, &readfds))
			{
				flag |= FSOCKET_READ;
			}
			if (FD_ISSET(s, &writefds))
			{
				flag |= FSOCKET_WRITE;
			}
			if (FD_ISSET(s, &exceptfds))
			{
				flag |= FSOCKET_ERROR;
			}
			if (flag != 0)
			{
				m_flag[m_eventsnum] = flag;
				m_pointer[m_eventsnum] = p;
				m_eventsnum++;
			}
		}
		return true;
	}
	force_inline int32_t event_size() const
	{
		return m_eventsnum;
	}
	force_inline void get_event(int32_t i, int32_t & flag, void * & p)
	{
		FASSERT(i >= 0 && i < m_eventsnum);
		flag = m_flag[i];
		p = m_pointer[i];
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
	typedef fhashmap<socket_t, void *, N> _epollormap;
	typedef typename _epollormap::iterator _epollormap_iter;
	_epollormap m_epollormap;
	int32_t m_eventsnum;
	int32_t m_flag[N];
	void * m_pointer[N];
};

#else
template <uint32_t N>
class epollor
{
public:
	force_inline epollor()
	{
        m_epollfd = epoll_create(N);
	}
	force_inline ~epollor()
	{
		close(m_epollfd);
	}
public:
	force_inline bool add(socket_t s, void * p)
	{
		epoll_event ev;
		ev.events = EPOLLIN | EPOLLOUT | EPOLLERR;
		ev.data.fd = s;
		ev.data.ptr = p;
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
		m_eventsnum = epoll_wait(m_epollfd, m_events, N, 5);

		if (m_eventsnum < 0)
		{
			m_eventsnum = 0;
			return false;
		}

		return true;
	}
	force_inline int32_t event_size() const
	{
		return m_eventsnum;
	}
	force_inline void get_event(int32_t i, int32_t & flag, void * & p)
	{
		FASSERT(i >= 0 && i < m_eventsnum);
		flag = m_events[i].events;
		p = m_events[i].data.ptr;
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
	int32_t m_eventsnum;
};

#endif

