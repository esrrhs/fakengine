#pragma once

struct cmdcontrol
{
	force_inline bool is_set()
	{
		return isset == 1;
	}
	force_inline void clear()
	{
		memset(cmd, 0, sizeof(cmd));
		memset(param, 0, sizeof(param));
		isset = 0;
	}
	force_inline bool is_cmd(const int8_t * _cmd)
	{
		if (fstricmp(cmd, _cmd) == 0)
		{
			return true;
		}
		return false;
	}
	force_inline bool set_cmd(const int8_t * _cmd, const int8_t * _param)
	{
		fstrcopy(cmd, _cmd, sizeof(cmd));
		fstrcopy(param, _param, sizeof(param));
		isset = 1;
		return true;
	}
	force_inline const int8_t * get_cmd()
	{
		return cmd;
	}
private:
	int8_t isset;
	int8_t cmd[c_CmdControlCmdSize];
	int8_t param[c_CmdControlCmdSize];
};
