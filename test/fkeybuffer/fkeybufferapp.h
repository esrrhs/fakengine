#pragma once

class fkeybufferapp : public mainapp
{
public:
	fkeybufferapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
private:
};
