#pragma once

// socket_sender 发送类
// _socket 装消息的容器
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

