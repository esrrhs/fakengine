#pragma once

class fallocapp : public mainapp
{
public:
	fallocapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
private:
};
