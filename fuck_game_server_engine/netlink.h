#pragma once

// netlink ���ӿͻ�����
// _send ����Ϣʵ�壬����һ�������߳�+�����������������߳�
// _recv ����Ϣʵ�壬����һ�������߳�+�����������������߳�
template <typename _send, typename _recv>
class netlink
{
public:
	netlink()
	{
	}
	~netlink()
	{
	}
public:
	template<typename _msg>
	FORCEINLINE bool send_msg(const _msg * msg)
	{
		return m_send.send_msg(msg);
	}
	template<typename _msg>
	FORCEINLINE bool recv_msg(_msg * msg)
	{
		return m_recv.recv_msg(msg);
	}
private:
	_send m_send;
	_recv m_recv;
};

