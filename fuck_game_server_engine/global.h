#pragma once

extern time_t g_start_time;

class fperf_manager;
extern THREAD_LOCAL_VALUE fperf_manager * g_fperf_manager;
extern THREAD_LOCAL_VALUE int8_t * g_fperf_file_name;
