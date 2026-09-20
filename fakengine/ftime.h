#pragma once

#include <chrono>
#include <ctime>

static force_inline time_t get_s_tick()
{
	return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

static force_inline uint32_t get_ms_tick()
{
	using namespace std::chrono;
	return static_cast<uint32_t>(
		duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
}

static force_inline uint64_t get_ns_tick()
{
	using namespace std::chrono;
	return static_cast<uint64_t>(
		duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count());
}
