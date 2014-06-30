#include "fuck_game_server_engine.h"
#include "sqliteapp.h"

bool sqliteapp::ini( int argc, char *argv[] )
{
	return true;
}

bool sqliteapp::heartbeat()
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
    
	sqlite3 * handle = 0;
	int ret = sqlite3_open("test.db", &handle);
	sqlite3_stmt *stmt;
	ret = sqlite3_prepare(handle, "select * from aaa", -1, &stmt, NULL);
	while (SQLITE_ROW == sqlite3_step(stmt)) 
	{
		stringc aa = (const char *)sqlite3_column_text(stmt, 0);
		int bb = sqlite3_column_int(stmt, 1);
		FUSE(bb);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(handle);
	return true;
}

bool sqliteapp::exit()
{
	return true;
}

sqliteapp::sqliteapp() : mainapp("sqliteapp")
{

}
