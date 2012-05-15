#pragma once

class fperf_manager
{
public:
	force_inline void sart_profile(const char* name)
	{

	}
	force_inline void stop_profile()
	{

	}
};

// ²ÉÑù
class fperf_sample
{
public:
	force_inline fperf_sample(fperf_manager & manager, const char * name) :
	m_manager(manager)
	{ 
		m_manager.sart_profile(name); 
	}

	force_inline ~fperf_sample()					
	{ 
		m_manager.stop_profile(); 
	}
private:
	fperf_manager & m_manager;
};

extern THREAD_LOCAL_VALUE fperf_manager g_fperf_manager;

#ifdef USE_PROFILE
#define	PERF(manager, name) fperf_sample _fperf_sample((manager), (name))
#else
#define	PERF(manager, name)
#endif

#define PERF_FUNC(manager) PERF(manager, __FUNCTION__)
#define PERF_DEFAULT(name) PERF(g_fperf_manager, name)
#define PERF_DEFAULT_FUNC() PERF(g_fperf_manager, __FUNCTION__)

