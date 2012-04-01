#pragma once

template <typename _buffer>
class netmsg
{
public:
	netmsg()
	{
	}
	~netmsg()
	{
	}
public:
	FORCEINLINE const char * to_buffer() const
	{
		return 0;
	}
	FORCEINLINE size_t to_buffer_size() const
	{
		return 0;
	}
	template <typename F>
	FORCEINLINE bool from_buffer(const F & f, size_t & read_size)
	{
		return true;
	}
private:
	_buffer m_buffer;
};

