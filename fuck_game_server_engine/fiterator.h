#pragma once

template <typename Container>
class fiterator
{
public:
	typedef typename Container::Value T;
	force_inline fiterator(Container * _Container) : m_index(0), m_Container(_Container)
	{

	}
	force_inline fiterator(Container * _Container, int32_t _index) : m_index(_index), m_Container(_Container)
	{

	}
	force_inline ~fiterator()
	{

	}

	force_inline T & operator*() const
	{	// return designated value
		return m_Container->getbyidx(m_index);
	}

	force_inline T * operator->() const
	{	// return pointer to class object
		return &(m_Container->getbyidx(m_index));
	}

	force_inline fiterator & operator++()
	{	// preincrement
		m_index = m_Container->getnextidx(m_index);
		return (*this);
	}

	force_inline fiterator operator++(int)
	{	// postincrement
		fiterator _Tmp = *this;
		++*this;
		return (_Tmp);
	}

	force_inline fiterator & operator--()
	{	// predecrement
		m_index = m_Container->getpreidx(m_index);
		return (*this);
	}

	force_inline fiterator operator--(int)
	{	// postdecrement
		fiterator _Tmp = *this;
		--*this;
		return (_Tmp);
	}

	force_inline bool operator==(const fiterator & _Right) const
	{	// test for iterator equality
		return m_index == _Right.m_index;
	}

	force_inline bool operator!=(const fiterator & _Right) const
	{	// test for iterator inequality
		return (!(*this == _Right));
	}

	force_inline fiterator & operator=(const fiterator & _Right)
	{	
		m_index = _Right.m_index;
		m_Container = _Right.m_Container;
		return (*this);
	}

	force_inline int32_t index() const
	{
		return m_index;
	}

private:
	int32_t m_index;
	Container * m_Container;
};
