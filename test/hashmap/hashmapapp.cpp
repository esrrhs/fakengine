#include "fakengine.h"
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
	aa = 0;

	testset.erase(1);
	for (myhashset::iterator it = testset.begin(); it != testset.end(); it++)
	{
		int a = *it;
		a++;
	}
	size = testset.size();

	testset.insert(3);
	int ss = testset[3];
	ss = 0;
	for (myhashset::iterator it = testset.begin(); it != testset.end(); it++)
	{
		int a = *it;
		a++;
	}
	int sss = testset[4];
	sss = 0;
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

	{

		typedef farray<int,3> myarray;
		myarray testarray;
		testarray.push_back(1);
		testarray.push_back(2);
		testarray.push_back(3);

		for (myarray::iterator it = testarray.begin(); it != testarray.end(); it++)
		{
			int a = *it;
			a++;
		}

		int aa;
		testarray.front(aa);

		int aaa;
		testarray.back(aaa);

		int i;
		testarray.pop_front(i);
		testarray.pop_back(i);
		testarray.pop_front(i);

		testarray.push_back(1);
		testarray.push_back(2);
		testarray.push_back(3);
		testarray.push_back(4);

		testarray.erase(1);
		for (myarray::iterator it = testarray.begin(); it != testarray.end(); it++)
		{
			int a = *it;
			a++;
		}

		testarray.erase(1);
		for (myarray::iterator it = testarray.begin(); it != testarray.end(); it++)
		{
			int a = *it;
			a++;
		}

		testarray.erase(0);
		for (myarray::iterator it = testarray.begin(); it != testarray.end(); it++)
		{
			int a = *it;
			a++;
		}

		testarray.insert(0, 3);
		for (myarray::iterator it = testarray.begin(); it != testarray.end(); it++)
		{
			int a = *it;
			a++;
		}

		testarray.insert(0, 1);
		for (myarray::iterator it = testarray.begin(); it != testarray.end(); it++)
		{
			int a = *it;
			a++;
		}

		testarray.insert(0, 2);
		for (myarray::iterator it = testarray.begin(); it != testarray.end(); it++)
		{
			int a = *it;
			a++;
		}

		i = testarray.size();
		i = 0;
	}

	{
		typedef flist<int, 3> mylist;
		mylist list;
		list.push_back(2);
		list.push_front(1);
		list.push_back(3);
		for (mylist::iterator it = list.begin(); it != list.end(); it++)
		{
			int a = *it;
			a++;
		}

		int aa;
		list.back(aa);
		list.front(aa);
		list.pop_back(aa);
		list.pop_front(aa);
		for (mylist::iterator it = list.begin(); it != list.end(); it++)
		{
			int a = *it;
			a++;
		}
	}

	return true;
}

bool hashmapapp::exit()
{
	return true;
}

hashmapapp::hashmapapp() : mainapp("hashmapapp")
{

}
