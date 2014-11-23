#pragma once

class fakeapp : public mainapp
{
public:
	fakeapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
private:
};
