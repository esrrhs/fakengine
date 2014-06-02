#pragma once

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
