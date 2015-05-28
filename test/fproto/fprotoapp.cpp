#include "fakengine.h"
#include "fprotoapp.h"

bool fprotoapp::ini( int argc, char *argv[] )
{
	return true;
}

bool fprotoapp::heartbeat()
{
	return true;
}

bool fprotoapp::exit()
{
	return true;
}

fprotoapp::fprotoapp() : mainapp("fprotoapp")
{

}
