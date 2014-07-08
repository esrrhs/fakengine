#pragma once

class fproxyclientapp : public mainapp
{
public:
	fproxyclientapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
private:
};
