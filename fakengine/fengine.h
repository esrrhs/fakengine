#pragma once

class fengine
{
public:
	fengine(const char * name) : m_name(name)
	{
		ini();
	}
	~fengine()
	{
		exit();
	}
	force_inline const stringc & getname()
	{
		return m_name;
	}
private:
	force_inline bool ini()
	{
		fhook_all_func();

		if (g_ini)
		{
			LOG_ERROR(FENGINE_HEADER "re ini");
			return false;
		}

		g_ini = true;

		flog_system::ptr()->setname(m_name);

		LOG_SYS(FENGINE_HEADER "version[%d][%s] lastlog[%s]", fversion(),
			(const char *)fversionstr().c_str(),
			(const char *)flastlog().c_str());

		// socket
#ifdef WIN32
		WORD wVersionRequested;
		WSADATA wsaData;
		int32_t err;
		wVersionRequested = MAKEWORD(2, 2);
		err = WSAStartup(wVersionRequested, &wsaData);
		if (err != 0)
		{
			LOG_ERROR(FENGINE_HEADER "WSAStartup error\n");
			return false;
		}
#endif

		// lzo
		int ret = lzo_init();
		if (LZO_E_OK != ret)
		{
			LOG_ERROR(FENGINE_HEADER "lzo_init error\n");
			return false;
		}

		// ini
		g_start_time = get_s_tick();

		start_profiler((const char *)(m_name + ".prof").c_str());

		RandInit(fclock::Cycles());
		SRandInit(fclock::Cycles());

		LOG_SYS(FENGINE_HEADER "fengine ini ok");

		return true;
	}
	force_inline bool exit()
	{
		stop_profiler();

		LOG_SYS(FENGINE_HEADER "fengine exit ok");

		frestore_all_func();

		return true;
	}
private:
	stringc m_name;
};

