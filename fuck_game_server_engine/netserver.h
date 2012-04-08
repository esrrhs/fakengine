#pragma once

// netserver 连接服务端类
// _container 连接容器实体，包括accept、select
template <typename _container>
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
	fore_inline bool ini(const net_server_param & param)
	{
		return m_container.ini(param);
	}
	fore_inline void tick()
	{
		m_container.tick();
	}
public:
	template<typename ele_type, typename _msg>
	fore_inline bool send_msg(ele_type * e, const _msg & msg)
	{
		return m_container.send_msg(e, msg);
	}
	template<typename ele_type, typename _msg>
	fore_inline bool recv_msg(ele_type * & e, _msg & msg)
	{
		return m_container.recv_msg(e, msg);
	}
private:
	_container m_container;
};

