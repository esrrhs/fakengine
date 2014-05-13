#pragma once

class clientapp : public mainapp
{
public:
	clientapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
};
