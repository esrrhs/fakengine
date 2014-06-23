#pragma once

// netserver 连接服务端类
// _container 连接容器实体，包括accept、select
template <typename _container>
class netserver
{
public:
	force_inline netserver()
	{
	}
	force_inline ~netserver()
	{
	}
public:
	template<typename _param>
	force_inline bool ini(const _param & param)
	{
		return m_container.ini(param);
	}
	force_inline void tick()
	{
		m_container.tick();
	}
public:
	template<typename ele_type, typename _msg>
	force_inline bool send_msg(ele_type & e, const _msg & msg)
	{
		return m_container.send_msg(e, msg);
	}
private:
	_container m_container;
};

