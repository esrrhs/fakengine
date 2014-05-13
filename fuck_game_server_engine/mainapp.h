#pragma once

class mainapp
{
public:
	mainapp(const char * name, int32_t cmdkey = c_DefaultCmdKey) : 
		m_engine(name), m_exit(false), m_cmdkey((shm_key)cmdkey)
	{
		LOG_SYS("new mainapp %s, %d", name, cmdkey);
	}
	virtual ~mainapp()
	{
	}
	force_inline void run(int argc, char *argv[])
	{
		LOG_SYS("run mainapp");
		if (!_ini())
		{
			LOG_ERROR("_ini fail");
			return;
		}
		if (!ini(argc, argv))
		{
			LOG_ERROR("ini fail");
			return;
		}
		while (!m_exit)
		{
			_heartbeat();
			heartbeat();
		}
		exit();
		_exit();
		LOG_SYS("exit mainapp");
	}
	std::string getname()
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
			LOG_ERROR("create_share_mem ret %d", m_cmdhandle);
			return false;
		}

		m_cmdcontrol = (cmdcontrol *)map_share_mem(m_cmdhandle);
		if (m_cmdcontrol == 0)
		{
			LOG_ERROR("map_share_mem fail %d", m_cmdhandle);
			return false;
		}

		LOG_SYS("ini cmd control %d %p", m_cmdhandle, m_cmdcontrol);
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
			LOG_SYS("get cmd control %s", m_cmdcontrol->get_cmd());
			if (m_cmdcontrol->is_cmd(c_CmdExit))
			{
				LOG_SYS("get cmd control,start exit");
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

	// cmd control
	shm_key m_cmdkey;
	shm_handle m_cmdhandle;
	cmdcontrol * m_cmdcontrol;
};


