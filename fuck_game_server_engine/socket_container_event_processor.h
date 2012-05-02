#pragma once

template <typename _socket>
class socket_container_event_processor
{
public:
	socket_container_event_processor()
	{
	}
	~socket_container_event_processor()
	{
	}
public:
	force_inline bool on_close(const _socket & s)
	{
		return true;
	}
	force_inline bool on_accept(const _socket & s)
	{
		return true;
	}
private:
};



