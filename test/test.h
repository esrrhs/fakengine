#pragma once

#define MAX_BUFF_SIZE 1024
#define MAX_MSG_SIZE 50
#define MAX_LINK_SIZE 1000

typedef tcpsocket<cirle_buffer<int8_t, MAX_BUFF_SIZE>, selector> mysocket;
typedef line_buffer<int8_t, MAX_MSG_SIZE> mylinebuffer;
typedef netmsg<mylinebuffer> mymsg;
typedef neteventprocessor<mysocket, mymsg> myneteventprocessor;

class server_processor : public myneteventprocessor
{
public:
	force_inline bool on_recv_msg(mysocket & s, const mymsg & msg)
	{
		int16_t r;
		msg.read(r);
		FPRINTF("recv %d", r);

		mymsg sendm;
		int16_t i = 11;
		sendm.write(i);

		s.send(sendm);
		return true;
	}
};

class client_processor : public myneteventprocessor
{
public:
	force_inline bool on_recv_msg(mysocket & s, const mymsg & msg)
	{
		int16_t r;
		msg.read(r);
		FPRINTF("recv %d", r);

		mymsg sendm;
		int16_t i = 10;
		sendm.write(i);

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

typedef socket_container<mymsg, mysocket, selector, server_processor, MAX_LINK_SIZE> mysocketcontainer;
typedef netserver<mysocketcontainer> mynetserver;

typedef cmdparser<std::map<std::string, std::string> > mycmdparser;

typedef factorymng<mainapp, stringc> myfactorymng;
