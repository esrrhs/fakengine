#pragma once

// thread_container 连接服务端类
template <typename _msg, typename _ele_container, typename _real_container>
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
		recv_max_pac_per_frame = 50,
		send_max_pac_per_frame = 50,
	};
	typedef typename _ele_container::mapped_type _msg_container;
	typedef typename _real_container::ele_type ele_type;
	typedef typename _real_container::ele_iter ele_iter;
public:
	virtual void run()
	{
		_msg tmp;
		int32_t i = 0;
		while (1)
		{
			m_real_container.tick();

			// recv
			for (ele_iter eb = m_real_container.ebegin(); eb != m_real_container.eend(); eb++)
			{
				auto_lock<thread_lock> lock(m_recv_thread_lock);

				ele_type & e = *eb;
				_msg_container & mc = m_recv_container[&e];

				i = 0;
				while (m_real_container.recv_msg(&e, tmp) && i < recv_max_pac_per_frame)
				{
					mc.push_back(tmp);
					i++;
				}
			}
			
			// send
			{
				auto_lock<thread_lock> lock(m_send_thread_lock);

				for (_ele_container::iterator it = m_send_container.begin(); 
					it != m_send_container.end(); 
					it++)
				{
					ele_type * e = it->first;
					_msg_container & mc = it->second;

					i = 0;
					while(i < send_max_pac_per_frame)
					{
						if (mc.size() > 0)
						{
							_msg & msg = mc.front();
							if (m_real_container.send_msg(e, msg))
							{
								mc.pop_front();
								i++;
							}
							else
							{
								break;
							}
						}
						else
						{
							break;
						}
					}
				}
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
	FORCEINLINE bool send_msg(ele_type * e, const _msg & msg)
	{
		auto_lock<thread_lock> lock(m_send_thread_lock);
		_msg_container & mc = m_send_container[e];
		mc.push_back(msg);
		return true;
	}
	FORCEINLINE bool recv_msg(ele_type * e, _msg & msg)
	{
		auto_lock<thread_lock> lock(m_recv_container);
		_msg_container & mc = m_recv_container[e];
		if (mc.size() > 0)
		{
			msg = mc.front();
			mc.pop_front();
			return true;
		}
		return false;
	}
private:
	_ele_container m_send_container;
	_ele_container m_recv_container;
	thread_lock m_send_thread_lock;
	thread_lock m_recv_thread_lock;
	_real_container m_real_container;
};



