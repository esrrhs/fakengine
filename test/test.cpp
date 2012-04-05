#include "fuck_game_server_engine.h"

int main()
{
	fengine fe;

	netlink<thread_link<netmsg<std::vector<char>>, std::list<netmsg<std::vector<char>>*>, socket_link<tcpsocket<cirle_buffer, selector> > > > nl;
	netserver<thread_container<socket_container<tcpsocket<cirle_buffer, selector>, selector, std::list<tcpsocket<cirle_buffer, selector> > > > > ns;

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