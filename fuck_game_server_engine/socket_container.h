#pragma once

// socket_container 连接服务端类
template <typename _socket, typename _socket_store>
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
	FORCEINLINE bool ini(const net_server_param & param)
	{
		return m_socket.ini(param);
	}
	FORCEINLINE void tick()
	{
		// 处理新连接
		accept();

		// select现有连接
		select();

		// read现有连接
		tick_read();

		// write现有连接
		tick_write();
	}
private:
	FORCEINLINE bool accept()
	{
		if (m_socket.can_read())
		{
			_socket s;
			int32_t num = 0;
			while(m_socket.accept(s) && num < max_accept_per_frame)
			{
				// add to container
				num++;
			}
		}
		return true;
	}
	FORCEINLINE bool select()
	{
		return true;
	}
	FORCEINLINE bool tick_read()
	{
		return true;
	}
	FORCEINLINE bool tick_write()
	{
		return true;
	}
private:
	_socket m_socket;
	int8_t m_ip[c_ip_size];
	uint16_t m_port;
	_socket_store m_socket_store;
};



