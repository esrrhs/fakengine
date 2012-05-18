#include "fuck_game_server_engine.h"

bool fengine::ini()
{
	// logo
	FPRINTF("                          \n		\
	*******************************\n		\
	|    zzzzzzzzz   x       x    |\n		\
	|           z     x     x     |\n		\
	|          z       x   x      |\n		\
	|         z         x x       |\n		\
	|        z           x        |\n		\
	|       z           x x       |\n		\
	|      z           x   x      |\n		\
	|     z           x     x     |\n		\
	|    zzzzzzzzz   x       x    |\n		\
	*******************************\n");

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

	return true;
}

bool fengine::exit()
{
	return true;
}

