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

