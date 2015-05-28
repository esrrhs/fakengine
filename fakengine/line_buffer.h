#pragma once

/*

iter:    m_iter(2)   m_datasize(8)
            |           |     
data:   * * * * * * * * * _ _ 
buffer: _ _ _ _ _ _ _ _ _ _ _ 
index:  0 1 2 3 4 5 6 7 8 9 10

*/

template <typename T, size_t N>
class line_buffer
{
public:
	line_buffer() : m_iter(0), m_datasize(0), m_store_iter(0), m_store_datasize(0)
	{
	}
	line_buffer(const line_buffer<T, N> & r) : m_iter(r.m_iter), m_datasize(r.m_datasize), 
		m_store_iter(r.m_store_iter),
		m_store_datasize(r.m_store_datasize)
	{
		memcpy(m_buffer, r.m_buffer, sizeof(T) * N);
	}
	~line_buffer()
	{
	}
	line_buffer<T, N> & operator = (const line_buffer<T, N> & r)
	{
		memcpy(m_buffer, r.m_buffer, sizeof(T) * N);
		m_iter = r.m_iter;
		m_datasize = r.m_datasize;
		m_store_iter = r.m_store_iter;
		m_store_datasize = r.m_store_datasize;
		return *this;
	}
public:
	force_inline bool can_write(size_t size)
	{
		return m_datasize + size <= N;
	}
	force_inline void skip_write(size_t size)
	{
		m_datasize += size;
	}
	force_inline bool write(const T * p, size_t size)
	{
		if (!can_write(size))
		{
			return false;
		}

		real_write(p, size);

		skip_write(size);

		return true;
	}
	template <typename OT>
	force_inline bool write(const OT & ot)
	{
		store();
		if (!ot.serialize(this))
		{
			restore();
			return false;
		}
		return true;
	}
	force_inline bool can_read(size_t size)
	{
		return m_iter + size <= m_datasize;
	}
	force_inline void skip_read(size_t size)
	{
		m_iter += size;
	}
	force_inline bool read(T * out, size_t size)
	{
		if (!can_read(size))
		{
			return false;
		}

		real_read(out, size);

		skip_read(size);

		return true;
	}
	template <typename OT>
	force_inline bool read(OT & ot)
	{
		store();
		if (!ot.deserialize(this))
		{
			restore();
			return false;
		}
		return true;
	}
	force_inline void store()
	{
		m_store_datasize = m_datasize;
		m_store_iter = m_iter;
	}
	force_inline void restore()
	{
		m_datasize = m_store_datasize;
		m_iter = m_store_iter;
	}
	force_inline bool clear()
	{
		m_datasize = 0;
		m_iter = 0;
		return true;
	}
	force_inline size_t size() const
	{
		return m_datasize;
	}
	force_inline size_t capacity() const
	{
		return N;
	}
	force_inline bool empty() const
	{
		return size() == 0;
	}
	force_inline bool full() const
	{
		return size() == capacity();
	}
	template <typename OT>
	force_inline bool serialize(OT & t) const
	{
		// 写入size
		size_t data_size = size() * sizeof(T);
		FASSERT(c_buffer_size_size <= sizeof(size_t));
		FASSERT(data_size <= (1<<(c_buffer_size_size * 8)));
		if (!t.write((int8_t*)&data_size, c_buffer_size_size))
		{
			return false;
		}

		// 写入data
		if (!t.write((const int8_t*)m_buffer, data_size))
		{
			return false;
		}

		return true;
	}
	template <typename OT>
	force_inline bool deserialize(OT & t)
	{
		// 读出size
		size_t data_size = 0;
		FASSERT(c_buffer_size_size <= sizeof(size_t));
		if (!t.read((int8_t*)&data_size, c_buffer_size_size))
		{
			return false;
		}

		// 读出data
		FASSERT(data_size <= N * sizeof(T));
		data_size = Min<size_t>(data_size, N * sizeof(T));
		if (!t.read((int8_t*)m_buffer, data_size))
		{
			return false;
		}
		m_datasize = data_size;
		m_iter = 0;

		return true;
	}
private:
	force_inline void real_write(const T * p, size_t size)
	{
		memcpy(m_buffer + m_datasize, p, size * sizeof(T));
	}
	force_inline void real_read(T * out, size_t size)
	{
		memcpy(out, m_buffer + m_iter, size * sizeof(T));
	}
private:
	T m_buffer[N];
	size_t m_iter;
	size_t m_datasize;
	size_t m_store_iter;
	size_t m_store_datasize;
};

