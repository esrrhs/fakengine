#pragma once

template <class T, uint32_t N>
class fstack
{
public:
	fstack()
	{
		clear();
	}

	~fstack()
	{

	}

	void clear()
	{
		used = 0;
	}

	bool pop(T & t)
	{
		if (empty())
		{
			return false;
		}
		t = data[used - 1];
		used--;
		return true;
	}

	bool push(const T & t)
	{
		if (full())
		{
			return false;
		}

		data[used] = t;
		used++;

		return true;
	}

	uint32_t size() const
	{
		return used;
	}

	bool empty()
	{
		return used == 0;
	}

	bool full()
	{
		return used == N;
	}
private:
	T data[N];
	uint32_t used;
};

