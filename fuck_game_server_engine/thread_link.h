#pragma once

// thread_link 客户端线程类
template <typename _msg, typename _container, typename _reallink>
class thread_link : public thread
{
public:
	thread_link()
	{
	}
	~thread_link()
	{
	}
public:
	virtual void run()
	{
		while (1)
		{
			m_reallink.tick();

			// recv
			{
				_msg * msg = 0;
				if (m_reallink.recv_msg(msg))
				{
					auto_lock<thread_lock> lock(m_recv_thread_lock);
					m_recv_container.push_back(msg);
				}
			}

			// send
			{
				_msg * msg = 0;
				{
					auto_lock<thread_lock> lock(m_send_thread_lock);
					if (m_send_container.size() > 0)
					{
						msg = m_send_container.front();
					}
				}

				if (m_reallink.send_msg(msg))
				{
					auto_lock<thread_lock> lock(m_send_thread_lock);
					m_send_container.pop_front();
				}
			}
		}
	}
public:
	FORCEINLINE bool ini()
	{
		start();
		return m_reallink.ini();
	}
	FORCEINLINE void tick()
	{
	}
	FORCEINLINE bool send_msg(const _msg * msg)
	{
		auto_lock<thread_lock> lock(m_send_thread_lock);
		return m_send_container.push_back(msg);
	}
	FORCEINLINE bool recv_msg(_msg * msg)
	{
		auto_lock<thread_lock> lock(m_recv_container);
		if (m_recv_container.size() > 0)
		{
			msg = m_recv_container.front();
			m_recv_container.pop_front();
			return true;
		}
		return false;
	}
private:
	_container m_send_container;
	_container m_recv_container;
	thread_lock m_send_thread_lock;
	thread_lock m_recv_thread_lock;
	_reallink m_reallink;
};

