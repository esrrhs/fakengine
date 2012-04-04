#pragma once

void fstrcopy(int8_t * des, const int8_t * src, size_t dest_size)
{
	if (dest_size > 0)
	{
		strncpy((char*)des, (char*)src, dest_size - 1);
		des[dest_size - 1] = 0;
	}
}

