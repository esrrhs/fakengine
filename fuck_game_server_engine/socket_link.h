#pragma once

// socket_link 发送类
template <typename _msg, typename _socket, typename _msg_processor>
class socket_link
{
public:
	socket_link()
	{
	}
	~socket_link()
	{
	}
public:
	force_inline bool ini(const net_link_param & param)
	{
		return m_socket.ini(param);
	}
	force_inline void tick()
	{
		if (m_socket.connected())
		{
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
			}
		}
		else
		{
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
			m_msg_processor.process(m_socket, msg);
		}
		return true;
	}
private:
	_socket m_socket;
	_msg_processor m_msg_processor;
};

