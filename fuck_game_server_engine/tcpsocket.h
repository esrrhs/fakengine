#pragma once

class tcpsocket
{
public:
	tcpsocket()
	{
	}
	~tcpsocket()
	{
	}
public:
	template<typename _msg>
	FORCEINLINE bool send(const _msg * msg)
	{
		return true;
	}
	template<typename _msg>
	FORCEINLINE bool recv(_msg * msg)
	{
		return true;
	}
private:
};

