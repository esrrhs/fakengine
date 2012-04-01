#include "fuck_game_server_engine.h"

int main()
{
	netlink<thread_sender<netmsg<std::vector<char>>, std::list<netmsg<std::vector<char>>*>, socket_sender<tcpsocket> >, 
		thread_recver<netmsg<std::vector<char>>, std::list<netmsg<std::vector<char>>*>, socket_recver<tcpsocket> > > nl;

	return 0;
}