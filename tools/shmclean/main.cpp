#include "fuck_game_server_engine.h"

int main()
{
    int * p = (int*)malloc(10);
    *p =123;
	fengine engine("shmclean");

	FPRINTF("shmclean start\n");

	int n = 1000;
	for (int i = 0; i < n; i++)
	{
		shm_handle cmdhandle = open_share_mem(c_DefaultCmdKey - (n / 2) + i, sizeof(cmdcontrol));
		if (cmdhandle <= 0)
		{
			continue;
		}

		cmdcontrol * pcmdcontrol = (cmdcontrol *)map_share_mem(cmdhandle);
		if (pcmdcontrol == 0)
		{
			continue;
		}

		unmap_share_mem((int8_t*)pcmdcontrol);
		close_share_mem(cmdhandle);
	}
	
	FPRINTF("shmclean exit\n");

	return 0;
}
