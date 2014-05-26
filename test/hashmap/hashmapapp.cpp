#include "fuck_game_server_engine.h"
#include "hashmapapp.h"

bool hashmapapp::ini( int argc, char *argv[] )
{
	return true;
}

bool hashmapapp::heartbeat()
{
	typedef fhashset<int,1141> myhashset;
    myhashset testset;
    int size = myhashset::SIZE;
    testset.insert(1);
    testset.insert(1544);
    testset.insert(3);
    testset.insert(1);
	myhashset::iterator it = testset.find(1);
	if (it != testset.end())
	{
		int a = *it;
		a++;
	}
	it = testset.find(2);
	bool b = (it == testset.end());
    
	for (myhashset::iterator it = testset.begin(); it != testset.end(); it++)
	{
		int a = *it;
		a++;
	}

	testset.erase(2);
	for (myhashset::iterator it = testset.begin(); it != testset.end(); it++)
	{
		int a = *it;
		a++;
	}

	testset.erase(3);
	for (myhashset::iterator it = testset.begin(); it != testset.end(); it++)
	{
		int a = *it;
		a++;
	}

	testset.erase(1544);
	for (myhashset::iterator it = testset.begin(); it != testset.end(); it++)
	{
		int a = *it;
		a++;
	}

	it = testset.find(1);
	b = (it == testset.end());
	int aa = *it;

	testset.erase(1);
	for (myhashset::iterator it = testset.begin(); it != testset.end(); it++)
	{
		int a = *it;
		a++;
	}
	size = testset.size();

	testset.insert(3);
	int ss = testset[3];
	for (myhashset::iterator it = testset.begin(); it != testset.end(); it++)
	{
		int a = *it;
		a++;
	}
	int sss = testset[4];
	for (myhashset::iterator it = testset.begin(); it != testset.end(); it++)
	{
		int a = *it;
		a++;
	}

	//////////////////////////////////////////////
    {
	    typedef fhashmap<int, int, 3> myhashmap;
	    myhashmap testhashmap;
	    testhashmap.insert(1, 3);
	    testhashmap.insert(1, 5);
    	for (myhashmap::iterator it = testhashmap.begin(); it != testhashmap.end(); it++)
    	{
    		int a = it->first;
    		int b = it->second;
    		a++;
    		b++;
    	}
    	testhashmap.erase(1);
    	for (myhashmap::iterator it = testhashmap.begin(); it != testhashmap.end(); it++)
    	{
    		int a = it->first;
    		int b = it->second;
    		a++;
    		b++;
    	}
    	testhashmap[1] = 4;
    	testhashmap[11] = 14;
    	for (myhashmap::iterator it = testhashmap.begin(); it != testhashmap.end(); it++)
    	{
    		int a = it->first;
    		int b = it->second;
    		a++;
    		b++;
    	}
    }
	
	//////////////////////////////////////////////

	fhashmap<int, int, 10000> testhashmap;
	std::map<int, int> testmap;
	for (int i = 0; i < 10000; i++)
	{
		int k = rand();
		int v = rand();
		testmap[k] = v;
		testhashmap[k] = v;
	}
	for (int i = 0; i < 10000; i++)
	{
		int k = rand();
		testmap.erase(k);
		testhashmap.erase(k); 
	}
	if (testhashmap.size() != testmap.size())
	{
		std::cout<<"test hash map size error"<<testhashmap.size()<<" "<<testmap.size()<<std::endl;
	}
	for (std::map<int, int>::iterator it = testmap.begin();
		it != testmap.end();
		it++)
	{
		int k = it->first;
		int v = it->second;
		fhashmap<int, int, 10000>::iterator ittest = testhashmap.find(k);
		int v_test = ittest->second;
		if (v != v_test)
		{
			std::cout<<"test hash map error"<<k<<" "<<v<<" "<<v_test<<std::endl;
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
