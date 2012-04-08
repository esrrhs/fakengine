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
	enum _const
	{
		recv_max_pac_per_frame = 100,
		send_max_pac_per_frame = 100,
	};
public:
	virtual void run()
	{
		_msg mempty;
		_msg & m = mempty;
		int32_t i = 0;
		while (1)
		{
			m_reallink.tick();

			// recv
			i = 0;
			while (m_reallink.recv_msg(m) && i < recv_max_pac_per_frame)
			{
				auto_lock<thread_lock> lock(m_recv_thread_lock);
				m_recv_container.push_back(m);
				i++;
			}

			// send
			i = 0;
			while(i < send_max_pac_per_frame)
			{
				{
					auto_lock<thread_lock> lock(m_send_thread_lock);
					if (m_send_container.size() > 0)
					{
						m = m_send_container.front();
					}
					else
					{
						break;
					}
				}

				if (m_reallink.send_msg(m))
				{
					auto_lock<thread_lock> lock(m_send_thread_lock);
					m_send_container.pop_front();
				}
				i++;
			}

			fsleep(1);
		}
	}
public:
	force_inline bool ini(const net_link_param & param)
	{
		// 连接失败可以重连
		m_reallink.ini(param);
		start();
		return true;
	}
	force_inline void tick()
	{
	}
	force_inline bool send_msg(const _msg & msg)
	{
		auto_lock<thread_lock> lock(m_send_thread_lock);
		m_send_container.push_back(msg);
		return true;
	}
	force_inline bool recv_msg(_msg & msg)
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

