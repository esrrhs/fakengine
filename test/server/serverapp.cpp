#include "fuck_game_server_engine.h"
#include "serverapp.h"

bool serverapp::ini( int argc, char *argv[] )
{
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
