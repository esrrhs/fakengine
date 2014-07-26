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
#include "./sqlite/sqliteapp.h"
#include "./falloc/fallocapp.h"
#include "./fkeybuffer/fkeybufferapp.h"
#include "./fmd5/fmd5app.h"
#include "./fpoint/fpointapp.h"
#include "./pcre/pcreapp.h"
#include "./fproxyclient/fproxyclientapp.h"
#include "./fproto/fprotoapp.h"

char g_msg_buffer[MAX_MSG_SIZE];
Fproto::NetMsg g_netmsg;

#define REG(type) {\
	ifactory<mainapp> * p = fnew< factory<type, mainapp> >();\
	myfactorymng::ptr()->regist(#type, p);\
}

int main(int argc, char *argv[])
{
	REG(hashmapapp);
	REG(mysqlapp);
	REG(compressapp);
	REG(inifileapp);
	REG(ftrieapp);
	REG(clientapp);
	REG(serverapp);
	REG(fstringapp);
	REG(fpoolapp);
	REG(xmlapp);
	REG(sqliteapp);
	REG(fallocapp);
	REG(fkeybufferapp);
	REG(fmd5app);
	REG(fpointapp);
	REG(pcreapp);
	REG(fproxyclientapp);
	REG(fprotoapp);

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

	papp->run(argc - 2, argv + 2);

	myfactorymng::ptr()->dealloc(papp);
	
	return 0;
}
