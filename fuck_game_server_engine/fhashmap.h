#pragma once

template <typename K, typename V, uint32_t N, typename HashFunc = fhash< K >, typename CmpFunc = fcmp< K > >
class fhashmap
{
public:
	struct Node
	{
		Node()
		{
		}
		Node(const K & _k) : first(_k)
		{
		}
		Node(const K & _k, const V & _v) : first(_k), second(_v)
		{
		}
		~Node()
		{
		}
		K first;
		V second;
	};
public:
	typedef fhashmap<K, V, N, HashFunc, CmpFunc> MyType;
	typedef Node Value;
	typedef fiterator<MyType> iterator;
	friend class fiterator<MyType>;
public:
	force_inline fhashmap()
	{
		clear();
	}

	force_inline ~fhashmap()
	{

	}

	force_inline V& operator [](const K & k)
	{
		return m_set[Node(k)].second;
	}

	force_inline const V& operator [](const K & k) const
	{
		return m_set[Node(k)].second;
	}

	force_inline void clear()
	{
		m_set.clear();
	}

	force_inline iterator insert(const K & k, const V & v)
	{
		return iterator(this, m_set.insert(Node(k, v)).index());
	}
	
	force_inline iterator find(const K & k)
	{
		return iterator(this, m_set.find(Node(k)).index());
	}

	force_inline iterator erase(const K & k)
	{
		return iterator(this, m_set.erase(Node(k)).index());
	}

	force_inline uint32_t size() const
	{
		return m_set.size();
	}

	force_inline bool empty() const
	{
		return m_set.empty();
	}

	force_inline bool full() const
	{
		return m_set.full();
	}

	force_inline iterator begin()
	{
		return iterator(this, m_set.begin().index());
	}

	force_inline iterator end()
	{
		return iterator(this, m_set.end().index());
	}

private:

	force_inline Node& getbyidx(uint32_t index)
	{
		return m_set.getbyidx(index);
	}

	force_inline const Node& getbyidx(uint32_t index) const
	{
		return m_set.getbyidx(index);
	}

	force_inline int32_t getnextidx(int32_t idx)
	{
		return m_set.getnextidx(idx);
	}

	force_inline int32_t getpreidx(int32_t idx)
	{
		return m_set.getpreidx(idx);
	}

private:
    struct NodeCmp
    {
        force_inline bool operator()(const Node & n1, const Node & n2) const
        {
        	return m_CmpFunc(n1.first, n2.first);
        }
	    CmpFunc m_CmpFunc;
    };
    struct NodeHash
    {
        force_inline size_t operator()(const Node & node) const
        {
        	return m_HashFunc(node.first);
        }
	    HashFunc m_HashFunc;
    };
	typedef fhashset<Node, N, NodeHash, NodeCmp> Hashset;
private:
	Hashset m_set;
};


