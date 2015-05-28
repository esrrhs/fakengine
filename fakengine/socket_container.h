#pragma once

// socket_container 连接服务端类
template <typename _msg, typename _socket, typename _real_select, 
	typename _event_processor, uint32_t N>
class socket_container
{
public:
	force_inline socket_container()
	{
	}
	force_inline ~socket_container()
	{
	}
public:
	template<typename _param>
	force_inline bool ini(const _param & param)
	{
		if (m_socket.ini(param))
		{
			m_real_select.add(m_socket.get_socket_t(), &m_socket);
			return true;
		}
		return false;
	}
	force_inline void tick()
	{
		// select现有连接
		select();

		// tick现有连接
		tick_event();

		// 处理消息
		process_msg();

		// 删除
		process_del();
	}
private:
	force_inline bool select()
	{
		return m_real_select.select();
	}
	force_inline bool tick_event()
	{
		for (int32_t i = 0; i < m_real_select.event_size(); i++)
		{
			int32_t flag = 0;
			void * p = 0;
			m_real_select.get_event(i, flag, p);
			_socket & s = *(_socket*)p;
			if (s.connected())
			{
				if (flag & FSOCKET_READ)
				{
					if (s.get_socket_t() == m_socket.get_socket_t())
					{
						accept();
						continue;
					}

					if (!s.fill())
					{
						close(s);
						continue;
					}
				}
				if (flag & FSOCKET_WRITE)
				{
					if (!s.flush())
					{
						close(s);
						continue;
					}
				}
				if (flag & FSOCKET_ERROR)
				{
					close(s);
					continue;
				}
			}
		}
		return true;
	}
	force_inline bool accept()
	{
		_socket & s = m_acceptsocket;
		if (!m_socket_store.full() && m_socket.accept(s))
		{
			// add to container
			m_socket_store.push_back(s);
			m_event_processor.on_accept(m_socket_store.back());
			m_real_select.add(s.get_socket_t(), &m_socket_store.back());
			LOG_SYS(FENGINE_HEADER "socket link accepted %s:%d", (const char *)s.get_peer_ip(), s.get_peer_port());
		}
		return true;
	}
	force_inline bool process_msg()
	{
		_msg msg;
		for (_socket_store_iter it = m_socket_store.begin(); it != m_socket_store.end(); it++)
		{
			_socket & s = *it;
			int32_t i = 0;
			while (s.connected() && i < c_socket_container_max_process_per_frame && s.recv(msg))
			{
				if (!m_event_processor.on_recv_msg(s, msg))
				{
					break;
				}
				i++;
			}
		}
		return true;
	}
	force_inline bool process_del()
	{
		for (_socket_store_iter it = m_socket_store.begin(); it != m_socket_store.end(); it++)
		{
			_socket & s = *it;
			if (!s.connected())
			{
				m_socket_store.erase(it++);
			}
			else
			{
				it++;
			}
		}
		return true;
	}
public:
	force_inline bool close(_socket & s)
	{
		if (s.connected())
		{
			m_real_select.del(s.get_socket_t());
			m_event_processor.on_close(s);
			s.close();
			LOG_SYS(FENGINE_HEADER "socket link closed %s:%d", (const char *)s.get_peer_ip(), s.get_peer_port());
		}
		return true;
	}
	force_inline bool send_msg(_socket & s, const _msg & msg)
	{
		if (s.connected())
		{
			return s.send(msg);
		}
		return false;
	}
private:
	typedef flist<_socket, N> _socket_store;
	typedef typename _socket_store::iterator _socket_store_iter;
	_socket m_socket;
	_real_select m_real_select;
	_socket_store m_socket_store;
	_event_processor m_event_processor;
	_socket m_acceptsocket;
};



