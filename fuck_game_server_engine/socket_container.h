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
	FORCEINLINE bool ini(net_server_param * param)
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
	_socket_store m_socket_store;
};



