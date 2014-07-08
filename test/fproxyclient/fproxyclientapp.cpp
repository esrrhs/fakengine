#include "fuck_game_server_engine.h"
#include "fproxyclientapp.h"

bool fproxyclientapp::ini( int argc, char *argv[] )
{
	return true;
}

bool fproxyclientapp::heartbeat()
{
	return true;
}

bool fproxyclientapp::exit()
{
	return true;
}

fproxyclientapp::fproxyclientapp() : mainapp("fproxyclientapp")
{

}
