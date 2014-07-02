#pragma once

#include "../tools/genmsg/Struct.h"

#define MAX_BUFF_SIZE 10240
#define MAX_MSG_SIZE 1024
#define MAX_LINK_SIZE 1000

extern char g_msg_buffer[MAX_MSG_SIZE];
extern Fproto::NetMsg g_netmsg;

typedef tcpsocket<cirle_buffer<int8_t, MAX_BUFF_SIZE>, simpleselector> mysocket;
typedef line_buffer<int8_t, MAX_MSG_SIZE> mylinebuffer;
typedef netmsg<mylinebuffer> mymsg;
typedef neteventprocessor<mysocket, mymsg> myneteventprocessor;

class server_processor : public myneteventprocessor
{
public:
	force_inline bool on_recv_msg(mysocket & s, const mymsg & msg)
	{
		msg.read_buffer((int8_t*)g_msg_buffer, sizeof(g_msg_buffer));
		int32_t recvmsgsize = g_netmsg.Unmarshal(g_msg_buffer, sizeof(g_msg_buffer));
		FPRINTF("recv id %d size %d\n", g_netmsg.m_NetMsgPara.m_Type, recvmsgsize);

		// send
		g_netmsg.m_NetMsgPara.m_Type = Fproto::SC_RES_LOGIN;
		Fproto::SCResLogin & para = g_netmsg.m_NetMsgPara.m_SCResLogin;
		memset(&para, 0, sizeof(para));
		para.m_Ret = 0;
		para.m_RoleInfoNum = 5;
		int32_t msgsize = g_netmsg.Marshal(g_msg_buffer, sizeof(g_msg_buffer));
		FASSERT(msgsize > 0);

		mymsg sendm;
		sendm.write_buffer((const int8_t*)g_msg_buffer, msgsize);
		s.send(sendm);
		return true;
	}
};

class client_processor : public myneteventprocessor
{
public:
	force_inline bool on_recv_msg(mysocket & s, const mymsg & msg)
	{
		msg.read_buffer((int8_t*)g_msg_buffer, sizeof(g_msg_buffer));
		int32_t recvmsgsize = g_netmsg.Unmarshal(g_msg_buffer, sizeof(g_msg_buffer));
		FPRINTF("recv id %d size %d\n", g_netmsg.m_NetMsgPara.m_Type, recvmsgsize);

		// send
		g_netmsg.m_NetMsgPara.m_Type = Fproto::CS_REQ_LOGIN;
		Fproto::CSReqLogin & para = g_netmsg.m_NetMsgPara.m_CSReqLogin;
		memset(&para, 0, sizeof(para));
		fstrcopy(para.m_Acc, "test");
		fstrcopy(para.m_Pwd, "123");
		int32_t msgsize = g_netmsg.Marshal(g_msg_buffer, sizeof(g_msg_buffer));
		FASSERT(msgsize > 0);

		mymsg sendm;
		sendm.write_buffer((const int8_t*)g_msg_buffer, msgsize);
		s.send(sendm);
		return true;
	}
	force_inline bool on_connect(mysocket & s)
	{
		mymsg sendm;
		int16_t i = 111;
		sendm.write(i);

		s.send(sendm);
		return true;
	}
};

typedef socket_link<mymsg, mysocket, client_processor> mysocketlink;
typedef netlink<mysocketlink> mynetlink;

#ifdef WIN32
typedef selector<1000> serverselector;
#else
typedef epollor<10240> serverselector;
#endif

typedef socket_container<mymsg, mysocket, serverselector, server_processor, MAX_LINK_SIZE> mysocketcontainer;
typedef netserver<mysocketcontainer> mynetserver;

typedef cmdparser<10> mycmdparser;

typedef factorymng<mainapp, 100, stringc> myfactorymng;
