#include "fakengine.h"
#include "fproxyclientapp.h"

bool fproxyclientapp::ini( int argc, char *argv[] )
{
	mycmdparser cp;
	cp.parse(argc, argv);
	int32_t id;
	int32_t type;
	SAFE_TEST_RET_VAL(cp.get("id", id), false ,false);
	SAFE_TEST_RET_VAL(cp.get("type", type), false, false);

	farray<tcp_socket_link_param, PROXY_NUM> param;
	tcp_socket_link_param proxy1;
	proxy1.ip = "127.0.0.1";
	proxy1.port = 11111;
	param.push_back(proxy1);

	m_proxyclient.ini(id, type, param);
	return true;
}

bool fproxyclientapp::heartbeat()
{
	m_proxyclient.tick();

	g_netmsg.m_NetMsgPara.m_Type = Fproto::CS_REQ_LOGIN;
	Fproto::CSReqLogin & para = g_netmsg.m_NetMsgPara.m_CSReqLogin;
	memset(&para, 0, sizeof(para));
	fstrcopy(para.m_Acc, "test");
	fstrcopy(para.m_Pwd, "123");
	m_proxyclient.send_msg_by_hash(2, 1, g_netmsg);

	return true;
}

bool fproxyclientapp::exit()
{
	return true;
}

fproxyclientapp::fproxyclientapp() : mainapp("fproxyclientapp")
{

}
