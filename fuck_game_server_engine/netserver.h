#pragma once

// netserver ���ӷ������
// _container ��������ʵ�壬����accept��select
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
	FORCEINLINE bool ini(net_server_param * param)
	{
		return m_container.ini(param);
	}
	FORCEINLINE void tick()
	{
		m_container.tick();
	}
private:
	_container m_container;
};

