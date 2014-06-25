#include "fuck_game_server_engine.h"

bool g_ini = false;
time_t g_start_time = 0;
int32_t g_SRandSeed = 0;
THREAD_LOCAL_VALUE void* g_falloc_instance = 0;

