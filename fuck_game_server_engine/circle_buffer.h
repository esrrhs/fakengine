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
	FORCEINLINE bool write(const int8_t * p, size_t size)
	{
		return true;
	}
	FORCEINLINE bool read(int8_t * out, size_t size)
	{
		return true;
	}
	FORCEINLINE bool skip_write(size_t size)
	{
		return true;
	}
	FORCEINLINE bool skip_read(size_t size)
	{
		return true;
	}
private:
	int8_t * m_buffer;
	size_t m_size;
};

