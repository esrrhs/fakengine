#pragma once

class fprotoapp : public mainapp
{
public:
	fprotoapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
private:
};
