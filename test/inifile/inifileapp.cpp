#include "fakengine.h"
#include "inifileapp.h"
#include "../tools/genmsg/Struct.h"

bool inifileapp::ini( int argc, char *argv[] )
{
	return true;
}

bool inifileapp::heartbeat()
{
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
