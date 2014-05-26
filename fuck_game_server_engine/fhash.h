#pragma once

template <typename T>
struct fcmp
{
    bool operator()(const T & t1, const T & t2) const
    {
    	return t1 == t2;
    }
};

template <typename T>
struct fhash
{
    size_t operator()(const T & t) const
    {
    	return size_t(t);
    }
};

template<uint32_t Cur, uint32_t Next>
class fhashersize
{
public:
    static const uint32_t SIZE = 0;
};

template<uint32_t Cur>
class fhashersize<4294967291ul, Cur>
{
public:
    static const uint32_t SIZE = Cur < 4294967291ul ? 4294967291ul : Cur;
};

#define DEF_HASHER_SIZE(PRE, NEXT) \
template<uint32_t Cur>\
class fhashersize<PRE, Cur>\
{\
public:\
    static const uint32_t SIZE = Cur < PRE ? PRE : fhashersize<NEXT, Cur>::SIZE;\
};

DEF_HASHER_SIZE(3221225473ul, 4294967291ul)
DEF_HASHER_SIZE(1610612741ul, 3221225473ul)
DEF_HASHER_SIZE(805306457ul, 1610612741ul)
DEF_HASHER_SIZE(402653189ul, 805306457ul)
DEF_HASHER_SIZE(201326611ul, 402653189ul)
DEF_HASHER_SIZE(100663319ul, 201326611ul)
DEF_HASHER_SIZE(50331653ul, 100663319ul)
DEF_HASHER_SIZE(25165843ul, 50331653ul)
DEF_HASHER_SIZE(12582917ul, 25165843ul)
DEF_HASHER_SIZE(6291469ul, 12582917ul)
DEF_HASHER_SIZE(3145739ul, 6291469ul)
DEF_HASHER_SIZE(1572869ul, 3145739ul)
DEF_HASHER_SIZE(786433ul, 1572869ul)
DEF_HASHER_SIZE(393241ul, 786433ul)
DEF_HASHER_SIZE(196613ul, 393241ul)
DEF_HASHER_SIZE(98317ul, 196613ul)
DEF_HASHER_SIZE(49157ul, 98317ul)
DEF_HASHER_SIZE(24593ul, 49157ul)
DEF_HASHER_SIZE(12289ul, 24593ul)
DEF_HASHER_SIZE(6151ul, 12289ul)
DEF_HASHER_SIZE(3079ul, 6151ul)
DEF_HASHER_SIZE(1543ul, 3079ul)
DEF_HASHER_SIZE(769ul, 1543ul)
DEF_HASHER_SIZE(193ul, 769ul)
DEF_HASHER_SIZE(97ul, 193ul)
DEF_HASHER_SIZE(53ul, 97ul)

template <typename Container>
class fiterator
{
public:
	typedef typename Container::Value T;
	fiterator(Container * _Container) : m_index(0), m_Container(_Container)
	{

	}
	fiterator(Container * _Container, int32_t _index) : m_index(_index), m_Container(_Container)
	{

	}
	~fiterator()
	{

	}

	T & operator*() const
	{	// return designated value
		return m_Container->getbyidx(m_index);
	}

	T * operator->() const
	{	// return pointer to class object
		return &(m_Container->getbyidx(m_index));
	}

	fiterator & operator++()
	{	// preincrement
		m_index = m_Container->getnextidx(m_index);
		return (*this);
	}

	fiterator operator++(int)
	{	// postincrement
		fiterator _Tmp = *this;
		++*this;
		return (_Tmp);
	}

	fiterator & operator--()
	{	// predecrement
		m_index = m_Container->getpreidx(m_index);
		return (*this);
	}

	fiterator operator--(int)
	{	// postdecrement
		fiterator _Tmp = *this;
		--*this;
		return (_Tmp);
	}

	bool operator==(const fiterator & _Right) const
	{	// test for iterator equality
		return m_index == _Right.m_index;
	}

	bool operator!=(const fiterator & _Right) const
	{	// test for iterator inequality
		return (!(*this == _Right));
	}

	fiterator & operator=(const fiterator & _Right)
	{	
		m_index = _Right.m_index;
		m_Container = _Right.m_Container;
		return (*this);
	}

	int32_t index() const
	{
		return m_index;
	}

private:
	int32_t m_index;
	Container * m_Container;
};
