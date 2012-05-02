#pragma once

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
	force_inline bool on_close()
	{
		return true;
	}
	force_inline bool on_connect()
	{
		return true;
	}
private:
};



