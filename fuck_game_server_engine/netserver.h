#pragma once

// netlink ���ӿͻ�����
// _msg ��Ϣ
// _accept ��������ʵ��
// _container ��������ʵ�壬����accept��select
// _send ����Ϣʵ�壬�˴����ܾ�ֻ�ǻ���������������_container
// _recv ����Ϣʵ�壬�˴����ܾ�ֻ�ǻ���������������_container
template <typename _msg, typename _container, typename _send, typename _recv>
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
	FORCEINLINE bool send_msg(netlink<_msg, _send, _recv> & link, const _msg * msg)
	{
		return link.send_msg(msg);
	}
	FORCEINLINE bool recv_msg(netlink<_msg, _send, _recv> & link, _msg * msg)
	{
		return link.recv_msg(msg);
	}
private:
	_container m_container;
};

