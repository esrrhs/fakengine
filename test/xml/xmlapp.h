#pragma once

#include "../tools/genxml/code.h"
#include "../tools/gencfg/SampleCfgLoader.h"

class xmlapp : public mainapp
{
public:
	xmlapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
private:
};
