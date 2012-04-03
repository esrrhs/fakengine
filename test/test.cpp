#include "fuck_game_server_engine.h"

int main()
{
	netlink<thread_link<netmsg<std::vector<char>>, std::list<netmsg<std::vector<char>>*>, socket_link<tcpsocket<cirle_buffer> > > > nl;
	netserver<thread_container<socket_container<tcpsocket<cirle_buffer>, std::list<char*> > > > ns;
	
	nl.ini();
	ns.ini();
	return 0;
}