#include "fuck_game_server_engine.h"
#include "fproxyapp.h"

bool fproxyapp::ini(int argc, char *argv[])
{
	return true;
}

bool fproxyapp::heartbeat()
{
	return true;
}

bool fproxyapp::exit()
{
	return true;
}

fproxyapp::fproxyapp() : mainapp("fproxyapp")
{

}
