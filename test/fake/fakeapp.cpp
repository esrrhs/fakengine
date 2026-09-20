#include "fakengine.h"
#include "fakeapp.h"

bool fakeapp::ini(int argc, char *argv[])
{
	return true;
}

bool fakeapp::heartbeat()
{
#ifdef HAVE_FAKESCRIPT
	fake * fk = newfake();
	delfake(fk);
#endif
	return true;
}

bool fakeapp::exit()
{
	return true;
}

fakeapp::fakeapp() : mainapp("fakeapp")
{

}
