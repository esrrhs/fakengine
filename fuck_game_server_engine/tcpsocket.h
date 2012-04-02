#pragma once

template <typename _queue>
class tcpsocket
{
public:
	tcpsocket() : m_send_slot(&(_queue::write), &m_send_queue),
		m_recv_slot(&(_queue::read), &m_recv_queue)
	{
	}
	~tcpsocket()
	{
	}
public:
	template<typename _msg>
	FORCEINLINE bool send(const _msg * msg)
	{
		size_t write_size = 0;
		if (msg->to_buffer(m_send_slot, write_size))
		{
			m_send_queue.skip_read(write_size);
			return true;
		}
		return false;
	}
	template<typename _msg>
	FORCEINLINE bool recv(_msg * msg)
	{
		size_t read_size = 0;
		if (msg->from_buffer(m_recv_slot, read_size))
		{
			m_recv_queue.skip_write(read_size);
			return true;
		}
		return false;
	}
private:
	_queue m_send_queue;
	_queue m_recv_queue;
	slot<_queue, bool (_queue::*)(const int8_t * p, size_t size)> m_send_slot;
	slot<_queue, bool (_queue::*)(int8_t * out, size_t size)> m_recv_slot;
};

