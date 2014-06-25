#include "fuck_game_server_engine.h"

bool fengine::ini()
{
	if (g_ini)
	{
		LOG_ERROR("re ini\n");
		return false;
	}

	g_ini = true;

#ifndef WIN32
#ifdef USE_ALLOC_HOOK
    void* (*__malloc_hook)(size_t, const void*) = &glibc_override_malloc;
    void* (* __realloc_hook)(void*, size_t, const void*) = &glibc_override_realloc;
    void (* __free_hook)(void*, const void*) = &glibc_override_free;
    void* (* __memalign_hook)(size_t,size_t, const void*) = &glibc_override_memalign;
    FUSE(__malloc_hook);
    FUSE(__realloc_hook);
    FUSE(__free_hook);
    FUSE(__memalign_hook);
#endif
#endif

	flog_system::ptr()->setname((const int8_t*)(m_name + ".log").c_str());

	// logo
	LOG_SYS(
		"\n*******************************\n"
		"| FFFFF  U  U  CCCC  K  K   |\n"
		"| F      U  U  C     K K    |\n"
		"| FFFFF  U  U  C     KK     |\n"
		"| F      U  U  C     K K    |\n"
		"| F      UUUU  CCCC  K  K   |\n"
		"*******************************\n");

	// socket
#ifdef WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	int32_t err;
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		LOG_ERROR("WSAStartup error\n");
		return false;
	}
#endif

	// lzo
	int ret = lzo_init();
	if (LZO_E_OK != ret)
	{
		LOG_ERROR("lzo_init error\n");
		return false;
	}

	// ini
	g_start_time = get_s_tick();

	start_profiler((const char *)(m_name + ".prof").c_str());

	RandInit( fclock::Cycles() );
	SRandInit( fclock::Cycles() );
		
	LOG_SYS("fengine ini ok");

	return true;
}

bool fengine::exit()
{
	stop_profiler();

	LOG_SYS("fengine exit ok");

	return true;
}

