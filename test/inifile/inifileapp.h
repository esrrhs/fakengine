#pragma once

class inifileapp : public mainapp
{
public:
	inifileapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
};

typedef inifile< std::map<std::string, std::map<std::string, std::string> >, std::map<std::string, std::string> > myinifile;
