#include "fuck_game_server_engine.h"

bool g_ini = false;

time_t g_start_time = 0;

int32_t g_SRandSeed = 0;

THREAD_LOCAL_VALUE void* g_falloc_instance = 0;

uint8_t g_old_malloc_hook_mem[JMP_CODE_LEN];
uint8_t g_old_free_hook_mem[JMP_CODE_LEN];
uint8_t g_old_realloc_hook_mem[JMP_CODE_LEN];
uint8_t g_old_memalign_hook_mem[JMP_CODE_LEN];

