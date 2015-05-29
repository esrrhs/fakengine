#pragma once

// socket_link 发送类
template <typename _msg, typename _socket, typename _real_select, typename _event_processor>
class socket_link
{
public:
	force_inline socket_link()
	{
	}
	force_inline ~socket_link()
	{
	}
public:
	template<typename _param>
	force_inline bool ini(const _param & param)
	{
		return m_socket.ini(param);
	}
	force_inline bool isconnect() const
	{
		return m_socket.connected();
	}
	force_inline void tick()
	{
		if (m_socket.connected())
		{
			// select现有连接
			select();

			// tick现有连接
			tick_event();

			// 处理消息
			process_msg();
		}
		else
		{
			if (m_socket.reconnect())
			{
				m_event_processor.on_connect(m_socket);
				m_real_select.add(m_socket.get_socket_t(), &m_socket);
				LOG_SYS(FENGINE_HEADER "socket link connected %s:%d", (const char *)m_socket.get_peer_ip(), m_socket.get_peer_port());
			}
		}
	}
	force_inline bool send_msg(const _msg & msg)
	{
		return m_socket.send(msg);
	}
	force_inline bool close()
	{
		if (m_socket.connected())
		{
			m_real_select.del(m_socket.get_socket_t());
			m_event_processor.on_close(m_socket);
			m_socket.close();
			LOG_SYS(FENGINE_HEADER "socket link closed %s:%d", (const char *)m_socket.get_peer_ip(), m_socket.get_peer_port());
		}
		return true;
	}
	force_inline void set_event_processor(const _event_processor & pro)
	{
		m_event_processor = pro;
	}
private:
	force_inline bool select()
	{
		return m_real_select.select();
	}
	force_inline bool tick_event()
	{
		if (m_real_select.event_size())
		{
			int32_t flag = 0;
			void * p = 0;
			m_real_select.get_event(0, flag, p);
			FASSERT(&m_socket == p);
			if (m_socket.connected())
			{
				if (flag & FSOCKET_READ)
				{
					if (!m_socket.fill())
					{
						close();
						return false;
					}
				}
				if (flag & FSOCKET_WRITE)
				{
					if (!m_socket.flush())
					{
						close();
						return false;
					}
				}
				if (flag & FSOCKET_ERROR)
				{
					close();
					return false;
				}
			}
		}
		return true;
	}
	force_inline bool process_msg()
	{
		_msg msg;
		int32_t i = 0;
		while (i < (int32_t)c_socket_link_max_process_per_frame && m_socket.recv(msg))
		{
			m_event_processor.on_recv_msg(m_socket, msg);
			i++;
		}
		return true;
	}
private:
	_socket m_socket;
	_real_select m_real_select;
	_event_processor m_event_processor;
};

