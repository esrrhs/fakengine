#pragma once

#ifdef WIN32

template <typename buffer>
class fserialize
{
public:
	force_inline fserialize() : m_write_slot(&(buffer::write), &m_buffer),
		m_read_slot(&(buffer::read), &m_buffer)
	{

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
	force_inline bool serialize(const int8_t & t)
	{
		return write((const int8_t *)&t, sizeof(t));
	}
	force_inline bool serialize(const int16_t & t)
	{
		return write((const int8_t *)&t, sizeof(t));
	}
	force_inline bool serialize(const int32_t & t)
	{
		return write((const int8_t *)&t, sizeof(t));
	}
	force_inline bool serialize(const int64_t & t)
	{
		return write((const int8_t *)&t, sizeof(t));
	}
	force_inline bool serialize(const std::string & t)
	{
		store();
		uint16_t size = (uint16_t)t.size();
		if (!write((const int8_t *)&size, sizeof(size)))
		{
			restore();
			return false;
		}
		
		if (!write((const int8_t *)t.c_str(), t.size()))
		{
			restore();
			return false;
		}

		return true;
	}

	template <typename T> force_inline
	bool deserialize(T & t)
	{
		return t.deserialize(m_read_slot);
	}
	force_inline bool deserialize(int8_t & t)
	{
		return read((int8_t *)&t, sizeof(t));
	}
	force_inline bool deserialize(int16_t & t)
	{
		return read((int8_t *)&t, sizeof(t));
	}
	force_inline bool deserialize(int32_t & t)
	{
		return read((int8_t *)&t, sizeof(t));
	}
	force_inline bool deserialize(int64_t & t)
	{
		return read((int8_t *)&t, sizeof(t));
	}
	force_inline bool deserialize(std::string & t)
	{
		store();
		uint16_t size = 0;
		if (!read((int8_t *)&size, sizeof(size)))
		{
			restore();
			return false;
		}

		t.clear();
		t.resize(size, 0);

		if (!read((int8_t *)t.c_str(), t.size()))
		{
			restore();
			return false;
		}

		return true;
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
	force_inline void store()
	{
		m_buffer.store();
	}
	force_inline void restore()
	{
		m_buffer.restore();
	}
private:
	buffer m_buffer;
	// 供外部调用的处理函数
	slot<buffer, bool (buffer::*)(const int8_t * p, size_t size)> m_write_slot;
	slot<buffer, bool (buffer::*)(int8_t * out, size_t size)> m_read_slot;
};

#else

template <typename buffer>
class fserialize
{
public:
	force_inline fserialize() : m_write_slot(&(buffer::write), &m_buffer),
		m_read_slot(&(buffer::read), &m_buffer)
	{

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
	bool serialize(const T & t);
	force_inline bool serialize(const int8_t & t);
 	force_inline bool serialize(const int16_t & t);
 	force_inline bool serialize(const int32_t & t);
 	force_inline bool serialize(const int64_t & t);
 	force_inline bool serialize(const std::string & t);
	
	template <typename T> force_inline
	bool deserialize(T & t);
	force_inline bool deserialize(int8_t & t);
	force_inline bool deserialize(int16_t & t);
	force_inline bool deserialize(int32_t & t);
	force_inline bool deserialize(int64_t & t);
	force_inline bool deserialize(std::string & t);
private:
	force_inline bool write(const int8_t * p, size_t size)
	{
		return m_buffer.write(p, size);
	}
	force_inline bool read(int8_t * out, size_t size)
	{
		return m_buffer.read(out, size);
	}
	force_inline void store()
	{
		m_buffer.store();
	}
	force_inline void restore()
	{
		m_buffer.restore();
	}
private:
	buffer m_buffer;
	// 供外部调用的处理函数
	slot<buffer, bool (buffer::*)(const int8_t * p, size_t size)> m_write_slot;
	slot<buffer, bool (buffer::*)(int8_t * out, size_t size)> m_read_slot;
};

template <typename buffer>
template <typename T> 
bool fserialize<buffer>::serialize(const T & t)
{
	return t.serialize(m_write_slot);
}

template <> 
template <typename buffer> force_inline
bool fserialize<buffer>::serialize(const int8_t & t)
{
	return write((const int8_t *)&t, sizeof(t));
}
template <> 
template <typename buffer> force_inline
bool fserialize<buffer>::serialize(const int16_t & t)
{
	return write((const int8_t *)&t, sizeof(t));
}
template <> 
template <typename buffer> force_inline
bool fserialize<buffer>::serialize(const int32_t & t)
{
	return write((const int8_t *)&t, sizeof(t));
}
template <> 
template <typename buffer> force_inline
bool fserialize<buffer>::serialize(const int64_t & t)
{
	return write((const int8_t *)&t, sizeof(t));
}
template <> 
template <typename buffer> force_inline
bool fserialize<buffer>::serialize(const std::string & t)
{
	store();
	uint16_t size = (uint16_t)t.size();
	if (!write((const int8_t *)&size, sizeof(size)))
	{
		restore();
		return false;
	}

	if (!write((const int8_t *)t.c_str(), t.size()))
	{
		restore();
		return false;
	}

	return true;
}

template <typename buffer>
template <typename T> 
bool fserialize<buffer>::deserialize(T & t)
{
	return t.deserialize(m_read_slot);
}
template <> 
template <typename buffer> force_inline
bool fserialize<buffer>::deserialize(int16_t & t)
{
	return read((int8_t *)&t, sizeof(t));
}
template <> 
template <typename buffer> force_inline
bool fserialize<buffer>::deserialize(int32_t & t)
{
	return read((int8_t *)&t, sizeof(t));
}
template <> 
template <typename buffer> force_inline
bool fserialize<buffer>::deserialize(int64_t & t)
{
	return read((int8_t *)&t, sizeof(t));
}
template <> 
template <typename buffer> force_inline
bool fserialize<buffer>::deserialize(std::string & t)
{
	store();
	uint16_t size = 0;
	if (!read((int8_t *)&size, sizeof(size)))
	{
		restore();
		return false;
	}

	t.clear();
	t.resize(size, 0);

	if (!read((int8_t *)t.c_str(), t.size()))
	{
		restore();
		return false;
	}

	return true;
}

#endif

