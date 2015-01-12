#pragma once

typedef inifile<10, 10> myinifile;
class inifileapp : public mainapp
{
public:
	inifileapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
private:
	myinifile ifile;
};
