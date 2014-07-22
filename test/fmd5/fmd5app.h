#pragma once

class fmd5app : public mainapp
{
public:
	fmd5app();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
private:
};
