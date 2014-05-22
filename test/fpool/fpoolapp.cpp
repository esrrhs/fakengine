#include "fuck_game_server_engine.h"
#include "fpoolapp.h"

bool fpoolapp::ini( int argc, char *argv[] )
{
	return true;
}

bool fpoolapp::heartbeat()
{
	fpool<stringc, 3> pool;

	int32_t index1 = pool.allocindex();
	int32_t index2 = pool.allocindex();
	int32_t index3 = pool.allocindex();
	int32_t index4 = pool.allocindex();

	fpool<stringc, 3>::iterator it = pool.begin();
	for (; it != pool.end();it++)
	{
	    stringc & itstr = *it;
	    itstr = rand();
	    uint32_t size = itstr.size();
	}

	stringc str1 = pool[index1];
	stringc str2 = pool[index2];
	const stringc & str33 = pool[index3];
	stringc str3;

	pool.deallocindex(index1);
	pool.deallocindex(index3);
	pool.deallocindex(index2);
	pool.deallocindex(index4);

	stringc * pstr1 = pool.alloc();
	*pstr1 = 123;
	stringc * pstr2 = pool.alloc();
	*pstr2 = 12412412411415;
	stringc * pstr3 = pool.alloc();
	*pstr3 = 0.1210f;

	uint32_t s = pool.size();

	pool.dealloc(*pstr1);
	pool.dealloc(*pstr2);
	pool.dealloc(*pstr3);

	s = pool.size();

	return true;
}

bool fpoolapp::exit()
{
	return true;
}

fpoolapp::fpoolapp() : mainapp("fpool")
{

}
