#pragma once

template <typename T, uint32_t N>
class farray
{
public:
	typedef farray<T, N> MyType;
	typedef T Value;
	typedef fiterator<MyType> iterator;
	friend class fiterator<MyType>;
public:
	force_inline farray() : m_tmpdata(T())
	{
		clear();
	}

	force_inline ~farray()
	{

	}

	force_inline T& operator [](uint32_t index)
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return m_tmpdata;
		}

		return m_pool[m_indexarray[index]].data;
	}

	force_inline const T& operator [](uint32_t index) const
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return m_tmpdata;
		}

		return m_pool[m_indexarray[index]].data;
	}

	force_inline void clear()
	{
		m_pool.clear();
		memset(m_indexarray, 0xFF, sizeof(m_indexarray));
	}

	force_inline bool push_back(const T & t)
	{
		if (full())
		{
			return false;
		}

		uint32_t index = size();

		uint32_t newidx = m_pool.allocindex();
		SAFE_TEST_RET_VAL((int32_t)newidx, INVALID_IDX, false);

		Node & node = m_pool[newidx];
		node.data = t;

		m_indexarray[index] = newidx;

		return true;
	}

	force_inline bool pop_back(T & t)
	{
		if (empty())
		{
			return false;
		}

		uint32_t index = size() - 1;
		uint32_t idx = m_indexarray[index];

		Node & node = m_pool[idx];
		t = node.data;

		m_pool.deallocindex(idx);

		return true;
	}

	force_inline bool back(T & t) const
	{
		if (empty())
		{
			return false;
		}

		uint32_t index = size() - 1;
		uint32_t idx = m_indexarray[index];

		const Node & node = m_pool[idx];
		t = node.data;

		return true;
	}

	force_inline bool push_front(const T & t)
	{
		if (full())
		{
			return false;
		}

		int32_t s = size();

		uint32_t newidx = m_pool.allocindex();
		SAFE_TEST_RET_VAL((int32_t)newidx, INVALID_IDX, false);

		Node & node = m_pool[newidx];
		node.data = t;

		if (s > 0)
		{
			memmove(m_indexarray + 1, m_indexarray, s * sizeof(int32_t));
		}
		m_indexarray[0] = newidx;

		return true;
	}

	force_inline bool pop_front(T & t)
	{
		if (empty())
		{
			return false;
		}

		int32_t s = size();

		uint32_t idx = m_indexarray[0];

		Node & node = m_pool[idx];
		t = node.data;

		m_pool.deallocindex(idx);

		m_indexarray[0] = INVALID_IDX;
		if (s > 1)
		{
			memmove(m_indexarray, m_indexarray + 1, (s - 1) * sizeof(int32_t));
		}

		return true;
	}

	force_inline bool front(T & t) const
	{
		if (empty())
		{
			return false;
		}

		uint32_t idx = m_indexarray[0];

		const Node & node = m_pool[idx];
		t = node.data;

		return true;
	}

	force_inline bool insert(int32_t index, const T & t)
	{
		if (full())
		{
			return false;
		}

		int32_t s = size();
		if (index < 0 || index > s)
		{
			return false;
		}

		uint32_t newidx = m_pool.allocindex();
		SAFE_TEST_RET_VAL((int32_t)newidx, INVALID_IDX, false);

		Node & node = m_pool[newidx];
		node.data = t;

		if (s - index > 0)
		{
			memmove(m_indexarray + index + 1, m_indexarray + index, (s - index)*sizeof(int32_t));
		}
		m_indexarray[index] = newidx;

		return true;
	}

	force_inline bool erase(int32_t index)
	{
		if (empty())
		{
			return false;
		}

		int32_t s = size();
		SAFE_TEST_INDEX_VAL(index, s, false);

		uint32_t idx = m_indexarray[index];

		m_pool.deallocindex(idx);

		m_indexarray[index] = INVALID_IDX;

		if (s - index > 1)
		{
			memmove(m_indexarray + index, m_indexarray + index + 1, (s - index - 1)*sizeof(int32_t));
		}

		return true;
	}

	force_inline uint32_t size() const
	{
		return m_pool.size();
	}

	force_inline bool empty() const
	{
		return m_pool.empty();
	}

	force_inline bool full() const
	{
		return m_pool.full();
	}

	force_inline iterator begin()
	{
		return iterator(this, 0);
	}

	force_inline iterator end()
	{
		return iterator(this, size());
	}

private:

	force_inline T& getbyidx(uint32_t index)
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return m_tmpdata;
		}

		return m_pool[m_indexarray[index]].data;
	}

	force_inline const T& getbyidx(uint32_t index) const
	{
		if (index>=N)
		{
			FASSERT(index>=N);
			return m_tmpdata;
		}

		return m_pool[m_indexarray[index]].data;
	}

	force_inline int32_t getnextidx(int32_t idx)
	{
		if (idx >= 0 && idx + 1 < (int32_t)size())
		{
			return idx + 1;
		}
		return size();
	}

	force_inline int32_t getpreidx(int32_t idx)
	{
		if (idx - 1 >= 0 && idx < (int32_t)size())
		{
			return idx - 1;
		}
		return size();
	}

private:
	struct Node
	{
		T data;
	};
	typedef fpool<Node, N> Pool;
	static const int32_t INVALID_IDX = Pool::INVALID_IDX;
private:
	Pool m_pool;
	int32_t m_indexarray[N];
	T m_tmpdata;
};

