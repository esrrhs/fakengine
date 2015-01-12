#pragma once

template <typename _ele_type, typename _msg>
class neteventprocessor
{
public:
	force_inline neteventprocessor()
	{
	}
	force_inline ~neteventprocessor()
	{
	}
public:
	force_inline bool on_close(_ele_type & e)
	{
		return true;
	}
	force_inline bool on_accept(_ele_type & e)
	{
		return true;
	}
	force_inline bool on_connect(_ele_type & e)
	{
		return true;
	}
	force_inline bool on_recv_msg(_ele_type & e, const _msg & msg)
	{
		return true;
	}
private:
};



