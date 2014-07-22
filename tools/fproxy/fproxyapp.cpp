#include "fuck_game_server_engine.h"
#include "common.h"
#include "fproxyapp.h"

bool fproxyapp::ini(int argc, char *argv[])
{
	mycmdparser cp;
	cp.parse(argc, argv);
	stringc ip;
	cp.get("ip", ip);
	int32_t port;
	cp.get("port", port);
	LOG_DEBUG("%s:%d", ip.c_str(), port);

	tcp_socket_server_param param;
	param.ip = ip;
	param.port = port;
	param.socket_send_buffer_size = 1024 * 1024;
	param.socket_recv_buffer_size = 1024 * 1024;

	bool ret = m_mynetserver.ini(param);
	if (!ret)
	{
		return false;
	}

	return true;
}

bool fproxyapp::heartbeat()
{
	m_mynetserver.tick();
	return true;
}

bool fproxyapp::exit()
{
	return true;
}

fproxyapp::fproxyapp() : mainapp("fproxyapp")
{

}
