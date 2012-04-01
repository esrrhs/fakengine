#pragma once

// netlink ���ӿͻ�����
// _container ��������ʵ�壬����accept��select
// _send ����Ϣʵ�壬�˴����ܾ�ֻ�ǻ���������������_container
// _recv ����Ϣʵ�壬�˴����ܾ�ֻ�ǻ���������������_container
template <typename _container, typename _send, typename _recv>
class netserver
{
public:
	netserver()
	{
	}
	~netserver()
	{
	}
public:
	template<typename _msg>
	FORCEINLINE bool send_msg(netlink<_send, _recv> & link, const _msg * msg)
	{
		return link.send_msg(msg);
	}
	template<typename _msg>
	FORCEINLINE bool recv_msg(netlink<_send, _recv> & link, _msg * msg)
	{
		return link.recv_msg(msg);
	}
private:
	_container m_container;
};

