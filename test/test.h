#pragma once

typedef tcpsocket<cirle_buffer<int8_t, 1024 * 1024>, selector> mysocket;
typedef line_buffer<int8_t, 1024> mylinebuffer;
typedef netmsg<mylinebuffer> mymsg;
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
		sendm.write_str((const char *)str.c_str(), 1024);

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
		sendm.write_str((const char *)str.c_str(), 1024);

		s.send(sendm);
		return true;
	}
};

typedef socket_link<mymsg, mysocket, client_processor> mysocketlink;
typedef netlink<mysocketlink> mynetlink;

typedef socket_container<mymsg, mysocket, selector, mysocketlist, server_processor> mysocketcontainer;
typedef netserver<mysocketcontainer> mynetserver;

typedef flog_system< std::map<uint32_t, flog *> > myflog_system;
typedef inifile< std::map<std::string, std::map<std::string, std::string> >, std::map<std::string, std::string> > myinifile;

typedef ftrie<int8_t, std::map<int8_t, std::pair<void *, bool> > > myftrie;

typedef cmdparser<std::map<std::string, std::string> > mycmdparser;

