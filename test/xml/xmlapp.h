#pragma once

class xmlapp : public mainapp
{
public:
	xmlapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
private:
};
