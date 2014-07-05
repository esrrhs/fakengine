#include "fuck_game_server_engine.h"
#include "common.h"

char g_msg_buffer[MAX_MSG_SIZE];
Fproto::ProxyMsg g_netmsg;

bool proxy_processor::on_recv_msg(mysocket & s, const mymsg & msg)
{
	msg.read_buffer((int8_t*)g_msg_buffer, sizeof(g_msg_buffer));
	int32_t recvmsgsize = g_netmsg.Unmarshal(g_msg_buffer, sizeof(g_msg_buffer));
	LOG_DEBUG("recv cmd %d from %u size %d\n", 
		g_netmsg.m_ProxyMsgHead.m_CmdMsgPara.m_Type, 
		g_netmsg.m_ProxyMsgHead.m_SrcSvrID,
		recvmsgsize);

	switch (g_netmsg.m_ProxyMsgHead.m_CmdMsgPara.m_Type)
	{
	case Fproto::CMD_REGISTR:
		HandleRegister(g_netmsg);
		break;
	case Fproto::CMD_TRANS_BY_KEY_HASH:
	case Fproto::CMD_TRANS_BY_ID:
	case Fproto::CMD_TRANS_BROADCAST:
	default:
		break;
	}

	return true;
}

bool proxy_processor::HandleRegister(const Fproto::ProxyMsg & proxymsg)
{
	const Fproto::CmdRegister & para = proxymsg.m_ProxyMsgHead.m_CmdMsgPara.m_CmdRegister;

	LOG_DEBUG("HandleRegister ID %u Type %u", para.m_ID, para.m_Type);

	return true;
}

bool proxy_processor::HandleTransByKey(const Fproto::ProxyMsg & proxymsg)
{
	LOG_DEBUG("HandleTransByKey");

	return true;
}

bool proxy_processor::HandleTransByID(const Fproto::ProxyMsg & proxymsg)
{
	LOG_DEBUG("HandleTransByID");

	return true;
}

bool proxy_processor::HandleTransBroadcast(const Fproto::ProxyMsg & proxymsg)
{
	LOG_DEBUG("HandleTransBroadcast");

	return true;
}
