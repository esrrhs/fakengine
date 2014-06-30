#pragma once

static force_inline stringc fitoa(int32_t i)
{
	return stringc(i);
}

static force_inline stringc fitoa16(int32_t i)
{
	return stringc(i, 16);
}

static force_inline int32_t fatoi(const stringc & str)
{
	return atoi((const char *)str.c_str());
}

static force_inline int32_t fatoi16(const stringc & str)
{
    int32_t ret = 0;   
    for (int32_t i = 0; i < (int32_t)str.size(); i++)   
    {		
        const uint8_t & c = str[i];	
        if (c >= '0' && c <= '9')	
        {		
            ret = ret * 16 + (c - '0');	
        }	
        else if (c >= 'A' && c <= 'F')		
        {		
            ret = ret * 16 + (c - 'A') + 10;	
        }  
        else if (c >= 'a' && c <= 'f')		
        {		
            ret = ret * 16 + (c - 'a') + 10;	
        }  
    }
    return ret;
}

