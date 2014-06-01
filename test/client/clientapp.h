#pragma once

#include "..\test.h"

class clientapp : public mainapp
{
public:
	clientapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
private:
	mynetlink m_mynetlink;
};
