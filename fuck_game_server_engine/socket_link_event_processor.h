#pragma once

template <typename _socket>
class socket_link_event_processor
{
public:
	socket_link_event_processor()
	{
	}
	~socket_link_event_processor()
	{
	}
public:
	force_inline bool on_close(const _socket & s)
	{
		return true;
	}
	force_inline bool on_connect(const _socket & s)
	{
		return true;
	}
private:
};



