#include "fuck_game_server_engine.h"
#include "inifileapp.h"

bool inifileapp::ini( int argc, char *argv[] )
{
	return true;
}

bool inifileapp::heartbeat()
{
	int8_t param1 = 123;
	int16_t param2 = 1234;
	int32_t param3 = 123456;
	int64_t param4 = 1234567890;
	float param5 = 1.34f;
	stringc param6 = "aagagag";
	stringc param7;
	fkeybuffer<1024> buff;
	buff.begin("test");
	buff.add("param1", param1);
	buff.add("param2", param2);
	buff.add("param3", param3);
	buff.add("param4", param4);
	buff.add("param5", param5);
	buff.add("param6", param6);
	buff.add("param7", param7);

	fkeybuffer<300> subbuff;
	subbuff.begin("subtest");
	subbuff.add("subparam1", param1);
	subbuff.add("subparam2", param2);
	subbuff.end();

	buff.add("subbuff", subbuff.buff(), subbuff.size());

	buff.end();

	//////////////////////////////////////////////////////////////////////////
	int8_t tmpparam1;
	int16_t tmpparam2;
	int32_t tmpparam3;
	int64_t tmpparam4;
	float tmpparam5;
	stringc tmpparam6;
	stringc tmpparam7;
	buff.get("param3", tmpparam3);
	buff.get("param6", tmpparam6);
	buff.get("param2", tmpparam2);
	buff.get("param1", tmpparam1);
	buff.get("param4", tmpparam4);
	buff.get("param7", tmpparam7);
	buff.get("param5", tmpparam5);

	ifile.clear();
	ifile.load("config.ini");
	stringc ss;
	ifile.get("sec0", "key0", ss);

	std::cout<<"."<<ss.c_str();
	return true;
}

bool inifileapp::exit()
{
	return true;
}

inifileapp::inifileapp() : mainapp("inifileapp")
{

}
