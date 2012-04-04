#pragma once

// netlink 连接客户端类
template <typename _link>
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
	FORCEINLINE bool ini(const net_link_param & param)
	{
		return m_link.ini(param);
	}
	FORCEINLINE void tick()
	{
		m_link.tick();
	}
	template<typename _msg>
	FORCEINLINE bool send_msg(const _msg * msg)
	{
		return m_link.send_msg(msg);
	}
	template<typename _msg>
	FORCEINLINE bool recv_msg(_msg * msg)
	{
		return m_link.recv_msg(msg);
	}
private:
	_link m_link;
};

