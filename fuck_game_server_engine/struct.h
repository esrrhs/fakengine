#pragma once

struct net_server_param
{
};

struct tcp_socket_server_param : public net_server_param
{
	tcp_socket_server_param() : ip("127.0.0.1"), port(2012), backlog(5), 
		lingertime(0), is_non_blocking(true), 
		socket_send_buffer_size(1024), socket_recv_buffer_size(1024) 
	{
	}
	std::string ip;
	uint16_t port;
	uint32_t backlog;
	uint32_t lingertime;
	bool is_non_blocking;
	uint32_t socket_send_buffer_size;
	uint32_t socket_recv_buffer_size;
};

struct net_link_param
{
};

struct tcp_socket_link_param : public net_link_param
{
	tcp_socket_link_param() : ip("127.0.0.1"), port(2012), is_non_blocking(true), 
		lingertime(0), socket_send_buffer_size(1024), socket_recv_buffer_size(1024) 
	{
	}
	std::string ip;
	uint16_t port;
	uint32_t lingertime;
	bool is_non_blocking;
	uint32_t socket_send_buffer_size;
	uint32_t socket_recv_buffer_size;
};


