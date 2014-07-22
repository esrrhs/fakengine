#include "fuck_game_server_engine.h"
#include "clientapp.h"

bool clientapp::ini( int argc, char *argv[] )
{
	tcp_socket_link_param param;
	bool ret = m_mynetlink.ini(param);
	if (!ret)
	{
		return false;
	}
	return true;
}

bool clientapp::heartbeat()
{
	m_mynetlink.tick();
	return true;
}

bool clientapp::exit()
{
	return true;
}

clientapp::clientapp() : mainapp("clientapp", c_DefaultCmdKey + 1)
{

}
