#pragma once

template <typename _queue>
class tcpsocket
{
public:
	tcpsocket() : m_send_slot(&(_queue::write), &m_send_queue),
		m_recv_slot(&(_queue::read), &m_recv_queue)
	{
	}
	~tcpsocket()
	{
	}
public:
	template<typename _msg>
	FORCEINLINE bool send(const _msg * msg)
	{
		size_t write_size = 0;
		if (msg->to_buffer(m_send_slot, write_size))
		{
			m_send_queue.skip_read(write_size);
			return true;
		}
		return false;
	}
	template<typename _msg>
	FORCEINLINE bool recv(_msg * msg)
	{
		size_t read_size = 0;
		if (msg->from_buffer(m_recv_slot, read_size))
		{
			m_recv_queue.skip_write(read_size);
			return true;
		}
		return false;
	}
	FORCEINLINE bool ini()
	{
		return true;
	}
	FORCEINLINE void tick()
	{
		if (connected())
		{
			if (can_write())
			{
				flush();
			}
			if (can_read())
			{
				fill();
			}
		}
		else
		{
			reconnect();
		}
	}
	FORCEINLINE bool flush()
	{
		return true;
	}
	FORCEINLINE bool fill()
	{
		return true;
	}
	FORCEINLINE bool can_read()
	{
		return true;
	}
	FORCEINLINE bool can_write()
	{
		return true;
	}
	FORCEINLINE bool connected()
	{
		return true;
	}
	FORCEINLINE bool reconnect()
	{
		return true;
	}
public:
	static FORCEINLINE socket_t socket(int32_t domain, int32_t type, int32_t protocol)
	{
		return ::socket(domain, type, protocol);
	}
	static FORCEINLINE int32_t bind(socket_t s, const struct sockaddr * name, uint32_t namelen)
	{
		return ::bind(s, name, namelen);
	}
	static FORCEINLINE int32_t connect(socket_t s, const struct sockaddr* name, uint32_t namelen)
	{
		return ::connect(s, name, namelen);
	}
	static FORCEINLINE int32_t listen(socket_t s, uint32_t backlog) 
	{
		return ::listen(s, backlog);
	}
	static FORCEINLINE socket_t accept(socket_t s, struct sockaddr * name, int32_t * addrlen) 
	{
		return ::accept(s, name, addrlen);
	}
	static FORCEINLINE int32_t getsockopt(socket_t s, int32_t level, int32_t optname, void * optval, socklen_t * optlen) 
	{
		return ::getsockopt(s, level, optname, optval, optlen);
	}
	static FORCEINLINE int32_t setsockopt(socket_t s, int32_t level, int32_t optname, const void * optval, socklen_t optlen) 
	{
		return ::setsockopt(s, level, optname, optval, optlen);
	}
	static FORCEINLINE int32_t send(socket_t s, const void * buf, int32_t len, int32_t flags) 
	{
		return ::send(s, buf, len, flags);
	}
	static FORCEINLINE int32_t recv(socket_t s, void * buf, int32_t len, int32_t flags) 
	{
		return ::recv(s, buf, len, flags);
	}
	static FORCEINLINE int32_t close_socket(socket_t s)
	{
#if defined(WIN32)
		return ::closesocket(s);
#else
		return ::close(s);
#endif
	}
	static FORCEINLINE bool set_socket_nonblocking(socket_t s, bool on) 
	{
#if defined(WIN32)
		return ioctlsocket(s, FIONBIO, (unsigned long*)&on) == SOCKET_ERROR ? false : true;
#else
		int32_t opts;
		opts = fcntl (s, F_GETFL, 0);

		if (opts < 0)
		{
			return false;
		}

		if (on)
			opts = (opts | O_NONBLOCK);
		else
			opts = (opts & ~O_NONBLOCK);

		fcntl(s, F_SETFL, opts);
		return opts < 0 ? false : true;
#endif
	}
	static FORCEINLINE int32_t select(int32_t maxfdp1, fd_set * readset, fd_set * writeset, fd_set * exceptset, struct timeval* timeout) 
	{
		return ::select(maxfdp1, readset, writeset, exceptset, timeout);
	}
private:
	socket_t m_socket;
	_queue m_send_queue;
	_queue m_recv_queue;
	slot<_queue, bool (_queue::*)(const int8_t * p, size_t size)> m_send_slot;
	slot<_queue, bool (_queue::*)(int8_t * out, size_t size)> m_recv_slot;
};

