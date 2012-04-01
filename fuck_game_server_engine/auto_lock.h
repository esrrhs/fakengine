#pragma once

template <typename T>
class auto_lock
{
public:
	auto_lock(T & rLock) : m_lock(rLock)
	{
		m_lock.lock();
	}
	~auto_lock()
	{
		m_lock.unlock();
	}
private:
	auto_lock() {}
private:
	T & m_lock;
};

