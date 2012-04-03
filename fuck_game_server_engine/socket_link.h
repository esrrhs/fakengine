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
	FORCEINLINE bool ini()
	{
		return m_socket.ini();
	}
	FORCEINLINE void tick()
	{
		m_socket.tick();
	}
	template<typename _msg>
	FORCEINLINE bool send_msg(const _msg * msg)
	{
		return m_socket.send(msg);
	}
	template<typename _msg>
	FORCEINLINE bool recv_msg(_msg * msg)
	{
		return m_socket.recv(msg);
	}
private:
	_socket m_socket;
};

