#pragma once

template <typename buffer>
class fserialize
{
public:
	force_inline fserialize() : m_write_slot(&(buffer::write), &m_buffer),
		m_read_slot(&(buffer::read), &m_buffer)
	{

	}
	force_inline fserialize(const std::string & file_name, const std::string & mod) : m_write_slot(&(buffer::write), &m_buffer),
		m_read_slot(&(buffer::read), &m_buffer)
	{
		open_file(file_name, mod);
	}
	force_inline fserialize(const fserialize & r) : m_buffer(r.m_buffer),
		m_write_slot(&(buffer::write), &m_buffer),
		m_read_slot(&(buffer::read), &m_buffer)
	{
	}
	force_inline fserialize & operator = (const fserialize & r)
	{
		m_buffer = r.m_buffer;
		return *this;
	}
	force_inline ~fserialize()
	{
	}
public:
	template <typename T> force_inline
	bool serialize(const T & t)
	{
		return t.serialize(m_write_slot);
	}
	template <> force_inline
	bool serialize(const int8_t & t)
	{
		return write((const int8_t *)&t, sizeof(t));
	}
	template <> force_inline
	bool serialize(const int16_t & t)
	{
		return write((const int8_t *)&t, sizeof(t));
	}
	template <> force_inline
	bool serialize(const int32_t & t)
	{
		return write((const int8_t *)&t, sizeof(t));
	}
	template <> force_inline
	bool serialize(const int64_t & t)
	{
		return write((const int8_t *)&t, sizeof(t));
	}
	template <> force_inline
	bool serialize(const std::string & t)
	{
		uint32_t size = (uint32_t)t.size();
		if (!write((const int8_t *)&size, sizeof(size)))
		{
			return false;
		}
		return write((const int8_t *)t.c_str(), t.size());
	}
	template <typename T> force_inline
	bool deserialize(T & t)
	{
		return t.deserialize(m_read_slot);
	}
private:
	force_inline bool write(const int8_t * p, size_t size)
	{
		return m_buffer.write(p, size);
	}
	force_inline bool read(int8_t * out, size_t size)
	{
		return m_buffer.read(out, size);
	}
private:
	buffer m_buffer;
	// 供外部调用的处理函数
	slot<buffer, bool (buffer::*)(const int8_t * p, size_t size)> m_write_slot;
	slot<buffer, bool (buffer::*)(int8_t * out, size_t size)> m_read_slot;
};

