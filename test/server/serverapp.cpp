#include "fakengine.h"
#include "serverapp.h"

bool serverapp::ini( int argc, char *argv[] )
{
    mycmdparser cp;
    cp.parse(argc, argv);
    stringc ip;
    cp.get("ip", ip);
    int32_t port;
    cp.get("port", port);
	LOG_DEBUG("%s:%d", ip.c_str(), port);

	tcp_socket_server_param param;
	bool ret = m_mynetserver.ini(param);
	if (!ret)
	{
		return false;
	}
	return true;
}

bool serverapp::heartbeat()
{
	m_mynetserver.tick();
	return true;
}

bool serverapp::exit()
{
	return true;
}

serverapp::serverapp() : mainapp("serverapp")
{

}
