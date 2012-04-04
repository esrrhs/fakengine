#pragma once

struct net_server_param
{
};

struct tcp_socket_server_param : public net_server_param
{
	tcp_socket_server_param() : port(0), backlog(5), is_non_blocking(false), 
		socket_send_buffer_size(1024), socket_recv_buffer_size(1024) 
	{
	}
	std::string ip;
	uint16_t port;
	uint32_t backlog;
	bool is_non_blocking;
	uint32_t socket_send_buffer_size;
	uint32_t socket_recv_buffer_size;
};

struct net_link_param
{
	std::string ip;
	uint16_t port;
};
