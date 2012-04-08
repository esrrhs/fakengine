#pragma once

template <typename T, typename F>
class slot
{
public:
	slot() 
	{
	}
	
	slot(F func, T * obj) : d_function(func), d_object(obj) {}

	~slot() 
	{
	}

	bool operator()()
	{
		return (d_object->*d_function)();
	}

	template <typename P1>
	bool operator()(P1 param1) 
	{
		return (d_object->*d_function)(param1);
	}

	template <typename P1>
	bool operator()(P1 param1) const
	{
		return (d_object->*d_function)(param1);
	}

	template <typename P1, typename P2>
	bool operator()(P1 param1, P2 param2)
	{
		return (d_object->*d_function)(param1, param2);
	}

	template <typename P1, typename P2>
	bool operator()(P1 param1, P2 param2) const
	{
		return (d_object->*d_function)(param1, param2);
	}
private:
	F d_function;
	T * d_object;
};

template <typename F>
class slot<void, F>
{
public:
	slot() 
	{
	}

	slot(F f) : d_function(f) {}

	~slot() 
	{
	}

	bool operator()()
	{
		return d_function();
	}

	template <typename P1>
	bool operator()(P1 param1) 
	{
		return d_function(param1);
	}

	template <typename P1>
	bool operator()(P1 param1) const
	{
		return d_function(param1);
	}

	template <typename P1, typename P2>
	bool operator()(P1 param1, P2 param2)
	{
		return d_function(param1, param2);
	}

	template <typename P1, typename P2>
	bool operator()(P1 param1, P2 param2) const
	{
		return d_function(param1, param2);
	}
private:
	F d_function;
};

