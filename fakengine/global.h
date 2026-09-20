#pragma once

inline bool g_ini = false;

inline time_t g_start_time = 0;

inline int32_t g_SRandSeed = 0;

inline thread_local void* g_falloc_instance = nullptr;

inline uint8_t g_old_malloc_hook_mem[JMP_CODE_LEN] = {0};
inline uint8_t g_old_free_hook_mem[JMP_CODE_LEN] = {0};
inline uint8_t g_old_realloc_hook_mem[JMP_CODE_LEN] = {0};
inline uint8_t g_old_memalign_hook_mem[JMP_CODE_LEN] = {0};
inline uint8_t g_old_calloc_hook_mem[JMP_CODE_LEN] = {0};
