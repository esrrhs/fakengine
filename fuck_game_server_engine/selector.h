#pragma once

class selector
{
public:
	force_inline selector()
	{
		clear();
	}
	force_inline ~selector()
	{

	}
public:
	force_inline bool clear()
	{
		FD_ZERO(&m_readfds);
		FD_ZERO(&m_writefds);
		FD_ZERO(&m_exceptfds);
		m_maxfd = -1;
		return true;
	}
	force_inline bool add(socket_t s)
	{
		FD_SET(s, &m_readfds);
		FD_SET(s, &m_writefds);
		FD_SET(s, &m_exceptfds);
		m_maxfd = Max<socket_t>(m_maxfd, s);
		return true;
	}
	force_inline bool select()
	{
		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;

		int32_t ret = ::select(m_maxfd + 1, 
			&m_readfds,
			&m_writefds,
			&m_exceptfds,
			&timeout);

		if (ret != -1)
		{
			return true;
		}
		return false;
	}
	force_inline bool is_read(socket_t s)
	{
		return FD_ISSET(s, &m_readfds) != 0;
	}
	force_inline bool is_write(socket_t s)
	{
		return FD_ISSET(s, &m_writefds) != 0;
	}
	force_inline bool is_except(socket_t s)
	{
		return FD_ISSET(s, &m_exceptfds) != 0;
	}
private:
	fd_set m_readfds;
	fd_set m_writefds;
	fd_set m_exceptfds;
	socket_t m_maxfd;
};

