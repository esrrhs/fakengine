#pragma once

class inifileapp : public mainapp
{
public:
	inifileapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
};

typedef inifile<10, 10> myinifile;
