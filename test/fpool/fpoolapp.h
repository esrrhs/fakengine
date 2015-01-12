#pragma once

class fpoolapp : public mainapp
{
public:
	fpoolapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
};
