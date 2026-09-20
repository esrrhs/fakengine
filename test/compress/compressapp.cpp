#include "fakengine.h"
#include "compressapp.h"

bool compressapp::ini( int argc, char *argv[] )
{
	return true;
}

bool compressapp::heartbeat()
{
	int8_t src[1024];
	memset(src, 'A', sizeof(src));
	uLong srclen = sizeof(src);
	int8_t des[2048];
	uLongf deslen = sizeof(des);
#ifdef HAVE_ZLIB
	time_t b, e;
	b = get_s_tick();
	for (int i = 0; i < 1000; i++)
	{
		deslen = sizeof(des);
		if (compress((Bytef*)des, &deslen, (const Bytef*)src, srclen) != Z_OK)
		{
			std::cout << "zlib compress error" << std::endl;
			return false;
		}

		int8_t uncompressed[1024];
		uLongf uncomplen = sizeof(uncompressed);
		if (uncompress((Bytef*)uncompressed, &uncomplen, (const Bytef*)des, deslen) != Z_OK)
		{
			std::cout << "zlib uncompress error" << std::endl;
			return false;
		}
	}
	e = get_s_tick();
	std::cout << "zlib compress 1000 iters time: " << e - b << "s" << std::endl;
#endif

	return true;
}

bool compressapp::exit()
{
	return true;
}

compressapp::compressapp() : mainapp("compressapp")
{

}
