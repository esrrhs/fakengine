#include "fuck_game_server_engine.h"
#include "serverapp.h"

bool serverapp::ini( int argc, char *argv[] )
{
	return true;
}

bool serverapp::heartbeat()
{
	
	return true;
}

bool serverapp::exit()
{
	return true;
}

serverapp::serverapp() : mainapp("server")
{

}
