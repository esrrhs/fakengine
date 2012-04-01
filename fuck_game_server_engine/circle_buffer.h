#pragma once

class cirle_buffer
{
public:
	cirle_buffer() : m_buffer(0), m_size(0)
	{
	}
	~cirle_buffer()
	{
		SAFE_FREE(m_buffer);
	}
public:
	FORCEINLINE bool write(const char * p, size_t size)
	{
		return true;
	}
	FORCEINLINE bool read(char * out, size_t size)
	{
		return true;
	}
	FORCEINLINE bool skip(size_t size)
	{
		return true;
	}
private:
	char * m_buffer;
	size_t m_size;
};

