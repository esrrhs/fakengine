#include "fakengine.h"
#include "ftrieapp.h"

bool ftrieapp::ini( int argc, char *argv[] )
{
	return true;
}

bool ftrieapp::heartbeat()
{
	myftrie ftrie;
	ftrie.insert("ABC");
	ftrie.insert("ABCD");

	int i = ftrie.ishaveword("ABCDE");
	stringc tmp("ABCDEFGABABCABCCDABCDD");
	myftrie::ftriestring tmp1 = tmp;
	int j = ftrie.filterword(tmp1, '*');

	ftrie.erase("ABC");
	myftrie::ftriestring tmp2 = tmp;
	j = ftrie.filterword(tmp2, '*');
	ftrie.erase("ABCD");
	i = ftrie.ishaveword("ABCDE");

	return true;
}

bool ftrieapp::exit()
{
	return true;
}

ftrieapp::ftrieapp() : mainapp("ftrieapp")
{

}
