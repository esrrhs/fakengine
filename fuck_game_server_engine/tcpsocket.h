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
		const char * p = msg->to_buffer();
		size_t size = msg->to_buffer_size();

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

