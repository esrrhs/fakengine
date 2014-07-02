#include "fuck_game_server_engine.h"
#include "fmd5app.h"

bool fmd5app::ini( int argc, char *argv[] )
{
	return true;
}

bool fmd5app::heartbeat()
{
	int a = 0xA0F980;
	stringc aa(a, 16);
	stringc bb = fitoa16(a);
	stringc tmp = fmd5(bb.c_str(), bb.size());

	uint32_t cc32 = fcrc32(bb.c_str(), bb.size());
	uint32_t cc32a = fcrc32a(bb.c_str(), bb.size());
	FUSE(cc32);
	FUSE(cc32a);
	stringc sha1 = fsha1(bb.c_str(), bb.size());
    FUSE(sha1);
    
    uint8_t src[]="12345678";
    uint8_t des[8];
    uint8_t ddes[8];
    fdes("12345678", src, 8, des);
    fundes("12345678", des, 8, ddes);
    stringc des1 = fdes("12345678", "12345678");
    stringc src1 = fundes("12345678", des1);
    FUSE(src1);
	return true;
}

bool fmd5app::exit()
{
	return true;
}

fmd5app::fmd5app() : mainapp("fmd5app")
{

}
