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
	template<typename _msg>
	FORCEINLINE bool recv_msg(_msg * msg)
	{
		return m_socket.recv(msg);
	}
	FORCEINLINE bool fill()
	{
		return m_socket.fill();
	}
private:
	_socket m_socket;
};

