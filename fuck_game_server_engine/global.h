#pragma once

extern time_t g_start_time;

class fperf_manager;
extern THREAD_LOCAL_VALUE fperf_manager * g_fperf_manager;
