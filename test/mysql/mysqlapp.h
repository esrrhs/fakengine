#pragma once

class mysqlapp : public mainapp
{
public:
	mysqlapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
};
