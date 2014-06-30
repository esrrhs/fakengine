#pragma once

// socket_link 发送类
template <typename _msg, typename _socket, typename _event_processor>
class socket_link
{
public:
	force_inline socket_link() : m_connect(false)
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
	force_inline void tick()
	{
		if (m_socket.connected())
		{
			if (!m_connect)
			{
				m_event_processor.on_connect(m_socket);
				m_connect = true;
			}

			if (m_socket.select())
			{
				if (m_socket.can_write())
				{
					m_socket.flush();
				}
				if (m_socket.can_read())
				{
					m_socket.fill();
					// 立即处理消息
					process_msg();
				}
				if (m_socket.can_except())
				{
					m_socket.close();
				}
			}
		}
		else
		{
			if (m_connect)
			{
				m_event_processor.on_close(m_socket);
				m_connect = false;
			}

			m_socket.reconnect();
		}
	}
	force_inline bool send_msg(const _msg & msg)
	{
		return m_socket.send(msg);
	}
private:
	force_inline bool process_msg()
	{
		_msg msg;
		while (m_socket.recv(msg))
		{
			m_event_processor.on_recv_msg(m_socket, msg);
		}
		return true;
	}
private:
	_socket m_socket;
	bool m_connect;
	_event_processor m_event_processor;
};

