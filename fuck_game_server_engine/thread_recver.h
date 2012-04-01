#pragma once

// tcp_thread_recver tcp������
// _msg ��Ϣ
// _container װ��Ϣ������
// _realrecver ��������Ϣ��ʵ��
template <typename _msg, typename _container, typename _realrecver>
class thread_recver : public thread
{
public:
	thread_recver()
	{
	}
	~thread_recver()
	{
	}
public:
	virtual void run()
	{
		while (1)
		{
			_msg * msg = 0;
			if (m_realrecver.recv_msg(msg))
			{
				auto_lock<thread_lock> lock(m_thread_lock);
				m_container.push_back(msg);
			}
			else
			{
				fsleep(1);
			}
		}
	}
public:
	FORCEINLINE bool recv_msg(_msg * msg)
	{
		auto_lock<thread_lock> lock(m_thread_lock);
		if (m_container.size() > 0)
		{
			msg = m_container.front();
			m_container.pop_front();
			return true;
		}
		return false;
	}
private:
	_container m_container;
	_realrecver m_realrecver;
	thread_lock m_thread_lock;
};

