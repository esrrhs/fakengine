#pragma once

template <typename _queue, typename _user_data = int32_t>
class tcpsocket
{
public:
	force_inline tcpsocket()
	{
		clear();
	}
	force_inline ~tcpsocket()
	{
	}
public:
	template<typename _msg>
	force_inline bool send(const _msg & msg)
	{
		return m_send_queue.write(msg);
	}
	template<typename _msg>
	force_inline bool recv(_msg & msg)
	{
		return m_recv_queue.read(msg);
	}
	force_inline bool ini(const tcp_socket_link_param & tparam)
	{
		// close
		close();
		clear();

		fstrcopy(m_peer_ip, (const int8_t *)tparam.ip.c_str(), sizeof(m_peer_ip));
		m_peer_port = tparam.port;
		m_is_non_blocking = tparam.is_non_blocking;
		m_socket_send_buffer_size = tparam.socket_send_buffer_size;
		m_socket_recv_buffer_size = tparam.socket_recv_buffer_size;

		LOG_SYS(FENGINE_HEADER "tcpsocket::ini client [%s:%d] ok...", m_peer_ip, m_peer_port);

		return true;
	}
	force_inline bool ini(const tcp_socket_server_param & tparam)
	{
		// close
		close();
		clear();

		fstrcopy(m_ip, (const int8_t *)tparam.ip.c_str(), sizeof(m_ip));
		m_port = tparam.port;
		m_is_non_blocking = tparam.is_non_blocking;
		m_socket_send_buffer_size = tparam.socket_send_buffer_size;
		m_socket_recv_buffer_size = tparam.socket_recv_buffer_size;

		LOG_SYS(FENGINE_HEADER "tcpsocket::ini server [%s:%d]", m_ip, m_port);
		
		// create
		m_socket = tcpsocket::socket(AF_INET, SOCK_STREAM, 0);
		if (m_socket == -1)
		{
			LOG_ERROR(FENGINE_HEADER "tcpsocket::socket error");
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
			_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		}
		int32_t ret = tcpsocket::bind(m_socket, (const sockaddr *)&_sockaddr, sizeof(_sockaddr));
		if (ret != 0)
		{
			LOG_ERROR(FENGINE_HEADER "tcpsocket::bind error");
			close();
			return false;
		}

		// listen
		ret = tcpsocket::listen(m_socket, tparam.backlog);
		if (ret != 0)
		{
			LOG_ERROR(FENGINE_HEADER "tcpsocket::listen error");
			close();
			return false;
		}

		// 非阻塞
		if (!set_nonblocking(m_is_non_blocking))
		{
			LOG_ERROR(FENGINE_HEADER "tcpsocket::set_nonblocking error");
			close();
			return false;
		}

		LOG_SYS(FENGINE_HEADER "tcpsocket::ini server [%s:%d] ok...", m_ip, m_port);
		
		return true;
	}
	force_inline bool flush()
	{
		if (m_send_queue.empty())
		{
			return true;
		}

		int32_t len = tcpsocket::send(m_socket, 
			m_send_queue.get_read_line_buffer(), 
			m_send_queue.get_read_line_size(), 0);
		
		if (len == 0 || len < 0)
		{
			if (GET_NET_ERROR != NET_BLOCK_ERROR && GET_NET_ERROR != NET_BLOCK_ERROR_EX)
			{
				return false;
			}
		}
		else
		{
			m_send_queue.skip_read(len);
		}
		
		return true;
	}
	force_inline bool fill()
	{
		if (m_recv_queue.full())
		{
			return true;
		}

		int32_t len = tcpsocket::recv(m_socket, 
			m_recv_queue.get_write_line_buffer(), 
			m_recv_queue.get_write_line_size(), 0);

		if (len == 0 || len < 0)
		{
			if (GET_NET_ERROR != NET_BLOCK_ERROR && GET_NET_ERROR != NET_BLOCK_ERROR_EX)
			{
				return false;
			}
		}
		else
		{
			m_recv_queue.skip_write(len);
		}

		return true;
	}
	force_inline bool connected() const
	{
		return m_socket != -1;
	}
	force_inline bool reconnect()
	{
		// close
		close();
		clear();

		LOG_SYS(FENGINE_HEADER "tcpsocket::reconnect [%s:%d]", m_peer_ip, m_peer_port);

		// create
		m_socket = tcpsocket::socket(AF_INET, SOCK_STREAM, 0);
		if (m_socket == -1)
		{
			LOG_ERROR(FENGINE_HEADER "tcpsocket::socket error");
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
			LOG_ERROR(FENGINE_HEADER "tcpsocket::connect error");
			close();
			return false;
		}

		// 缓冲区
		if (!set_recv_buffer_size(m_socket_recv_buffer_size))
		{
			LOG_ERROR(FENGINE_HEADER "tcpsocket::set_recv_buffer_size error");
			close();
			return false;
		}

		// 缓冲区
		if (!set_send_buffer_size(m_socket_send_buffer_size))
		{
			LOG_ERROR(FENGINE_HEADER "tcpsocket::set_send_buffer_size error");
			close();
			return false;
		}

		// 非阻塞
		if (!set_nonblocking(m_is_non_blocking))
		{
			LOG_ERROR(FENGINE_HEADER "tcpsocket::set_nonblocking error");
			close();
			return false;
		}

		// linger
		if (!set_linger(0))
		{
			LOG_ERROR(FENGINE_HEADER "tcpsocket::set_linger error");
			close();
			return false;
		}

		// 获取自己的信息
		sockaddr_in _local_sockaddr;
		memset(&_local_sockaddr, 0, sizeof(_local_sockaddr));
		socklen_t size = sizeof(_local_sockaddr);
		if (tcpsocket::getsockname(m_socket, (sockaddr *)&_local_sockaddr, &size) == 0)
		{
			fstrcopy(m_ip, (const int8_t *)inet_ntoa(_local_sockaddr.sin_addr), sizeof(m_ip));
			m_port = htons(_local_sockaddr.sin_port);
		}

		LOG_SYS(FENGINE_HEADER "tcpsocket::reconnect [%s:%d], local[%s:%d] ok...", m_peer_ip, m_peer_port,
			m_ip, m_port);

		return true;
	}
	force_inline bool accept(tcpsocket & socket)
	{
		sockaddr_in _sockaddr;
		memset(&_sockaddr, 0, sizeof(_sockaddr));
		socklen_t size = sizeof(_sockaddr);
		socket_t s;
		s = tcpsocket::accept(m_socket, (sockaddr*)&_sockaddr, &size);
		if (s == -1)
		{
			LOG_ERROR(FENGINE_HEADER "tcpsocket::accept error");
			return false;
		}

		socket.clear();
		socket.m_socket = s;
		fstrcopy(socket.m_peer_ip, (const int8_t *)inet_ntoa(_sockaddr.sin_addr), sizeof(socket.m_peer_ip));
		socket.m_peer_port = htons(_sockaddr.sin_port);

		// 缓冲区
		if (!socket.set_recv_buffer_size(m_socket_recv_buffer_size))
		{
			LOG_ERROR(FENGINE_HEADER "tcpsocket::set_recv_buffer_size error");
			socket.close();
			return false;
		}

		// 缓冲区
		if (!socket.set_send_buffer_size(m_socket_send_buffer_size))
		{
			LOG_ERROR(FENGINE_HEADER "tcpsocket::set_send_buffer_size error");
			socket.close();
			return false;
		}

		// 非阻塞
		if (!socket.set_nonblocking(m_is_non_blocking))
		{
			LOG_ERROR(FENGINE_HEADER "tcpsocket::set_nonblocking error");
			socket.close();
			return false;
		}

		// linger
		if (!socket.set_linger(0))
		{
			LOG_ERROR(FENGINE_HEADER "tcpsocket::set_linger error");
			socket.close();
			return false;
		}

		// 获取自己的信息
		sockaddr_in _local_sockaddr;
		memset(&_local_sockaddr, 0, sizeof(_local_sockaddr));
		size = sizeof(_local_sockaddr);
		if (tcpsocket::getsockname(socket.m_socket, (sockaddr *)&_local_sockaddr, &size) == 0)
		{
			fstrcopy(socket.m_ip, (const int8_t *)inet_ntoa(_local_sockaddr.sin_addr), sizeof(socket.m_ip));
			socket.m_port = htons(_local_sockaddr.sin_port);
		}

		LOG_SYS(FENGINE_HEADER "tcpsocket::accept[%s:%d], local[%s:%d]", socket.m_peer_ip,
			socket.m_peer_port,
			socket.m_ip,
			socket.m_port);

		return true;
	}
	force_inline bool set_recv_buffer_size(uint32_t size)
	{
		return tcpsocket::setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, &size, sizeof(uint32_t)) == 0;
	}
	force_inline bool set_send_buffer_size(uint32_t size)
	{
		return tcpsocket::setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, &size, sizeof(uint32_t)) == 0;
	}
	force_inline bool set_nonblocking(bool on)
	{ 
		return tcpsocket::set_socket_nonblocking(m_socket, on);
	}
	force_inline bool close()
	{
		if (m_socket != -1)
		{
			tcpsocket::close_socket(m_socket);
			m_socket = -1;
		}
		return true;
	}
	force_inline bool set_linger(uint32_t lingertime)
	{
		linger so_linger;
		so_linger.l_onoff = true;
		so_linger.l_linger = lingertime;
		return tcpsocket::setsockopt(m_socket, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger)) == 0;
	}
