#pragma once

template <typename _queue>
class tcpsocket
{
public:
	tcpsocket()
	{
	}
	~tcpsocket()
	{
	}
public:
	template<typename _msg>
	FORCEINLINE bool send(const _msg * msg)
	{
		int8_t * p = 0;
		size_t size = 0;
		if (!msg->to_buffer(p, size))
		{
			return false;
		}
	
		if (m_queue.write(p, size))
		{
			return true;
		}
		return false;
	}
	template<typename _msg>
	FORCEINLINE bool recv(_msg * msg)
	{
		size_t read_size = 0;
		if (msg->from_buffer(&_queue::read, read_size))
		{
			m_queue.skip(read_size);
			return true;
		}
		return false;
	}
private:
	_queue m_queue;
};

