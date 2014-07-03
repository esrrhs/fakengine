#include "fuck_game_server_engine.h"
#include "fpointapp.h"

bool fpointapp::ini( int argc, char *argv[] )
{
	return true;
}

bool fpointapp::heartbeat()
{
    point2df p1(1, 1);
    point2df p2(1, 1);
    p2 += p1;

    vector3df v1(1, 2, 3);
    vector3df v2(1, 2, 3);
    v2 -= v1;

    vector3df t1(-1, 0, 0);
    vector3df t2(0, 1, 0);
    vector3df t3(1, 0, 0);
    triangle3df tr(t1, t2, t3);
    bool b = tr.isPointInside(vector3df(0,0,0));
    b = tr.isPointInside(vector3df(1.1f,0,0));
    FUSE(b);
	return true;
}

bool fpointapp::exit()
{
	return true;
}

fpointapp::fpointapp() : mainapp("fpointapp")
{

}
