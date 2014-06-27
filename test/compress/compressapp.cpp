#include "fuck_game_server_engine.h"
#include "compressapp.h"

bool compressapp::ini( int argc, char *argv[] )
{
	return true;
}

bool compressapp::heartbeat()
{
	int8_t src[1024];
	int32_t srclen = 1024;
	int8_t des[1024];
	int32_t deslen = 1024;
	time_t b,e;
	b = get_s_tick();
	for (int i = 0; i < 1000000; i++)
	{
		deslen = 1024;
		if (compress((Bytef*)des, (uLongf*)&deslen, (const Bytef*)src, srclen) != Z_OK)
		{
			std::cout<<"zlib compress error"<<std::endl;
		}
	}
	e = get_s_tick();
	std::cout<<"zlib compress "<<e - b<<std::endl;

	int8_t buff[64 * 1024];
	b = get_s_tick();
	for (int i = 0; i < 1000000; i++)
	{
		deslen = 1024;
		if (lzo1x_1_compress((const unsigned char *)src, (lzo_uint)srclen, 
			(unsigned char *)des, (lzo_uint*)&deslen, (void*)buff) != LZO_E_OK)
		{
			std::cout<<"lzo compress error"<<std::endl;
		}
	}
	e = get_s_tick();
	std::cout<<"lzo compress "<<e - b<<std::endl;

	return true;
}

bool compressapp::exit()
{
	return true;
}

compressapp::compressapp() : mainapp("compressapp")
{

}
