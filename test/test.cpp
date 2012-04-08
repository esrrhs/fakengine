#include "fuck_game_server_engine.h"

typedef tcpsocket<cirle_buffer<int8_t, 1024 * 1024>, selector> mysocket;
typedef netmsg< std::vector<int8_t, fallocator<int8_t, normal_allocator<int8_t> > > > mymsg;
typedef std::list<mymsg, fallocator<mymsg, normal_allocator<mymsg> > > mymsgcontainer;
typedef std::pair<mysocket*, mymsg> myserverelement;
typedef std::list<myserverelement, fallocator<myserverelement, normal_allocator<myserverelement> > > myelecontainer;
typedef std::list<mysocket, fallocator<mysocket, normal_allocator<mysocket> > > mysocketlist;

typedef socket_link<mysocket> mysocketlink;
typedef thread_link<mymsg, mymsgcontainer, mysocketlink> mythreadlink;
typedef netlink<mythreadlink> mynetlink;

typedef socket_container<mysocket, selector, mysocketlist> mysocketcontainer;
typedef thread_container<mymsg, mysocket, myelecontainer, mysocketcontainer> mythreadcontainer;
typedef netserver<mythreadcontainer> mynetserver;

int main(int argc, char *argv[])
{
	if (argc <= 3)
	{
		std::cout<<"need arg: [type] [ip] [port]"<<std::endl;
		return 0;
	}

	std::string str = argv[1];
	std::string ip = argv[2];
	std::string port = argv[3];

	fengine fe;

	if (str == "server")
	{
		mynetserver ns;

		// server
		tcp_socket_server_param ssp;
		ssp.ip = ip;
		ssp.port = atoi(port.c_str());
		ns.ini(ssp);

		mymsg sendm;
		std::string str = "hello client";
		sendm.write_str((const int8_t *)str.c_str(), 1024);

		while (1)
		{
			mysocket * s = 0;
			mymsg rec;
			if (ns.recv_msg(s, rec))
			{
				int8_t buf[1024];
				rec.read_buffer(buf, 1024);
				std::string str((const char *)buf);
				std::cout<<"server recv msg "<<str<<" from "<<s<<std::endl;
				
				ns.send_msg(s, sendm);
			}

			fsleep(100);
		}
	}
	else if (str == "client")
	{
		mynetlink nl;

		// client
		tcp_socket_link_param slp;
		slp.ip = ip;
		slp.port = atoi(port.c_str());
		nl.ini(slp);
		
		mymsg sendm;
		std::string str = "hello server";
		sendm.write_str((const int8_t *)str.c_str(), 1024);
		nl.send_msg(sendm);

		while (1)
		{
			mymsg rec;
			if (nl.recv_msg(rec))
			{
				int8_t buf[1024];
				rec.read_buffer(buf, 1024);
				std::string str((const char *)buf);
				std::cout<<"client recv msg "<<str<<std::endl;

				nl.send_msg(sendm);
			}
			
			fsleep(100);
		}
	}
	
	return 0;
}