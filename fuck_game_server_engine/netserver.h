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
	FORCEINLINE bool ini()
	{
		return m_container.ini();
	}
	FORCEINLINE void tick()
	{
		m_container.tick();
	}
private:
	_container m_container;
};

