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
	force_inline bool ini(const net_link_param & param)
	{
		return m_link.ini(param);
	}
	force_inline void tick()
	{
		m_link.tick();
	}
	template<typename _msg>
	force_inline bool send_msg(const _msg & msg)
	{
		return m_link.send_msg(msg);
	}
	template<typename _msg_container>
	force_inline bool send_msgs(const _msg_container & mc)
	{
		return m_link.send_msgs(mc);
	}
	template<typename _msg>
	force_inline bool recv_msg(_msg & msg)
	{
		return m_link.recv_msg(msg);
	}
	template<typename _msg_container>
	force_inline bool recv_msgs(_msg_container & mc)
	{
		return m_link.recv_msgs(mc);
	}
private:
	_link m_link;
};

