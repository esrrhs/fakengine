#include "fuck_game_server_engine.h"
#include "clientapp.h"

bool clientapp::ini( int argc, char *argv[] )
{
	return true;
}

bool clientapp::heartbeat()
{
	
	return true;
}

bool clientapp::exit()
{
	return true;
}

clientapp::clientapp() : mainapp("client")
{

}
