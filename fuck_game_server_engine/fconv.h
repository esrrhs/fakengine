#pragma once

static force_inline stringc fitoa(int32_t i)
{
	return stringc(i);
}

static force_inline int32_t fatoi(const stringc & str)
{
	return atoi((const char *)str.c_str());
}
