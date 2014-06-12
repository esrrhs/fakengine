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
#include "./fpool/fpoolapp.h"
#include "./xml/xmlapp.h"

char g_msg_buffer[MAX_MSG_SIZE];
Protocol::NetMsg g_netmsg;

#define REG(type) {\
	ifactory<mainapp> * p = fnew< factory<type, mainapp> >();\
	myfactorymng::ptr()->regist(#type, p);\
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
	REG(fpoolapp);
	REG(xmlapp);
	
	if (argc <= 1)
	{
		std::cout<<"need arg: [name]"<<std::endl;
		return 0;
	}
	
	stringc name = argv[1];

	mainapp * papp = myfactorymng::ptr()->alloc(name);
	if (!papp)
	{
		std::cout<<"invalid "<<name.c_str()<<std::endl;\
			return 0;
	}

	papp->run(argc - 1, argv + 1);

	myfactorymng::ptr()->dealloc(papp);
	
	return 0;
}
