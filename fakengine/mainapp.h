#pragma once

class mainapp
{
public:
	mainapp(const char * name, int32_t cmdkey = c_DefaultCmdKey, uint8_t fps = c_DefaultFps) :
		m_engine(name), m_exit(false), m_fps(fps), m_cmdkey((shm_key)cmdkey)
	{
		LOG_SYS(FENGINE_HEADER "new mainapp %s, %d, %u", name, cmdkey, fps);
	}
	virtual ~mainapp()
	{
	}
	force_inline void run(int argc, char *argv[])
	{
		LOG_SYS(FENGINE_HEADER "run mainapp");
		if (!_ini())
		{
			LOG_ERROR(FENGINE_HEADER "_ini fail");
			return;
		}
		if (!ini(argc, argv))
		{
			LOG_ERROR(FENGINE_HEADER "ini fail");
			return;
		}
		uint32_t last = get_ms_tick();
		uint32_t now = last;
		const uint32_t tps = 1000 / m_fps;
		while (!m_exit)
		{
			now = get_ms_tick();
			if (now >= last && (now - last) < tps)
			{
				fsleep(1);
				continue;
			}
			last = now;
			_heartbeat();
			heartbeat();
		}
		exit();
		_exit();
		LOG_SYS(FENGINE_HEADER "exit mainapp");
	}
	const stringc & getname()
	{
		return m_engine.getname();
	}
	const stringc & gettype()
	{
		return m_engine.getname();
	}
protected:
	virtual bool ini(int argc, char *argv[]) = 0;
	virtual bool heartbeat() = 0;
	virtual bool exit() = 0;
private:
	force_inline bool _ini()
	{
		// ini cmd control
		m_cmdhandle = create_share_mem(m_cmdkey, sizeof(cmdcontrol));
		if (m_cmdhandle <= 0)
		{
			LOG_ERROR(FENGINE_HEADER "create_share_mem ret %d", m_cmdhandle);
			return false;
		}

		m_cmdcontrol = (cmdcontrol *)map_share_mem(m_cmdhandle);
		if (m_cmdcontrol == 0)
		{
			LOG_ERROR(FENGINE_HEADER "map_share_mem fail %d", m_cmdhandle);
			return false;
		}

		LOG_SYS(FENGINE_HEADER "ini cmd control %d %p", m_cmdhandle, m_cmdcontrol);
		memset(m_cmdcontrol, 0, sizeof(cmdcontrol));

		return true;
	}
	force_inline bool _heartbeat()
	{
		// clock
		fclock::ptr()->heartbeat();

		// check cmd control
		checkcmdcontrol();
		
		return true;
	}
	force_inline bool checkcmdcontrol()
	{
		if (m_cmdcontrol->is_set())
		{
			LOG_SYS(FENGINE_HEADER "get cmd control %s", m_cmdcontrol->get_cmd());
			if (m_cmdcontrol->is_cmd(c_CmdExit))
			{
				LOG_SYS(FENGINE_HEADER "get cmd control,start exit");
				m_exit = true;
			}
			m_cmdcontrol->clear();
		}
		return true;
	}
	force_inline bool _exit()
	{
		unmap_share_mem((int8_t*)m_cmdcontrol);
		close_share_mem(m_cmdhandle);
		return true;
	}
private:
	fengine m_engine;
	bool m_exit;
	uint8_t m_fps;

	// cmd control
	shm_key m_cmdkey;
	shm_handle m_cmdhandle;
	cmdcontrol * m_cmdcontrol;
};


