#pragma once

// socket_container 连接服务端类
template <typename _socket, typename _real_select, typename _socket_store>
class socket_container
{
public:
	socket_container()
	{
	}
	~socket_container()
	{
	}
	enum _const
	{
		max_accept_per_frame = 50,
	};
public:
	force_inline bool ini(const net_server_param & param)
	{
		return m_socket.ini(param);
	}
	force_inline void tick()
	{
		// 处理新连接
		accept();

		// select现有连接
		select();

		// read现有连接
		tick_read();

		// write现有连接
		tick_write();

		// 查错误
		tick_error();
	}
private:
	force_inline bool accept()
	{
		m_socket.select();
		if (m_socket.can_read())
		{
			_socket s;
			int32_t num = 0;
			while(m_socket.accept(s) && num < max_accept_per_frame)
			{
				// add to container
				m_socket_store.push_back(s);
				num++;
			}
		}
		return true;
	}
	force_inline bool select()
	{
		m_real_select.clear();

		// add
		for (_socket_store_iter it = m_socket_store.begin(); it != m_socket_store.end(); it++)
		{
			_socket & s = *it;
			m_real_select.add(s.get_socket_t());
		}

		return m_real_select.select();
	}
	force_inline bool tick_read()
	{
		for (_socket_store_iter it = m_socket_store.begin(); it != m_socket_store.end(); it++)
		{
			_socket & s = *it;
			if (m_real_select.is_read(s.get_socket_t()))
			{
				s.fill();
			}
		}

		return true;
	}
	force_inline bool tick_write()
	{
		for (_socket_store_iter it = m_socket_store.begin(); it != m_socket_store.end(); it++)
		{
			_socket & s = *it;
			if (m_real_select.is_write(s.get_socket_t()))
			{
				s.flush();
			}
		}

		return true;
	}
	force_inline bool tick_error()
	{
		for (_socket_store_iter it = m_socket_store.begin(); it != m_socket_store.end(); )
		{
			_socket & s = *it;
			if (!s.connected())
			{
				FPRINTF("socket_container::tick_error [%s:%d] close\n", 
					s.get_peer_ip(), 
					s.get_peer_port());

				m_socket_store.erase(it++);
			}
			else if (m_real_select.is_except(s.get_socket_t()))
			{
				FPRINTF("socket_container::tick_error [%s:%d] except\n", 
					s.get_peer_ip(), 
					s.get_peer_port());

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
	template<typename _msg>
	force_inline bool send_msg(_socket * s, const _msg & msg)
	{
		return s->send(msg);
	}
	template<typename _msg>
	force_inline bool recv_msg(_socket * & s, _msg & msg)
	{
		return s->recv(msg);
	}
	force_inline void reset()
	{
		m_it_cur = m_socket_store.begin();
	}
	force_inline bool get_next(_socket * & s)
	{
		if (m_it_cur != m_socket_store.end())
		{
			s = &(*m_it_cur);
			m_it_cur++;
			return true;
		}
		return false;
	}
private:
	_socket m_socket;
	_real_select m_real_select;
	_socket_store m_socket_store;
	typedef typename _socket_store::iterator _socket_store_iter;
	_socket_store_iter m_it_cur;
};



