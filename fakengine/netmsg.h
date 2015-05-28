#pragma once

template <typename _buffer>
class netmsg
{
public:
	force_inline netmsg()
	{
	}
	force_inline ~netmsg()
	{
	}
public:
	template <typename T>
	bool write(const T & i)
	{
		return write((const int8_t *)&i, sizeof(i));
	}
	template <typename T>
	bool read(T & i) const
	{
		return read((int8_t *)&i, sizeof(i));
	}
	force_inline size_t size() const
	{
		return m_buffer.size();
	}
	force_inline bool write_buffer(const int8_t * p, size_t size)
	{
		return write(p, size);
	}
	force_inline bool read_buffer(int8_t * p, size_t size) const
	{
		return read(p, size);
	}
public:
	template <typename T>
	force_inline bool serialize(T & t) const
	{
		const uint32_t c = c_msg_header;
		FASSERT(c_msg_header_size <= sizeof(c_msg_header));
		// 写入包头
		if (!t.write((const int8_t*)&c, c_msg_header_size))
		{
			return false;
		}

		// 写入buffer
		if (!m_buffer.serialize(t))
		{
			return false;
		}
		
		return true;
	}
	template <typename T>
	force_inline bool deserialize(T & t)
	{
		// 读出包头
		uint32_t c = c_msg_header;
		FASSERT(c_msg_header_size <= sizeof(c_msg_header));
		if (!t.read((int8_t*)&c, c_msg_header_size))
		{
			return false;
		}
		if (c != c_msg_header)
		{
			// 包头错了
			LOG_ERROR(FENGINE_HEADER "netmsg deserialize header error");
			t.clear();
			return false;
		}

		// 读出buffer
		if (!m_buffer.deserialize(t))
		{
			return false;
		}

		return true;
	}
private:
	force_inline bool write(const int8_t * p, size_t size)
	{
		return m_buffer.write(p, size);
	}
	force_inline bool read(int8_t * out, size_t size) const
	{
		return m_buffer.read(out, size);
	}
private:
	mutable _buffer m_buffer;
};

