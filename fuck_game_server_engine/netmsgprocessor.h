#pragma once

template <typename _ele_type, typename _msg>
class netmsgprocessor
{
public:
	netmsgprocessor()
	{
	}
	~netmsgprocessor()
	{
	}
public:
	force_inline bool process(_ele_type & e, const _msg & msg)
	{
		return true;
	}
private:
};



