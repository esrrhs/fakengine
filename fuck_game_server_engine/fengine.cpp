#include "fuck_game_server_engine.h"

bool fengine::ini()
{
	// logo
	FPRINTF(
		"*******************************\n"
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
		FPRINTF("WSAStartup error\n");
		return false;
	}
#endif

	// lzo
	int ret = lzo_init();
	if (LZO_E_OK != ret)
	{
		FPRINTF("lzo_init error\n");
		return false;
	}

	// ini
	g_start_time = get_s_tick();

	start_profiler((m_name + ".prof").c_str());

	return true;
}

bool fengine::exit()
{
	stop_profiler();

	return true;
}

