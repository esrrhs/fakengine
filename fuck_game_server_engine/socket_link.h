#pragma once

// socket_link ∑¢ÀÕ¿‡
template <typename _socket>
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
	FORCEINLINE bool ini(const net_link_param & param)
	{
		return m_socket.ini(param);
	}
	FORCEINLINE void tick()
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
				}
			}
		}
		else
		{
			m_socket.reconnect();
		}
	}
	template<typename _msg>
	FORCEINLINE bool send_msg(const _msg & msg)
	{
		return m_socket.send(msg);
	}
	template<typename _msg>
	FORCEINLINE bool recv_msg(_msg & msg)
	{
		return m_socket.recv(msg);
	}
private:
	_socket m_socket;
};

