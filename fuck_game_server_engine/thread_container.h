#pragma once

// thread_container 连接服务端类
template <typename _real_container>
class thread_container : public thread
{
public:
	thread_container()
	{
	}
	~thread_container()
	{
	}
public:
	virtual void run()
	{
		while (1)
		{
			m_real_container.tick();
		}
	}
public:
	FORCEINLINE bool ini(const net_server_param & param)
	{
		if (!m_real_container.ini(param))
		{
			return false;
		}
		start();
		return true;
	}
	FORCEINLINE void tick()
	{
	}
private:
	_real_container m_real_container;
};



