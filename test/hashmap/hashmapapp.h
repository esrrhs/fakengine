#pragma once

class hashmapapp : public mainapp
{
public:
	hashmapapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
};
