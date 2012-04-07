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
	enum _const
	{
		msg_header = 2012,		// 包头
		msg_header_size = 2,	// 包头长度
		msg_size_size = 2,		// 包长度的长度
	};
public:
	FORCEINLINE void write_int32(const int32_t & i)
	{
		write_buffer((int8_t*)&i, sizeof(i));
	}
	FORCEINLINE void write_buffer(const int8_t * p, size_t size)
	{
		m_buffer.insert(m_buffer.end(), p, p + size);
	}
public:
	template <typename F>
	FORCEINLINE bool to_buffer(const F & f) const
	{
		const _const c = msg_header;
		FASSERT(msg_header_size <= sizeof(_const));
		// 写入包头
		if (!f((const int8_t*)&c, msg_header_size))
		{
			return false;
		}

		// 写入size
		size_t data_size = m_buffer.size();
		FASSERT(msg_size_size <= sizeof(size_t));
		FASSERT(data_size <= (1<<(msg_size_size * 8)));
		if (!f((int8_t*)&data_size, msg_size_size))
		{
			return false;
		}

		// 写入data
		if (!f((int8_t*)&m_buffer[0], data_size))
		{
			return false;
		}
		
		return true;
	}
	template <typename F>
	FORCEINLINE bool from_buffer(const F & f)
	{
		// 读出包头
		_const c = msg_header;
		FASSERT(msg_header_size <= sizeof(_const));
		if (!f((int8_t*)&c, msg_header_size))
		{
			return false;
		}
		if (c != msg_header)
		{
			return false;
		}

		// 读出size
		size_t data_size = 0;
		FASSERT(msg_size_size <= sizeof(size_t));
		if (!f((int8_t*)&data_size, msg_size_size))
		{
			return false;
		}

		// 读出data
		m_buffer.resize(0, data_size);
		if (data_size > 0 && !f((int8_t*)&m_buffer[0], data_size))
		{
			return false;
		}

		return true;
	}
private:
	_buffer m_buffer;
};

