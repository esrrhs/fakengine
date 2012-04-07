#pragma once

// thread_container 连接服务端类
template <typename _msg, typename _ele_type, typename _container, typename _real_container>
class thread_container : public thread
{
public:
	thread_container()
	{
	}
	~thread_container()
	{
	}
	enum _const
	{
		recv_max_pac_per_link_per_frame = 100,
		send_max_pac_per_frame = 100,
	};
public:
	virtual void run()
	{
		_msg tmp;
		_ele_type etmp;
		int32_t i = 0;
		while (1)
		{
			m_real_container.tick();

			// recv
			m_real_container.reset();
			while(m_real_container.get_next(etmp))
			{
				i = 0;
				while (m_real_container.recv_msg(etmp, tmp) && i < recv_max_pac_per_link_per_frame)
				{
					auto_lock<thread_lock> lock(m_recv_thread_lock);
					m_recv_container.push_back(std::make_pair(etmp, tmp));
					i++;
				}
			}
						
			// send
			i = 0;
			while(i < send_max_pac_per_frame)
			{
				_msg & msg = tmp;
				_ele_type & e = etmp;
				{
					auto_lock<thread_lock> lock(m_send_thread_lock);
					if (m_send_container.size() > 0)
					{
						e = m_send_container.front().first;
						msg = m_send_container.front().second;
					}
					else
					{
						break;
					}
				}

				if (m_real_container.send_msg(e, msg))
				{
					auto_lock<thread_lock> lock(m_send_thread_lock);
					m_send_container.pop_front();
				}
				i++;
			}
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
public:
	FORCEINLINE bool send_msg(_ele_type & e, const _msg & msg)
	{
		auto_lock<thread_lock> lock(m_send_thread_lock);
		m_send_container.push_back(std::make_pair<e, msg>);
		return true;
	}
	FORCEINLINE bool recv_msg(_ele_type & e, _msg & msg)
	{
		auto_lock<thread_lock> lock(m_recv_thread_lock);
		if (m_recv_container.size() > 0)
		{
			e = m_recv_container.front().first;
			msg = m_recv_container.front().second;
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
	_real_container m_real_container;
};



