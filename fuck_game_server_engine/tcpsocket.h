#pragma once

template <typename _queue>
class tcpsocket
{
public:
	tcpsocket() : m_send_slot(&(_queue::write), &m_send_queue),
		m_recv_slot(&(_queue::read), &m_recv_queue)
	{
		clear();
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
	FORCEINLINE bool ini(const net_link_param & param)
	{
		// close
		close();
		clear();

		const tcp_socket_link_param & tparam = (const tcp_socket_link_param &)param;
		fstrcopy(m_peer_ip, (const int8_t *)tparam.ip.c_str(), sizeof(m_peer_ip));
		m_peer_port = tparam.port;
		m_is_non_blocking = tparam.is_non_blocking;
		m_socket_send_buffer_size = tparam.socket_send_buffer_size;
		m_socket_recv_buffer_size = tparam.socket_recv_buffer_size;

		FPRINTF("tcpsocket::ini client [%s:%d]\n", m_peer_ip, m_peer_port);

		// reconnect
		if (!reconnect())
		{
			return false;
		}

		FPRINTF("tcpsocket::ini client [%s:%d] ok...\n", m_peer_ip, m_peer_port);

		return true;
	}
	FORCEINLINE bool ini(const net_server_param & param)
	{
		// close
		close();
		clear();

		const tcp_socket_server_param & tparam = (const tcp_socket_server_param &)param;
		fstrcopy(m_ip, (const int8_t *)tparam.ip.c_str(), sizeof(m_ip));
		m_port = tparam.port;
		m_is_non_blocking = tparam.is_non_blocking;
		m_socket_send_buffer_size = tparam.socket_send_buffer_size;
		m_socket_recv_buffer_size = tparam.socket_recv_buffer_size;

		FPRINTF("tcpsocket::ini server [%s:%d]\n", m_ip, m_port);
		
		// create
		m_socket = tcpsocket::socket(AF_INET, SOCK_STREAM, 0);
		if (m_socket == -1)
		{
			FPRINTF("tcpsocket::socket error\n");
			return false;
		}

		// bind
		sockaddr_in _sockaddr;
		memset(&_sockaddr, 0, sizeof(_sockaddr));
		_sockaddr.sin_family = AF_INET;
		_sockaddr.sin_port = htons(m_port);
		if(!tparam.ip.empty())
		{
			_sockaddr.sin_addr.s_addr = inet_addr((const char *)m_ip);
		}
		else
		{
			_sockaddr.sin_addr.s_addr = inet_addr(INADDR_ANY);
		}
		int32_t ret = tcpsocket::bind(m_socket, (const sockaddr *)&_sockaddr, sizeof(_sockaddr));
		if (ret != 0)
		{
			FPRINTF("tcpsocket::bind error\n");
			return false;
		}

		// listen
		ret = tcpsocket::listen(m_socket, tparam.backlog);
		if (ret != 0)
		{
			FPRINTF("tcpsocket::listen error\n");
			return false;
		}

		FPRINTF("tcpsocket::ini server [%s:%d] ok...\n", m_ip, m_port);
		
		// 置上标志
		m_connected = true;

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
		if (!connected())
		{
			return false;
		}

		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;

		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(m_socket, &readfds);

		int32_t ret = tcpsocket::select(m_socket + 1, 
			&readfds,
			0,
			0,
			&timeout);

		if (ret != -1 && FD_ISSET(m_socket, &readfds))
		{
			return true;
		}

		return false;
	}
	FORCEINLINE bool can_write()
	{
		if (!connected())
		{
			return false;
		}

		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;

		fd_set writefds;
		FD_ZERO(&writefds);
		FD_SET(m_socket, &writefds);

		int32_t ret = tcpsocket::select(m_socket + 1, 
			0,
			&writefds,
			0,
			&timeout);

		if (ret != -1 && FD_ISSET(m_socket, &writefds))
		{
			return true;
		}

		return false;
	}
	FORCEINLINE bool connected()
	{
		return m_connected;
	}
	FORCEINLINE bool reconnect()
	{
		// close
		close();

		FPRINTF("tcpsocket::reconnect [%s:%d]\n", m_peer_ip, m_peer_port);

		// create
		m_socket = tcpsocket::socket(AF_INET, SOCK_STREAM, 0);
		if (m_socket == -1)
		{
			FPRINTF("tcpsocket::socket error\n");
			return false;
		}

		// connect
		sockaddr_in _sockaddr;
		memset(&_sockaddr, 0, sizeof(_sockaddr));
		_sockaddr.sin_family = AF_INET;
		_sockaddr.sin_port = htons(m_peer_port);
		_sockaddr.sin_addr.s_addr = inet_addr((const char *)m_peer_ip);
		int32_t ret = tcpsocket::connect(m_socket, (const sockaddr *)&_sockaddr, sizeof(_sockaddr));
		if (ret != 0)
		{
			FPRINTF("tcpsocket::connect error\n");
			return false;
		}

		// 缓冲区
		if (!set_recv_buffer_size(m_socket_recv_buffer_size))
		{
			FPRINTF("tcpsocket::set_recv_buffer_size error\n");
			return false;
		}

		// 缓冲区
		if (!set_send_buffer_size(m_socket_send_buffer_size))
		{
			FPRINTF("tcpsocket::set_send_buffer_size error\n");
			return false;
		}

		// 非阻塞
		if (!set_nonblocking(m_is_non_blocking))
		{
			FPRINTF("tcpsocket::set_nonblocking error\n");
			return false;
		}

		// linger
		if (!set_linger(0))
		{
			FPRINTF("tcpsocket::set_linger error\n");
			return false;
		}

		// 获取自己的信息
		sockaddr_in _local_sockaddr;
		memset(&_local_sockaddr, 0, sizeof(_local_sockaddr));
		int32_t size = sizeof(_local_sockaddr);
		if (tcpsocket::getsockname(m_socket, (sockaddr *)&_local_sockaddr, &size) == 0)
		{
			fstrcopy(m_ip, (const int8_t *)inet_ntoa(_local_sockaddr.sin_addr), sizeof(m_ip));
			m_port = htons(_local_sockaddr.sin_port);
		}

		FPRINTF("tcpsocket::reconnect [%s:%d], local[%s:%d] ok...\n", m_peer_ip, m_peer_port,
			m_ip, m_port);

		// 置上标志
		m_connected = true;

		return true;
	}
	FORCEINLINE bool accept(tcpsocket & socket)
	{
		sockaddr_in _sockaddr;
		memset(&_sockaddr, 0, sizeof(_sockaddr));
		int32_t size = sizeof(_sockaddr);
		socket_t s;
		s = tcpsocket::accept(m_socket, (sockaddr*)&_sockaddr, &size);
		if (s != -1)
		{
			socket.m_socket = s;
			fstrcopy(socket.m_peer_ip, (const int8_t *)inet_ntoa(_sockaddr.sin_addr), sizeof(socket.m_peer_ip));
			socket.m_peer_port = htons(_sockaddr.sin_port);

			// 缓冲区
			if (!socket.set_recv_buffer_size(m_socket_recv_buffer_size))
			{
				FPRINTF("tcpsocket::set_recv_buffer_size error\n");
				return false;
			}

			// 缓冲区
			if (!socket.set_send_buffer_size(m_socket_send_buffer_size))
			{
				FPRINTF("tcpsocket::set_send_buffer_size error\n");
				return false;
			}

			// 非阻塞
			if (!socket.set_nonblocking(m_is_non_blocking))
			{
				FPRINTF("tcpsocket::set_nonblocking error\n");
				return false;
			}

			// linger
			if (!socket.set_linger(0))
			{
				FPRINTF("tcpsocket::set_linger error\n");
				return false;
			}

			// 获取自己的信息
			sockaddr_in _local_sockaddr;
			memset(&_local_sockaddr, 0, sizeof(_local_sockaddr));
			int32_t size = sizeof(_local_sockaddr);
			if (tcpsocket::getsockname(socket.m_socket, (sockaddr *)&_local_sockaddr, &size) == 0)
			{
				fstrcopy(socket.m_ip, (const int8_t *)inet_ntoa(_local_sockaddr.sin_addr), sizeof(socket.m_ip));
				socket.m_port = htons(_local_sockaddr.sin_port);
			}

			FPRINTF("tcpsocket::accept[%s:%d], local[%s:%d]\n", socket.m_peer_ip, 
				socket.m_peer_port,
				socket.m_ip,
				socket.m_port);
			return true;
		}

		return false;
	}
	FORCEINLINE bool set_recv_buffer_size(uint32_t size)
	{
		return tcpsocket::setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, &size, sizeof(uint32_t)) == 0;
	}
	FORCEINLINE bool set_send_buffer_size(uint32_t size)
	{
		return tcpsocket::setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, &size, sizeof(uint32_t)) == 0;
	}
	FORCEINLINE bool set_nonblocking(bool on)
	{ 
		return tcpsocket::set_socket_nonblocking(m_socket, on);
	}
	FORCEINLINE bool close()
	{
		return tcpsocket::close_socket(m_socket) == 0;
	}
	FORCEINLINE bool set_linger(uint32_t lingertime)
	{
		linger so_linger;
		so_linger.l_onoff = TRUE;
		so_linger.l_linger = lingertime;
		BOOL bRtn = FALSE;
		return tcpsocket::setsockopt(m_socket, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger)) == 0;
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
		return ::getsockopt(s, level, optname, (const char *)optval, optlen);
	}
	static FORCEINLINE int32_t setsockopt(socket_t s, int32_t level, int32_t optname, const void * optval, socklen_t optlen) 
	{
		return ::setsockopt(s, level, optname, (const char *)optval, optlen);
	}
	static FORCEINLINE int32_t getsockname(socket_t s, struct sockaddr * name, int32_t * addrlen) 
	{
		return ::getsockname(s, name, addrlen);
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
		return ioctlsocket(s, FIONBIO, (u_long *)&on) == 0;
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
	FORCEINLINE void clear()
	{
		m_socket = -1;
		m_port = 0;
		m_peer_port = 0;
		m_connected = false;

		m_send_queue.clear();
		m_recv_queue.clear();
		
		memset(m_ip, 0, sizeof(m_ip));
		memset(m_peer_ip, 0, sizeof(m_peer_ip));
	}
private:
	// socket
	socket_t m_socket;

	// ip
	int8_t m_ip[c_ip_size];

	// 端口
	uint16_t m_port;

	// 远端ip
	int8_t m_peer_ip[c_ip_size];

	// 远端端口
	uint16_t m_peer_port;

	// 是否非阻塞
	bool m_is_non_blocking;

	// socket发送缓冲区大小
	uint32_t m_socket_send_buffer_size;

	// socket接受缓冲区大小
	uint32_t m_socket_recv_buffer_size;

	// socket状态
	bool m_connected;

	// 发送缓冲区
	_queue m_send_queue;

	// 接受缓冲区
	_queue m_recv_queue;

	// 供外部调用的缓冲区函数
	slot<_queue, bool (_queue::*)(const int8_t * p, size_t size)> m_send_slot;
	slot<_queue, bool (_queue::*)(int8_t * out, size_t size)> m_recv_slot;
};

