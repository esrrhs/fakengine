#include "fakengine.h"
#include "fallocapp.h"

bool fallocapp::ini( int argc, char *argv[] )
{
	return true;
}

bool fallocapp::heartbeat()
{
	void * pppp = calloc(10, 12);
	free(pppp);

    int * pp = new int;
	delete pp;

	pp = new int[10];
	delete[] pp;

    void * p1 = malloc(10);
	p1 = realloc(p1, 123);
    free(p1);
	    
    p1 = falloc(2);
    ffree(p1);

    void * p = 0;

    p = frealloc(0, 2);
    frealloc(p, 0);

    p = falloc(22);
    p = frealloc(p, 10);
    p = frealloc(p, 100);
    p = frealloc(p, 100*1024);
    ffree(p);
    
    p = fmemalign(1024, 10240);
    FASSERT(p % 1024 == 0);
	ffree(p);
    
    p = fmemalign(2, 98);
    FASSERT(p % 2 == 0);
	ffree(p);
    
	p = falloc(20);
	size_t size = fmemsize(p);
	FUSE(size);
    FASSERT(size == 20);
	ffree(p);
	
	p = falloc(21);
	ffree(p);
	p = falloc(19);
	ffree(p);

    p = falloc(102349);
    ffree(p);

	for (int i = 1; i < 99*1024;i += 32)
	{
		void * p = falloc(i);
		//memset(p, 1, i);
    	size_t size = fmemsize(p);
    	FUSE(size);
		ffree(p);
	}

	for (int i = 1; i < 99*1024;i += 32)
	{
	    for (int j = 2; j <= 1024 && j < i; j += 32)
	    {
		    void * p = fmemalign(j, i);
    		//memset(p, 1, i);
        	size_t size = fmemsize(p);
        	FUSE(size);
    		ffree(p);
	    }
	}
	
	return true;
}

bool fallocapp::exit()
{
	return true;
}

fallocapp::fallocapp() : mainapp("fallocapp")
{

}
