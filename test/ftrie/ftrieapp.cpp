#include "fuck_game_server_engine.h"
#include "ftrieapp.h"

bool ftrieapp::ini( int argc, char *argv[] )
{
	return true;
}

bool ftrieapp::heartbeat()
{
	myftrie ftrie;
	ftrie.insert((int8_t *)"ABC", 3);
	ftrie.insert((int8_t *)"AB", 2);
	ftrie.insert((int8_t *)"ABCD", 4);

	ftrie.ishaveword((int8_t *)"ABCDE", 5, false);

	return true;
}

bool ftrieapp::exit()
{
	return true;
}

ftrieapp::ftrieapp() : mainapp("ftrie")
{

}
