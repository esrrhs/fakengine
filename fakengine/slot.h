#pragma once

template <typename T, typename F>
class slot
{
public:
	slot() : m_function(0), m_object(0)
	{
	}
	
	slot(F func, T * obj) : m_function(func), m_object(obj)
	{
	}

	~slot() 
	{
	}

	void ini(F func, T * obj)
	{
		m_function = func;
		m_object = obj;
	}

	bool operator()()
	{
		return (m_object->*m_function)();
	}

	template <typename P1>
	bool operator()(P1 param1) 
	{
		return (m_object->*m_function)(param1);
	}

	template <typename P1>
	bool operator()(P1 param1) const
	{
		return (m_object->*m_function)(param1);
	}

	template <typename P1, typename P2>
	bool operator()(P1 param1, P2 param2)
	{
		return (m_object->*m_function)(param1, param2);
	}

	template <typename P1, typename P2>
	bool operator()(P1 param1, P2 param2) const
	{
		return (m_object->*m_function)(param1, param2);
	}

	template <typename P1, typename P2, typename P3>
	bool operator()(P1 param1, P2 param2, P3 param3)
	{
		return (m_object->*m_function)(param1, param2, param3);
	}

	template <typename P1, typename P2, typename P3>
	bool operator()(P1 param1, P2 param2, P3 param3) const
	{
		return (m_object->*m_function)(param1, param2, param3);
	}
private:
	F m_function;
	T * m_object;
};

template <typename F>
class slot<void, F>
{
public:
	slot() 
	{
	}

	slot(F f) : m_function(f) {}

	~slot() 
	{
	}

	bool operator()()
	{
		return m_function();
	}

	template <typename P1>
	bool operator()(P1 param1) 
	{
		return m_function(param1);
	}

	template <typename P1>
	bool operator()(P1 param1) const
	{
		return m_function(param1);
	}

	template <typename P1, typename P2>
	bool operator()(P1 param1, P2 param2)
	{
		return m_function(param1, param2);
	}

	template <typename P1, typename P2>
	bool operator()(P1 param1, P2 param2) const
	{
		return m_function(param1, param2);
	}

	template <typename P1, typename P2, typename P3>
	bool operator()(P1 param1, P2 param2, P3 param3)
	{
		return m_function(param1, param2, param3);
	}

	template <typename P1, typename P2, typename P3>
	bool operator()(P1 param1, P2 param2, P3 param3) const
	{
		return m_function(param1, param2, param3);
	}
private:
	F m_function;
};

