#pragma once

class fproxyapp : public mainapp
{
public:
	fproxyapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
private:
};
