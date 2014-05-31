#pragma once

typedef tcpsocket<cirle_buffer<int8_t, 1024 * 1024>, selector> mysocket;
typedef line_buffer<int8_t, 1024> mylinebuffer;
typedef netmsg<mylinebuffer> mymsg;
typedef neteventprocessor<mysocket, mymsg> myneteventprocessor;
typedef std::list<mysocket, fallocator<mysocket, normal_allocator<mysocket> > > mysocketlist;

class client_processor : public myneteventprocessor
{
public:
	force_inline bool on_recv_msg(mysocket & s, const mymsg & msg)
	{
		mymsg sendm;
		int16_t i = 10;
		sendm.write(i);

		s.send(sendm);
		return true;
	}
};
class server_processor : public myneteventprocessor
{
public:
	force_inline bool on_recv_msg(mysocket & s, const mymsg & msg)
	{
		mymsg sendm;
		int16_t i = 11;
		sendm.write(i);

		s.send(sendm);
		return true;
	}
};

typedef socket_link<mymsg, mysocket, client_processor> mysocketlink;
typedef netlink<mysocketlink> mynetlink;

typedef socket_container<mymsg, mysocket, selector, mysocketlist, server_processor> mysocketcontainer;
typedef netserver<mysocketcontainer> mynetserver;

//typedef cmdparser<std::map<std::string, std::string> > mycmdparser;

typedef factorymng<mainapp, stringc> myfactorymng;
