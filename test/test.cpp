#include "fuck_game_server_engine.h"

int main()
{
	fengine fe;

	typedef tcpsocket<cirle_buffer, selector> mysocket;
	typedef netmsg<std::vector<char>> mymsg;
	typedef std::list<mymsg> mymsgcontainer;
	typedef std::list<std::pair<mysocket, mymsg>> myelecontainer;
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

	while (1)
	{
		fsleep(100);
	}

	return 0;
}