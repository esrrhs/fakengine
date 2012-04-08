#include "fuck_game_server_engine.h"

typedef tcpsocket<cirle_buffer<int8_t, 1024>, selector> mysocket;
typedef netmsg< std::vector<int8_t> > mymsg;
typedef std::list<mymsg> mymsgcontainer;
typedef std::list< std::pair<mysocket*, mymsg> > myelecontainer;
typedef std::list<mysocket> mysocketlist;

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

		int32_t recv_i = 123456;
		mysocket * s;
		while (1)
		{
			mymsg recvm;
			if (ns.recv_msg(s, recvm))
			{
				std::cout<<"recvm: size["<<recvm.size()<<"] ";
				recvm.reset();
				int32_t tmp;
				recvm.read_int32(tmp);
				std::cout<<"data["<<tmp<<"]"<<std::endl;
				FASSERT(recv_i == tmp);
				recv_i++;
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

		int32_t send_i = 123456;
		while (1)
		{
			mymsg sendm;
			sendm.write_int32(send_i);
			if (nl.send_msg(sendm))
			{
				std::cout<<"sendm: size["<<sendm.size()<<"] ";
				std::cout<<"data["<<send_i<<"]"<<std::endl;
			}
			send_i++;

			fsleep(100);
		}
	}
	
	return 0;
}