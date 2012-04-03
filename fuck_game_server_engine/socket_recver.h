#pragma once

// socket_recver 发送类
// _socket 装消息的容器
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

