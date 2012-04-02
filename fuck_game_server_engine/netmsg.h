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
	enum
	{
		msg_header = 2012,
		msg_header_size = 2,
	};
public:
	FORCEINLINE bool to_buffer(int8_t * & p, size_t & size) const
	{
		size = m_buffer.size();
		p = size ? (int8_t *)&m_buffer[0] : 0;
		return true;
	}
	template <typename F>
	FORCEINLINE bool from_buffer(const F & f, size_t & read_size)
	{
		return true;
	}
private:
	_buffer m_buffer;
};

