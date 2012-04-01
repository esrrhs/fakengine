#pragma once

// netlink 连接客户端类
// _container 连接容器实体，包括accept、select
// _send 发消息实体，此处可能就只是缓冲区，动力来自_container
// _recv 接消息实体，此处可能就只是缓冲区，动力来自_container
template <typename _container, typename _send, typename _recv>
class netserver
{
public:
	netserver()
	{
	}
	~netserver()
	{
	}
public:
	template<typename _msg>
	FORCEINLINE bool send_msg(netlink<_send, _recv> & link, const _msg * msg)
	{
		return link.send_msg(msg);
	}
	template<typename _msg>
	FORCEINLINE bool recv_msg(netlink<_send, _recv> & link, _msg * msg)
	{
		return link.recv_msg(msg);
	}
private:
	_container m_container;
};

