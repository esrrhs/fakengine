#pragma once

extern bool g_ini;

extern time_t g_start_time;

extern int32_t g_SRandSeed;

extern THREAD_LOCAL_VALUE void* g_falloc_instance;

extern uint8_t g_old_malloc_hook_mem[JMP_CODE_LEN];
extern uint8_t g_old_free_hook_mem[JMP_CODE_LEN];
extern uint8_t g_old_realloc_hook_mem[JMP_CODE_LEN];
extern uint8_t g_old_memalign_hook_mem[JMP_CODE_LEN];
extern uint8_t g_old_calloc_hook_mem[JMP_CODE_LEN];
