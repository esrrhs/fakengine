#include "fuck_game_server_engine.h"

typedef tcpsocket<cirle_buffer<int8_t, 1024 * 1024>, selector> mysocket;
typedef netmsg< std::vector<int8_t, fallocator<int8_t, normal_allocator<int8_t> > > > mymsg;
typedef neteventprocessor<mysocket, mymsg> myneteventprocessor;
typedef std::list<mysocket, fallocator<mysocket, normal_allocator<mysocket> > > mysocketlist;

class client_processor : public myneteventprocessor
{
public:
	force_inline bool on_recv_msg(mysocket & s, const mymsg & msg)
	{
		int8_t buff[1024];
		msg.read_buffer(buff, 1024);
		std::string recstr((const char *)buff);
		std::cout<<"client rec msg "<<recstr<<" from "<<(char *)s.get_peer_ip()
			<<" port "<<s.get_peer_port()<<std::endl;

		mymsg sendm;
		std::string str = "hello server";
		sendm.write_str((const int8_t *)str.c_str(), 1024);

		s.send(sendm);
		return true;
	}
};
class server_processor : public myneteventprocessor
{
public:
	force_inline bool on_recv_msg(mysocket & s, const mymsg & msg)
	{
		int8_t buff[1024];
		msg.read_buffer(buff, 1024);
		std::string recstr((const char *)buff);
		std::cout<<"server rec msg "<<recstr<<" from "<<(char *)s.get_peer_ip()
			<<" port "<<s.get_peer_port()<<std::endl;

		mymsg sendm;
		std::string str = "hello client";
		sendm.write_str((const int8_t *)str.c_str(), 1024);

		s.send(sendm);
		return true;
	}
};

typedef socket_link<mymsg, mysocket, client_processor> mysocketlink;
typedef netlink<mysocketlink> mynetlink;

typedef socket_container<mymsg, mysocket, selector, mysocketlist, server_processor> mysocketcontainer;
typedef netserver<mysocketcontainer> mynetserver;

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
	
	inifile ifile;
	ifile.load("config.ini");
	std::string ss;
	ifile.get("sec0", "key0", ss);

	flogSystem::ptr()->add(1, "trace.log");
	flogSystem::ptr()->write(1, "haha");
	flogSystem::ptr()->write(1, "test");

	int8_t src[1024];
	int32_t srclen = 1024;
	int8_t des[1024];
	int32_t deslen = 1024;
	time_t b,e;
	b = get_s_tick();
	for (int i = 0; i < 1000000; i++)
	{
		deslen = 1024;
		if (compress((Bytef*)des, (uLongf*)&deslen, (const Bytef*)src, srclen) != Z_OK)
		{
			std::cout<<"zlib compress error"<<std::endl;
		}
	}
	e = get_s_tick();
	std::cout<<"zlib compress "<<e - b<<std::endl;

	int8_t buff[64 * 1024];
	b = get_s_tick();
	for (int i = 0; i < 1000000; i++)
	{
		deslen = 1024;
		if (lzo1x_1_compress((const unsigned char *)src, (lzo_uint)srclen, 
			(unsigned char *)des, (lzo_uint*)&deslen, (void*)buff) != LZO_E_OK)
		{
			std::cout<<"lzo compress error"<<std::endl;
		}
	}
	e = get_s_tick();
	std::cout<<"lzo compress "<<e - b<<std::endl;
	
	if (str == "server")
	{
		mynetserver ns;

		// server
		tcp_socket_server_param ssp;
		ssp.ip = ip;
		ssp.port = atoi(port.c_str());
		ns.ini(ssp);

		while (1)
		{
			ns.tick();

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
			nl.tick();
			
			fsleep(100);
		}
	}
	
	return 0;
}