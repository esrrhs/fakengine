#pragma once

class pcreapp : public mainapp
{
public:
	pcreapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
private:
};
