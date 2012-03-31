#pragma once

class thread 
{
public:
	enum thread_status 
	{
		ts_ready,		// 当前线程处于准备状态
		ts_running,		// 处于运行状态
		ts_exiting,		// 线程正在退出
		ts_exit			// 已经退出 
	};
	thread() : m_id(0), m_status(thread::ts_ready)
#if defined(WIN32)
		,m_hthread(0)
#endif
	{
	}
	virtual ~thread()
	{
	}
public:
	fore_inline void start()
	{
		FASSERT(m_status == thread::ts_ready);

#if defined(WIN32)
		m_hthread = ::CreateThread(NULL, 0, thread::my_thread_process, this, 0, &m_id);
#else
		pthread_create(&m_id, NULL, thread::my_thread_process, this);
#endif
	}

	fore_inline void exit(void * retval = NULL )
	{
#if defined(WIN32)
		::CloseHandle(m_hthread);
#else
		pthread_exit(retval);
#endif
	}

	virtual void run() {}
	fore_inline thread_status get_status() { return m_status; }
	fore_inline void set_status(thread_status status) { m_status = status; }
	fore_inline t_id get_id () { return m_id; }

#if defined(WIN32)
	static DWORD WINAPI my_thread_process(void * derivedThread)
#else
	static void * my_thread_process(void * derivedThread)
#endif
	{
		thread * t = (thread*)derivedThread;
		FASSERT(t);

		t->set_status(thread::ts_running);

		t->run();

		t->set_status(thread::ts_exiting);

		t->exit(0);

		return 0;	// avoid compiler's warning
	}
private:
	// thread identifier variable
	t_id m_id;
	
	// thread status
	thread_status m_status;

#if defined(WIN32)
	HANDLE m_hthread ;
#endif
};


