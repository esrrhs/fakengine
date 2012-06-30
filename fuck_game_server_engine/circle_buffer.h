#pragma once

/*
type:           [1]
iter:     begin(2)     end(8)
            |           |
data:   _ _ * * * * * * _ _ _ 
buffer: _ _ _ _ _ _ _ _ _ _ _ 
index:  0 1 2 3 4 5 6 7 8 9 10

type:           [2]
iter:      end(2)   begin(7)
            |         |
data:   * * _ _ _ _ _ * * * *  
buffer: _ _ _ _ _ _ _ _ _ _ _ 
index:  0 1 2 3 4 5 6 7 8 9 10


type:           [3]
iter:      begin(4),end(4)
                |
data:   _ _ _ _ _ _ _ _ _ _ _ 
buffer: _ _ _ _ _ _ _ _ _ _ _ 
index:  0 1 2 3 4 5 6 7 8 9 10

type:           [4]
iter:      begin(4),end(4)
|				 |
data:   * * * * * * * * * * *
buffer: _ _ _ _ _ _ _ _ _ _ _
index:  0 1 2 3 4 5 6 7 8 9 10

*/
template <typename T, size_t N>
class cirle_buffer
{
public:
	cirle_buffer() : m_buffer(0), m_datasize(0), m_begin(0), m_end(0),
		m_store_datasize(0), m_store_begin(0), m_store_end(0)
	{
		m_buffer = (int8_t*)FALLOC(N * sizeof(T));
	}
	cirle_buffer(const cirle_buffer<T, N> & r) : m_datasize(r.m_datasize), 
		m_begin(r.m_begin), m_end(r.m_end), m_store_datasize(r.m_store_datasize), 
		m_store_begin(r.m_store_begin), m_store_end(r.m_store_end)
	{
		m_buffer = (int8_t*)FALLOC(N * sizeof(T));
		memcpy(m_buffer, r.m_buffer, sizeof(T) * N);
	}
	~cirle_buffer()
	{
		SAFE_FREE(m_buffer);
	}
	cirle_buffer<T, N> & operator = (const cirle_buffer<T, N> & r)
	{
		memcpy(m_buffer, r.m_buffer, sizeof(T) * N);
		m_datasize = r.m_datasize;
		m_begin = r.m_begin;
		m_end = r.m_end;
		m_store_datasize = r.m_store_datasize;
		m_store_begin = r.m_store_begin;
		m_store_end = r.m_store_end;
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
		m_end += size;
		if (m_end >= N)
		{
			m_end -= N;
		}
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
		if (!ot.serialize(*this))
		{
			restore();
			return false;
		}
		return true;
	}
	force_inline bool can_read(size_t size)
	{
		return m_datasize >= size;
	}
	force_inline void skip_read(size_t size)
	{
		m_datasize -= size;
		m_begin += size;
		if (m_begin >= N)
		{
			m_begin -= N;
		}
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
		if (!ot.deserialize(*this))
		{
			restore();
			return false;
		}
		return true;
	}
	force_inline void store()
	{
		m_store_datasize = m_datasize;
		m_store_begin = m_begin;
		m_store_end = m_end;
	}
	force_inline void restore()
	{
		m_datasize = m_store_datasize;
		m_begin = m_store_begin;
		m_end = m_store_end;
	}
	force_inline bool clear()
	{
		m_datasize = 0;
		m_begin = 0;
		m_end = 0;
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
	force_inline T * get_read_line_buffer()
	{
		return m_buffer + m_begin;
	}
	force_inline size_t get_read_line_size()
	{
		return std::min<size_t>(m_datasize, N - m_begin);
	}
	force_inline T * get_write_line_buffer()
	{
		return m_buffer + m_end;
	}
	force_inline size_t get_write_line_size()
	{
		return std::min<size_t>(N - m_datasize, N - m_end);
	}
private:
	force_inline void real_write(const T * p, size_t size)
	{
		if (m_end >= m_begin)	// [1][3]
		{
			// 能装下
			if (N - m_end >= size)
			{
				memcpy(m_buffer + m_end, p, size * sizeof(T));
			}
			else
			{
				memcpy(m_buffer + m_end, p, (N - m_end) * sizeof(T));
				memcpy(m_buffer, p + (N - m_end), (size - (N - m_end)) * sizeof(T));
			}
		}
		else	//[2]
		{
			memcpy(m_buffer + m_end, p, size * sizeof(T));
		}
	}
	force_inline void real_read(T * out, size_t size)
	{
		if (m_begin >= m_end)	// [2][4]
		{
			// 能读完
			if (N - m_begin >= size)
			{
				memcpy(out, m_buffer + m_begin, size * sizeof(T));
			}
			else
			{
				memcpy(out, m_buffer + m_begin, (N - m_begin) * sizeof(T));
				memcpy(out + (N - m_begin), m_buffer, (size - (N - m_begin)) * sizeof(T));
			}
		}
		else	// [1]
		{
			memcpy(out, m_buffer + m_begin, size * sizeof(T));
		}
	}
private:
	T * m_buffer;
	size_t m_datasize;
	size_t m_begin;
	size_t m_end;
	size_t m_store_datasize;
	size_t m_store_begin;
	size_t m_store_end;
};

