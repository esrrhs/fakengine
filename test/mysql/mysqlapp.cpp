#include "fuck_game_server_engine.h"
#include "mysqlapp.h"

bool mysqlapp::ini( int argc, char *argv[] )
{
	return true;
}

bool mysqlapp::heartbeat()
{
	
	return true;
}

bool mysqlapp::exit()
{
	return true;
}

mysqlapp::mysqlapp() : mainapp("mysql")
{

}
