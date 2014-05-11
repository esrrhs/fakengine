#include "fuck_game_server_engine.h"
#include "test.h"
#include "./hashmap/hashmapapp.h"
#include "./compress/compressapp.h"
#include "./inifile/inifileapp.h"
#include "./ftrie/ftrieapp.h"

int main(int argc, char *argv[])
{
	if (argc <= 1)
	{
		std::cout<<"need arg: [name]"<<std::endl;
		return 0;
	}
	
	std::string name = argv[1];
	mainapp * papp = 0;
	if (name == "hashmap")
	{
		papp = new hashmapapp();
	}
	else if (name == "compress")
	{
		papp = new compressapp();
	}
	else if (name == "inifile")
	{
		papp = new inifileapp();
	}
	else if (name == "ftrie")
	{
		papp = new ftrieapp();
	}
	else
	{
		std::cout<<"invalid "<<name<<std::endl;\
		return 0;
	}

	papp->run(argc, argv);
	
	return 0;
}