#pragma once

// socket_sender ������
// _socket װ��Ϣ������
template <typename _socket>
class socket_sender
{
public:
	socket_sender()
	{
	}
	~socket_sender()
	{
	}
public:
	FORCEINLINE void tick()
	{
		m_socket.tick();
	}
	template<typename _msg>
	FORCEINLINE bool send_msg(const _msg * msg)
	{
		return m_socket.send(msg);
	}
private:
	_socket m_socket;
};

