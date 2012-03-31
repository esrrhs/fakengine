#pragma once

template <typename T>
class auto_lock
{
public:
	auto_lock(T & rLock)
	{
		m_pLock = &rLock;
		m_plock->lock();
	}
	~auto_lock()
	{
		m_plock->unlock();
	}
private:
	auto_lock() {}
private:
	T * m_plock;
};

