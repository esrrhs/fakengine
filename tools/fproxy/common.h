#pragma once

#include "proxymsg.h"

typedef cmdparser<10> mycmdparser;

#define MAX_BUFF_SIZE 16*1024*1024
#define MAX_MSG_SIZE 1024*1024
#ifdef DEBUG
#define MAX_LINK_SIZE 10
#else
#define MAX_LINK_SIZE 1000
#endif

extern char g_msg_buffer[MAX_MSG_SIZE];
extern Fproto::ProxyMsg g_netmsg;

typedef tcpsocket<cirle_buffer<int8_t, MAX_BUFF_SIZE>, simpleselector> mysocket;
typedef line_buffer<int8_t, MAX_MSG_SIZE> mylinebuffer;
typedef netmsg<mylinebuffer> mymsg;
typedef neteventprocessor<mysocket, mymsg> myneteventprocessor;

class proxy_processor : public myneteventprocessor
{
public:
	bool on_recv_msg(mysocket & s, const mymsg & msg);

private:
	bool HandleRegister(const Fproto::ProxyMsg & proxymsg);
	bool HandleTransByKey(const Fproto::ProxyMsg & proxymsg);
	bool HandleTransByID(const Fproto::ProxyMsg & proxymsg);
	bool HandleTransBroadcast(const Fproto::ProxyMsg & proxymsg);
};

#ifdef WIN32
typedef selector<MAX_LINK_SIZE> serverselector;
#else
typedef epollor<MAX_LINK_SIZE> serverselector;
#endif

typedef socket_container<mymsg, mysocket, serverselector, proxy_processor, MAX_LINK_SIZE> mysocketcontainer;
typedef netserver<mysocketcontainer> mynetserver;
