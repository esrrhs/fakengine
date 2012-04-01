#pragma once

// thread_sender 发送类
// _msg 消息
// _container 装消息的容器
// _realsender 真正发消息的实体
template <typename _msg, typename _container, typename _realsender>
class thread_sender : public thread
{
public:
	thread_sender()
	{
	}
	~thread_sender()
	{
	}
public:
	virtual void run()
	{
		while (1)
		{
			_msg * msg = 0;
			{
				auto_lock<thread_lock> lock(m_thread_lock);
				if (m_container.size() > 0)
				{
					msg = m_container.front();
				}
			}

			if (m_realsender.send_msg(msg))
			{
				auto_lock<thread_lock> lock(m_thread_lock);
				m_container.pop_front();
			}
			else
			{
				fsleep(1);
			}
		}
	}
public:
	FORCEINLINE bool send_msg(const _msg * msg)
	{
		auto_lock<thread_lock>(m_thread_lock);
		return m_container.push_back(msg);
	}
private:
	_container m_container;
	_realsender m_realsender;
	thread_lock m_thread_lock;
};

