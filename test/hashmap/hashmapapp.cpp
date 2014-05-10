#include "fuck_game_server_engine.h"
#include "hashmapapp.h"

bool hashmapapp::ini( int argc, char *argv[] )
{
	return true;
}

bool hashmapapp::heartbeat()
{
	fhashmap<int, int, 100, 32> testhashmap;
	std::map<int, int> testmap;
	for (int i = 0; i < 10000; i++)
	{
		int k = rand();
		int v = rand();
		testmap[k] = v;
		testhashmap.add(k, v); 
	}
	for (int i = 0; i < 10000; i++)
	{
		int k = rand();
		testmap.erase(k);
		testhashmap.del(k); 
	}
	if (testhashmap.data_size() != testmap.size())
	{
		std::cout<<"test hash map size error"<<testhashmap.data_size()<<" "<<testmap.size()<<std::endl;
	}
	for (std::map<int, int>::iterator it = testmap.begin();
		it != testmap.end();
		it++)
	{
		int k = it->first;
		int v = it->second;
		if (v != testhashmap.find(k))
		{
			std::cout<<"test hash map error"<<k<<" "<<v<<" "<<testhashmap.find(k)<<std::endl;
		}
	}
	return true;
}

bool hashmapapp::exit()
{
	return true;
}

hashmapapp::hashmapapp() : mainapp("hashmap")
{

}
