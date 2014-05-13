#include "fuck_game_server_engine.h"
#include "test.h"
#include "./hashmap/hashmapapp.h"
#include "./compress/compressapp.h"
#include "./inifile/inifileapp.h"
#include "./ftrie/ftrieapp.h"
#include "./client/clientapp.h"
#include "./server/serverapp.h"

std::map<std::string, mainapp *> g_map;

#define REG(type) {\
	mainapp * p = new type##app();\
	g_map[p->getname()] = p;\
}

int main(int argc, char *argv[])
{
	REG(hashmap);
	REG(compress);
	REG(inifile);
	REG(ftrie);
	REG(client);
	REG(server);

	if (argc <= 1)
	{
		std::cout<<"need arg: [name]"<<std::endl;
		return 0;
	}
	
	std::string name = argv[1];

	mainapp * papp = 0;
	papp = g_map[name];
	if (!papp)
	{
		std::cout<<"invalid "<<name<<std::endl;\
		return 0;
	}

	papp->run(argc, argv);
	
	return 0;
}
