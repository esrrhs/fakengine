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

		time_t lt = get_s_tick();
		uint32_t recvbytes = 0;

		std::vector< std::pair<mysocket*, mymsg> > recvmv;
		while (1)
		{
			recvmv.clear();
			if (ns.recv_msgs(recvmv))
			{
				recvbytes += 1024 * recvmv.size();
			}

			time_t nt = get_s_tick();
			if (nt - lt > 5)
			{
				std::cout<<"speed "<<recvbytes / 1024 / 1024 / (nt  - lt)<<"M/S"<<std::endl;
				lt = nt;
				recvbytes = 0;
			}

			fsleep(10);
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

		time_t lt = get_s_tick();
		uint32_t sendbytes = 0;

		mymsg sendm;
		int8_t tmpbuffer[1024];
		sendm.write_buffer(tmpbuffer, 1024);
		std::vector<mymsg> sendmv;
		sendmv.insert(sendmv.end(), 1000, sendm);

		while (1)
		{
			nl.send_msgs(sendmv);
			sendbytes += 1024 * 1000;
			
			time_t nt = get_s_tick();
			if (nt - lt > 5)
			{
				std::cout<<"speed "<<sendbytes / 1024 / 1024 / (nt  - lt)<<"M/S"<<std::endl;
				lt = nt;
				sendbytes = 0;
			}

			fsleep(10);
		}
	}
	
	return 0;
}