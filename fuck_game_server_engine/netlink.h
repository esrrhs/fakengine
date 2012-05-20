#pragma once

// netlink 连接客户端类
template <typename _link>
class netlink
{
public:
	force_inline netlink()
	{
	}
	force_inline ~netlink()
	{
	}
public:
	template<typename _param>
	force_inline bool ini(const _param & param)
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
private:
	_link m_link;
};

