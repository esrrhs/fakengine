#include "fakengine.h"
#include "fakeapp.h"

bool fakeapp::ini(int argc, char *argv[])
{
	return true;
}

bool fakeapp::heartbeat()
{
	fake * fk = newfake();
	delfake(fk);
	return true;
}

bool fakeapp::exit()
{
	return true;
}

fakeapp::fakeapp() : mainapp("fakeapp")
{

}
