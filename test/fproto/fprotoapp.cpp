#include "fuck_game_server_engine.h"
#include "fprotoapp.h"

bool fprotoapp::ini( int argc, char *argv[] )
{
	return true;
}

bool fprotoapp::heartbeat()
{
	fproto fp;
	bool ret = fp.parse("../tools/genmsg/struct.xml");
	if (!ret)
	{
		FPRINTF("%s\n", fp.err().c_str());
	}
	return true;
}

bool fprotoapp::exit()
{
	return true;
}

fprotoapp::fprotoapp() : mainapp("fprotoapp")
{

}