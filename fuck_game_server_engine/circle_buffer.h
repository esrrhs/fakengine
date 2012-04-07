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
	cirle_buffer() : m_buffer(0), m_size(0), m_datasize(0), m_begin(0), m_end(0)
	{
		m_buffer = (int8_t*)FALLOC(N * sizeof(T));
		m_size = N;
	}
	~cirle_buffer()
	{
		SAFE_FREE(m_buffer);
	}
public:
	FORCEINLINE bool try_write(const T * p, size_t size)
	{
		if (!can_write(size))
		{
			return false;
		}

		real_write(p, size);

		return true;
	}
	FORCEINLINE bool can_write(size_t size)
	{
		return m_datasize + size <= m_size;
	}
	FORCEINLINE void skip_write(size_t size)
	{
		m_datasize += size;
		m_end += size;
		if (m_end >= m_size)
		{
			m_end -= m_size;
		}
	}
	FORCEINLINE bool write(const T * p, size_t size)
	{
		if (!can_write(size))
		{
			return false;
		}

		real_write(p, size);

		skip_write(size);

		return true;
	}

	FORCEINLINE bool try_read(T * out, size_t size)
	{
		if (!can_read(size))
		{
			return false;
		}

		real_read(out, size);

		return true;
	}
	FORCEINLINE bool can_read(size_t size)
	{
		return m_datasize >= size;
	}
	FORCEINLINE void skip_read(size_t size)
	{
		m_datasize -= size;
		m_begin += size;
		if (m_begin >= m_size)
		{
			m_begin -= m_size;
		}
	}
	FORCEINLINE bool read(T * out, size_t size)
	{
		if (!can_read(size))
		{
			return false;
		}

		real_read(p, size);

		skip_read(size);

		return true;
	}
	FORCEINLINE bool clear()
	{
		return true;
	}
private:
	FORCEINLINE void real_write(const T * p, size_t size)
	{
		if (m_end >= m_begin)	// [1][3]
		{
			// 能装下
			if (m_size - m_end >= size)
			{
				memcpy(m_buffer + m_end, p, size * sizeof(T));
			}
			else
			{
				memcpy(m_buffer + m_end, p, (m_size - m_end) * sizeof(T));
				memcpy(m_buffer, p + (m_size - m_end), (size - (m_size - m_end)) * sizeof(T));
			}
		}
		else	//[2]
		{
			memcpy(m_buffer + m_end, p, size * sizeof(T));
		}
	}
	FORCEINLINE void real_read(T * out, size_t size)
	{
		if (m_begin >= m_end)	// [2][4]
		{
			// 能读完
			if (m_size - m_begin >= size)
			{
				memcpy(out, m_buffer + m_begin, size * sizeof(T));
			}
			else
			{
				memcpy(out, m_buffer + m_begin, (m_size - m_begin) * sizeof(T));
				memcpy(out + (m_size - m_begin), m_buffer, (size - (m_size - m_begin)) * sizeof(T));
			}
		}
		else	// [1]
		{
			memcpy(out, m_buffer + m_begin, size * sizeof(T));
		}
	}
private:
	T * m_buffer;
	size_t m_size;
	size_t m_datasize;
	size_t m_begin;
	size_t m_end;
};

