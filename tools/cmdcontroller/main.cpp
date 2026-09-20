#include "fakengine.h"

int main()
{
	fengine engine("cmdcontroller");

	FPRINTF("cmdcontroller start\n");

	while (1)
	{
		shm_handle cmdhandle = open_share_mem(c_DefaultCmdKey, sizeof(cmdcontrol));
		if (!IS_VALID_SHM_HANDLE(cmdhandle))
		{
			FPRINTF("open_share_mem fail, wait\n");
#if defined(WIN32)
			Sleep(1000);
#else
			usleep(1000 * 1000);
#endif
			continue;
		}

		cmdcontrol * pcmdcontrol = (cmdcontrol *)map_share_mem(cmdhandle);
		if (pcmdcontrol == 0)
		{
			FPRINTF("map_share_mem fail, wait\n");
#if defined(WIN32)
			Sleep(1000);
#else
			usleep(1000 * 1000);
#endif
			continue;
		}

		std::cout<<"input cmd:";
		std::string cmd;
		std::cin>>cmd;

		std::cout<<"input param:";
		std::string param;
		std::cin>>param;

		pcmdcontrol->set_cmd((const int8_t*)cmd.c_str(), (const int8_t*)param.c_str());

		unmap_share_mem((int8_t*)pcmdcontrol);
		close_share_mem(cmdhandle);
	}

	FPRINTF("cmdcontroller exit\n");

	return 0;
}
