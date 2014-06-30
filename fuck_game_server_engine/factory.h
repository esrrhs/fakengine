#pragma once

template <typename Base>
class ifactory
{
public:
	virtual Base * alloc() = 0;
	virtual bool dealloc(Base * pBase) = 0;
};

template <typename T, typename Base, typename Alloctor=fallocator<T, normal_allocator<T> > >	
class factory : public ifactory<Base>
{
public:
	virtual Base * alloc()
	{
		Base * pBase = m_alloctor.allocate(1);
		SAFE_TEST_RET_VAL(pBase, NULL, NULL);
		new (pBase) T ();
		return pBase;
	}
	virtual bool dealloc(Base * pBase)
	{
		SAFE_TEST_RET_VAL(pBase, NULL, NULL);
		((T*)pBase)->~T();
		m_alloctor.deallocate((T*)pBase, 1);
		return true;
	}
private:
	Alloctor m_alloctor;
};

template <typename Base, uint32_t N, typename Key=uint32_t>
class factorymng : public singleton< factorymng<Base, N, Key> >
{
public:
	typedef ifactory<Base> IFactory;
	typedef fhashmap<Key, IFactory*, N> FactoryMap;
public:
	bool regist(Key type, IFactory * pfactory)
	{
		m_map[type] = pfactory;
		return true;
	}
	Base * alloc(Key type)
	{
		IFactory * pfactory = m_map[type];
		SAFE_TEST_RET_VAL(pfactory, NULL, NULL);
		return pfactory->alloc();
	}
	bool dealloc(Base * pBase)
	{
		Key type = pBase->gettype();
		IFactory * pfactory = m_map[type];
		SAFE_TEST_RET_VAL(pfactory, NULL, false);
		return pfactory->dealloc(pBase);
	}
private:
	FactoryMap m_map;
};

