#include "fuck_game_server_engine.h"

int main()
{
	fengine fe;

	typedef tcpsocket<cirle_buffer<int8_t, 1024>, selector> mysocket;
	typedef netmsg<std::vector<int8_t>> mymsg;
	typedef std::list<mymsg> mymsgcontainer;
	typedef std::list<std::pair<mysocket*, mymsg>> myelecontainer;
	typedef std::list<mysocket> mysocketlist;

	typedef socket_link<mysocket> mysocketlink;
	typedef thread_link<mymsg, mymsgcontainer, mysocketlink> mythreadlink;
	typedef netlink<mythreadlink> mynetlink;

	typedef socket_container<mysocket, selector, mysocketlist> mysocketcontainer;
	typedef thread_container<mymsg, mysocket, myelecontainer, mysocketcontainer> mythreadcontainer;
	typedef netserver<mythreadcontainer> mynetserver;

	mynetlink nl;
	mynetserver ns;

	// server
	tcp_socket_server_param ssp;
	ssp.ip = "127.0.0.1";
	ssp.port = 2012;
	ns.ini(ssp);

	// client
	tcp_socket_link_param slp;
	slp.ip = "127.0.0.1";
	slp.port = 2012;
	nl.ini(slp);

	int32_t send_i = 123456;
	int32_t recv_i;
	mysocket * s;
	while (1)
	{
		mymsg sendm;
		sendm.write_int32(send_i);
		nl.send_msg(sendm);
		send_i++;

		mymsg recvm;
		if (ns.recv_msg(s, recvm))
		{
			std::cout<<"recvm: size["<<recvm.size()<<"] ";
			recvm.reset();
			recvm.read_int32(recv_i);
			std::cout<<"data["<<recv_i<<"]"<<std::endl;
		}

		fsleep(100);
	}

	return 0;
}