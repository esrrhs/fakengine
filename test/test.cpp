#include "fuck_game_server_engine.h"
#include "test.h"
#include "./hashmap/hashmapapp.h"
#include "./compress/compressapp.h"
#include "./inifile/inifileapp.h"
#include "./ftrie/ftrieapp.h"
#include "./client/clientapp.h"
#include "./server/serverapp.h"
#include "./mysql/mysqlapp.h"
#include "./fstring/fstringapp.h"

class IFactory
{
public:
	virtual mainapp * Alloc() = 0;
};

template <typename T>
class Factory : public IFactory
{
public:
	virtual mainapp * Alloc()
	{
		return new T();
	}
};

std::map<stringc, IFactory *> g_map;

#define REG(type) {\
	IFactory * p = new Factory<type>();\
	g_map[#type] = p;\
}

int main(int argc, char *argv[])
{
	REG(hashmapapp);
	REG(compressapp);
	REG(inifileapp);
	REG(ftrieapp);
	REG(clientapp);
	REG(serverapp);
	REG(fstringapp);

	if (argc <= 1)
	{
		std::cout<<"need arg: [name]"<<std::endl;
		return 0;
	}
	
	stringc name = argv[1];

	IFactory * pFactory = 0;
	pFactory = g_map[name];
	if (!pFactory)
	{
		std::cout<<"invalid "<<name.c_str()<<std::endl;\
		return 0;
	}

	mainapp * papp = pFactory->Alloc();

	papp->run(argc, argv);
	
	return 0;
}
