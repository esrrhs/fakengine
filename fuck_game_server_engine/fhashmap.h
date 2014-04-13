#pragma once

template <typename T>
size_t fhash(T t)
{
	return t;
}

template <typename K, typename V, size_t N>
class farraymap
{
public:
	farraymap() 
	{
		m_node = new _node[N];
		m_len = N;
		m_datalen = 0;
	}
	~farraymap()
	{
		delete []m_node;
	}
	struct _node
	{
		_node() : k(K()), v(V()) {}
		K k;
		V v;
	};
	
	force_inline bool get(size_t pos, K & k, V & v)
	{
		if (pos < m_datalen)
		{
			k = m_node[pos].k;
			v = m_node[pos].v;
			return true;
		}
		return false;
	}

	force_inline bool add(K k, V v)
	{
		if (full())
		{
			grow();
		}

		size_t pos = find_pos(k);
		if (pos != (size_t)-1)
		{
			m_node[pos].v = v;
			return false;
		}
		else
		{
			m_node[m_datalen].k = k;
			m_node[m_datalen].v = v;
			m_datalen++;
			return true;
		}
	}
	force_inline V find(K k)
	{
		size_t pos = find_pos(k);
		if (pos != (size_t)-1)
		{
			return m_node[pos].v;
		}
		return V();
	}
	force_inline bool del(K k)
	{
		size_t pos = find_pos(k);
		if (pos != (size_t)-1)
		{
			m_node[pos] = m_node[m_datalen - 1];
			m_node[m_datalen - 1] = _node();
			m_datalen--;
			return true;
		}
		return false;
	}
	force_inline bool full()
	{
		return m_datalen >= m_len;
	}
	force_inline bool empty()
	{
		return m_datalen == 0;
	}
	force_inline size_t size()
	{
		return m_len;
	}
	force_inline size_t data_size()
	{
		return m_datalen;
	}
private:
	force_inline size_t find_pos(K k)
	{
		for (size_t i = 0; i < m_datalen; i++)
		{
			if (m_node[i].k == k)
			{
				return i;
			}
		}
		return -1;
	}
	force_inline void grow()
	{
		size_t newlen = m_datalen + N;
		_node * pnode = new _node[newlen];
		memcpy(pnode, m_node, sizeof(_node) * m_datalen);
		delete []m_node;
		m_node = pnode;
		m_len = newlen;
	}
private:
	_node * m_node;
	size_t m_len;
	size_t m_datalen;
};

template <typename K, typename V, size_t N, size_t AN>
class fhashmap
{
public:
	fhashmap()
	{
		m_array_map = new farraymap<K, V, AN>[N];
		m_len = N;
		m_datalen = 0;
	}
	~fhashmap()
	{
		delete []m_array_map;
	}

	force_inline void add(K k, V v)
	{
		if (full())
		{
			grow();
		}
		farraymap<K, V, AN> & array = m_array_map[fhash<K>(k) % m_len];
		if (array.add(k, v))
		{
			m_datalen++;
		}
	}
	force_inline V find(K k)
	{
		farraymap<K, V, AN> & array = m_array_map[fhash<K>(k) % m_len];
		return array.find(k);
	}
	force_inline void del(K k)
	{
		farraymap<K, V, AN> & array = m_array_map[fhash<K>(k) % m_len];
		if (array.del(k))
		{
			m_datalen--;
		}
	}
	force_inline bool full()
	{
		return m_datalen >= m_len;
	}
	force_inline bool empty()
	{
		return m_datalen == 0;
	}
	force_inline size_t size()
	{
		return m_len;
	}
	force_inline size_t data_size()
	{
		return m_datalen;
	}
private:
	void grow()
	{
		farraymap<K, V, AN> * old_array_map = m_array_map;
		size_t oldlen = m_len;
		size_t newlen = m_len + N;
		m_array_map = new farraymap<K, V, AN>[newlen];
		m_datalen = 0;
		m_len = newlen;
		for (size_t i = 0; i < oldlen; i++)
		{
			farraymap<K, V, AN> & array = old_array_map[i];
			for (size_t j = 0; j < array.data_size(); j++)
			{
				K k;
				V v;
				if (array.get(j, k, v))
				{
					add(k, v);
				}
			}
		}
		delete []old_array_map;
	}
private:
	farraymap<K, V, AN> * m_array_map;
	size_t m_len;
	size_t m_datalen;
};
