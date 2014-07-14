#pragma once

class fpointapp : public mainapp
{
public:
	fpointapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
private:
};
