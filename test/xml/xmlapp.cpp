#include "fuck_game_server_engine.h"
#include "xmlapp.h"

bool xmlapp::ini( int argc, char *argv[] )
{
	return true;
}

bool xmlapp::heartbeat()
{
	return true;
}

bool xmlapp::exit()
{
	return true;
}

xmlapp::xmlapp() : mainapp("xmlapp")
{

}
