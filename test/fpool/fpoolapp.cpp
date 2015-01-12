#include "fuck_game_server_engine.h"
#include "fpoolapp.h"

bool fpoolapp::ini( int argc, char *argv[] )
{
	return true;
}

bool fpoolapp::heartbeat()
{
    fstack<int, 3> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
	for (fstack<int, 3>::iterator it = stack.begin(); it != stack.end();it++)
	{
	    int n = *it;
	    n++;
	}
	int tmp;
	stack.pop(tmp);
	for (fstack<int, 3>::iterator it = stack.begin(); it != stack.end();it++)
	{
	    int n = *it;
	    n++;
	}

	fpool<stringc, 3> pool;

	int32_t index1 = pool.allocindex();
	int32_t index2 = pool.allocindex();
	int32_t index3 = pool.allocindex();
	int32_t index4 = pool.allocindex();

	int i = 0;
	for (fpool<stringc, 3>::iterator it = pool.begin(); it != pool.end();it++)
	{
	    stringc & itstr = *it;
	    itstr = i++;
	    uint32_t size = itstr.size();
		size = 0;
	}

	stringc str1 = pool[index1];
	stringc str2 = pool[index2];
	const stringc & str33 = pool[index3];
	FUSE(str33);
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

	pool.dealloc(*pstr2);
	for (fpool<stringc, 3>::iterator it = pool.begin(); it != pool.end();it++)
	{
	    stringc & itstr = *it;
	    uint32_t size = itstr.size();
		size = 0;
	}
	pool.dealloc(*pstr1);
	for (fpool<stringc, 3>::iterator it = pool.begin(); it != pool.end();it++)
	{
	    stringc & itstr = *it;
	    uint32_t size = itstr.size();
		size = 0;
	}
	pool.dealloc(*pstr3);
	for (fpool<stringc, 3>::iterator it = pool.begin(); it != pool.end();it++)
	{
	    stringc & itstr = *it;
	    uint32_t size = itstr.size();
		size = 0;
	}

	s = pool.size();

	return true;
}

bool fpoolapp::exit()
{
	return true;
}

fpoolapp::fpoolapp() : mainapp("fpoolapp")
{

}
