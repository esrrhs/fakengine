#pragma once

class fstringapp : public mainapp
{
public:
	fstringapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
};