public:
	static force_inline socket_t socket(int32_t domain, int32_t type, int32_t protocol)
	{
		return ::socket(domain, type, protocol);
	}
	static force_inline int32_t bind(socket_t s, const struct sockaddr * name, uint32_t namelen)
	{
		return ::bind(s, name, namelen);
	}
	static force_inline int32_t connect(socket_t s, const struct sockaddr* name, uint32_t namelen)
	{
		return ::connect(s, name, namelen);
	}
	static force_inline int32_t listen(socket_t s, uint32_t backlog) 
	{
		return ::listen(s, backlog);
	}
	static force_inline socket_t accept(socket_t s, struct sockaddr * name, socklen_t * addrlen) 
	{
		return ::accept(s, name, addrlen);
	}
	static force_inline int32_t getsockopt(socket_t s, int32_t level, int32_t optname, void * optval, socklen_t * optlen) 
	{
		return ::getsockopt(s, level, optname, (const char *)optval, optlen);
	}
	static force_inline int32_t setsockopt(socket_t s, int32_t level, int32_t optname, const void * optval, socklen_t optlen) 
	{
		return ::setsockopt(s, level, optname, (const char *)optval, optlen);
	}
	static force_inline int32_t getsockname(socket_t s, struct sockaddr * name, socklen_t * addrlen) 
	{
		return ::getsockname(s, name, addrlen);
	}
	static force_inline int32_t send(socket_t s, const void * buf, int32_t len, int32_t flags) 
	{
		return ::send(s, (const char *)buf, len, flags);
	}
	static force_inline int32_t recv(socket_t s, void * buf, int32_t len, int32_t flags) 
	{
		return ::recv(s, (char *)buf, len, flags);
	}
	static force_inline int32_t close_socket(socket_t s)
	{
#if defined(WIN32)
		return ::closesocket(s);
#else
		return ::close(s);
#endif
	}
	static force_inline bool set_socket_nonblocking(socket_t s, bool on) 
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
	static force_inline int32_t select(int32_t maxfdp1, fd_set * readset, fd_set * writeset, fd_set * exceptset, struct timeval* timeout) 
	{
		return ::select(maxfdp1, readset, writeset, exceptset, timeout);
	}
private:
	force_inline void clear()
	{
		m_socket = -1;
		m_send_queue.clear();
		m_recv_queue.clear();
	}
public:
	socket_t get_socket_t() const
	{
		return m_socket;
	}
	const int8_t * get_ip() const
	{
		return m_ip;
	}
	const int8_t * get_peer_ip() const
	{
		return m_peer_ip;
	}
	uint16_t get_port() const
	{
		return m_port;
	}
	uint16_t get_peer_port() const
	{
		return m_peer_port;
	}
	const _user_data & get_user_data() const
	{
		return m_user_data;
	}
	void set_user_data(const _user_data & data)
	{
		m_user_data = data;
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

	// 发送缓冲区
	_queue m_send_queue;

	// 接受缓冲区
	_queue m_recv_queue;

	// 用户定义数据
	_user_data m_user_data;
};

