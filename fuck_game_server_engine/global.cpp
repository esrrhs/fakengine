#include "fuck_game_server_engine.h"

time_t g_start_time = 0;

THREAD_LOCAL_VALUE fperf_manager * g_fperf_manager;
THREAD_LOCAL_VALUE int8_t * g_fperf_file_name;
