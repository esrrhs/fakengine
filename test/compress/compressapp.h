#pragma once

class compressapp : public mainapp
{
public:
	compressapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
};
