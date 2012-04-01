#pragma once

// netlink 连接客户端类
// _send 发消息实体，比如一个发送线程+缓冲区，动力来自线程
// _recv 接消息实体，比如一个接收线程+缓冲区，动力来自线程
template <typename _send, typename _recv>
class netlink
{
public:
	netlink()
	{
	}
	~netlink()
	{
	}
public:
	template<typename _msg>
	FORCEINLINE bool send_msg(const _msg * msg)
	{
		return m_send.send_msg(msg);
	}
	template<typename _msg>
	FORCEINLINE bool recv_msg(_msg * msg)
	{
		return m_recv.recv_msg(msg);
	}
private:
	_send m_send;
	_recv m_recv;
};

