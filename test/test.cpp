#include "fuck_game_server_engine.h"

int main()
{
	fengine fe;

	netlink<thread_link<netmsg<std::vector<char>>, std::list<netmsg<std::vector<char>>*>, socket_link<tcpsocket<cirle_buffer> > > > nl;
	netserver<thread_container<socket_container<tcpsocket<cirle_buffer>, std::list<char*> > > > ns;
	
	nl.ini();

	tcp_socket_server_param sp;
	sp.ip = "127.0.0.1";
	sp.port = 2012;
	sp.backlog = 5;
	ns.ini(sp);
	return 0;
}