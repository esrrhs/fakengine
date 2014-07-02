#pragma once

class sqliteapp : public mainapp
{
public:
	sqliteapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
private:
};
