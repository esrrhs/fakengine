#pragma once

// socket_recver ������
// _socket װ��Ϣ������
template <typename _socket>
class socket_recver
{
public:
	socket_recver()
	{
	}
	~socket_recver()
	{
	}
public:
	FORCEINLINE void tick()
	{
		m_socket.tick();
	}
	template<typename _msg>
	FORCEINLINE bool recv_msg(_msg * msg)
	{
		return m_socket.recv(msg);
	}
private:
	_socket m_socket;
};

