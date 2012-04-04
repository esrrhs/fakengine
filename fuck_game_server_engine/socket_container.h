#pragma once

// socket_container ���ӷ������
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
public:
	FORCEINLINE bool ini(const net_server_param & param)
	{
		return m_socket.ini(param);
	}
	FORCEINLINE void tick()
	{
		// ����������
		accept();

		// select��������
		select();

		// read��������
		tick_read();

		// write��������
		tick_write();
	}
private:
	FORCEINLINE bool accept()
	{
		if (m_socket.can_read())
		{
			_socket s;
			while(m_socket.accept(s))
			{
				
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



