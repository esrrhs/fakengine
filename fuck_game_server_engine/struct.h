#pragma once

struct net_server_param
{
	std::string ip;
	uint16_t port;
	uint32_t backlog;
};

struct net_link_param
{
	std::string ip;
	uint16_t port;
};
