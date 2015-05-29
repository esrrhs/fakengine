#include "fakengine.h"
#include "common.h"

char g_msg_buffer[MAX_MSG_SIZE];
Fproto::ProxyMsg g_netmsg;

bool proxy_processor::on_recv_msg(mysocket & s, const mymsg & msg)
{
	msg.read_buffer((int8_t*)g_msg_buffer, msg.size());
	int32_t recvmsgsize = g_netmsg.Unmarshal(g_msg_buffer, msg.size());
	LOG_DEBUG("recv cmd %d from %u size %d\n", 
		g_netmsg.m_ProxyMsgHead.m_CmdMsgPara.m_Type, 
		g_netmsg.m_ProxyMsgHead.m_SrcSvrID,
		recvmsgsize);

	if (recvmsgsize < 0)
	{
		return false;
	}

	switch (g_netmsg.m_ProxyMsgHead.m_CmdMsgPara.m_Type)
	{
	case Fproto::CMD_REGISTR:
		HandleRegister(s, g_netmsg);
		break;
	case Fproto::CMD_TRANS_BY_KEY_HASH:
		HandleTransByKey(s, g_netmsg);
		break;
	case Fproto::CMD_TRANS_BY_ID:
		HandleTransByID(s, g_netmsg);
		break;
	case Fproto::CMD_TRANS_BROADCAST:
		HandleTransBroadcast(s, g_netmsg);
		break;
	default:
		break;
	}

	return true;
}

bool proxy_processor::HandleRegister(mysocket & s, const Fproto::ProxyMsg & proxymsg)
{
	const Fproto::CmdRegister & para = proxymsg.m_ProxyMsgHead.m_CmdMsgPara.m_CmdRegister;

	LOG_DEBUG("HandleRegister ID %u Type %u", para.m_ID, para.m_Type);

	return true;
}

bool proxy_processor::HandleTransByKey(mysocket & s, const Fproto::ProxyMsg & proxymsg)
{
	const Fproto::CmdTransByKeyHash & para = proxymsg.m_ProxyMsgHead.m_CmdMsgPara.m_CmdTransByKeyHash;

	LOG_DEBUG("HandleTransByKey Hash %u Key %u Type %u", para.m_Hash, para.m_Key, para.m_Type);

	return true;
}

bool proxy_processor::HandleTransByID(mysocket & s, const Fproto::ProxyMsg & proxymsg)
{
	const Fproto::CmdTransByID & para = proxymsg.m_ProxyMsgHead.m_CmdMsgPara.m_CmdTransByID;

	LOG_DEBUG("HandleTransByID DestSvrID %u", para.m_DestSvrID);

	return true;
}

bool proxy_processor::HandleTransBroadcast(mysocket & s, const Fproto::ProxyMsg & proxymsg)
{
	const Fproto::CmdTransBroadcast & para = proxymsg.m_ProxyMsgHead.m_CmdMsgPara.m_CmdTransBroadcast;

	LOG_DEBUG("HandleTransBroadcast Type %u", para.m_Type);

	return true;
}
