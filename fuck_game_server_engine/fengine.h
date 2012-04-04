#pragma once

class fengine
{
public:
	fengine()
	{
		ini();
	}
	~fengine()
	{
		exit();
	}
private:
	bool ini()
	{
#ifdef WIN32
		WORD wVersionRequested;
		WSADATA wsaData;
		int32_t err;
		wVersionRequested = MAKEWORD(2, 2);
		err = WSAStartup(wVersionRequested, &wsaData);
		if (err != 0)
		{
			FPRINTF("WSAStartup error\n");
			return false;
		}
#endif
		return true;
	}
	bool exit()
	{
		return true;
	}
};