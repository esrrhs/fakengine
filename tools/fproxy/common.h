#pragma once

#include "proxymsg.h"

typedef cmdparser<10> mycmdparser;

#define MAX_MSG_SIZE Fproto::PROXY_MSG_LEN + 1024

#ifdef DEBUG
#define MAX_LINK_SIZE 10
#define MAX_BUFF_SIZE MAX_MSG_SIZE
#else
#define MAX_LINK_SIZE 1000
#define MAX_BUFF_SIZE 16*1024*1024
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
	bool HandleRegister(mysocket & s, const Fproto::ProxyMsg & proxymsg);
	bool HandleTransByKey(mysocket & s, const Fproto::ProxyMsg & proxymsg);
	bool HandleTransByID(mysocket & s, const Fproto::ProxyMsg & proxymsg);
	bool HandleTransBroadcast(mysocket & s, const Fproto::ProxyMsg & proxymsg);
};

#ifdef WIN32
typedef selector<MAX_LINK_SIZE> serverselector;
#else
typedef epollor<MAX_LINK_SIZE> serverselector;
#endif

typedef socket_container<mymsg, mysocket, serverselector, proxy_processor, MAX_LINK_SIZE> mysocketcontainer;
typedef netserver<mysocketcontainer> mynetserver;
