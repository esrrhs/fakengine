#pragma once

class serverapp : public mainapp
{
public:
	serverapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
};
