#pragma once

#include "../test.h"

class proxy_processor
{
public:
	force_inline bool on_recv_msg(uint32_t srcsvrid, const uint8_t * buffer, uint32_t size) 
	{
		return true;
	}
};

#define PROXY_NUM 5

class fproxyclientapp : public mainapp
{
public:
	fproxyclientapp();
	virtual bool ini(int argc, char *argv[]);
	virtual bool heartbeat();
	virtual bool exit();
private:
	fproxyclient<mymsg, mysocket, proxy_processor, PROXY_NUM> m_proxyclient;
};
