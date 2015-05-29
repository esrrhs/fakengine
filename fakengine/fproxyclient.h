#pragma once

template <typename _msg, typename _socket, typename _real_select, typename _proxy_processor, size_t N>
class fproxyclient
{
public:
	static const int32_t INVALID_IDX = -1;
	static const size_t BUFFER_SIZE = Fproto::PROXY_MSG_LEN + 1024;
public:
    fproxyclient()
	{
		m_num = 0;
		m_lastlink = 0;
    }
    ~fproxyclient()
    {
    }

	class client_processor
	{
	public:
		force_inline bool on_recv_msg(_socket & s, const _msg & msg)
		{
			msg.read_buffer((int8_t*)m_msg_buffer, msg.size());
			int32_t recvmsgsize = m_proxymsg->Unmarshal((char*)m_msg_buffer, msg.size());
			if (recvmsgsize < 0)
			{
				return false;
			}

			m_proxy_processor.on_recv_msg(m_proxymsg->m_ProxyMsgHead.m_SrcSvrID, 
				m_proxymsg->m_ProxyMsgPara.m_Buff, m_proxymsg->m_ProxyMsgPara.m_Num);

			return true;
		}
		force_inline bool on_connect(_socket & s)
		{
			// зЂВс
			m_proxymsg->Clear();
			m_proxymsg->m_ProxyMsgHead.m_SrcSvrID = m_svrid;
			m_proxymsg->m_ProxyMsgHead.m_CmdMsgPara.m_Type = Fproto::CMD_REGISTR;
			m_proxymsg->m_ProxyMsgHead.m_CmdMsgPara.m_CmdRegister.m_ID = m_svrid;
			m_proxymsg->m_ProxyMsgHead.m_CmdMsgPara.m_CmdRegister.m_Type = m_svrtype;
			m_proxymsg->m_ProxyMsgPara.m_Num = 0;

			int32_t msgsize = m_proxymsg->Marshal((char*)m_msg_buffer, BUFFER_SIZE);
			if (msgsize < 0)
			{
				return false;
			}

			_msg sendm;
			sendm.write_buffer((const int8_t*)m_msg_buffer, msgsize);
			s.send(sendm);
			return true;
		}
		force_inline bool on_close(_socket & s)
		{
			return true;
		}
	public:
		_proxy_processor m_proxy_processor;
		uint32_t m_svrtype;
		uint32_t m_svrid;
		Fproto::ProxyMsg * m_proxymsg;
		int8_t * m_msg_buffer;
	};

    template<typename _param>
	force_inline bool ini(uint32_t svrtype, uint32_t svrid, const farray<_param, N> & param)
	{
		m_svrtype = svrtype;
		m_svrid = svrid;

		client_processor pro;
		pro.m_svrtype = svrtype;
		pro.m_svrid = svrid;
		pro.m_proxymsg = &m_proxymsg;
		pro.m_msg_buffer = m_msg_buffer;

	    bool ret;
		for (int32_t i = 0; i < (int32_t)param.size(); i++)
	    {
	        ret &= m_link[i].ini(param[i]);
			m_link[i].set_event_processor(pro);
	    }

		m_num = param.size();
		m_lastlink = 0;

		return ret;
	}
	
	force_inline void tick()
	{
		for (int32_t i = 0; i < m_num; i++)
	    {
	        m_link[i].tick();
	    }
	}

	template<typename _themsg>
	force_inline bool send_msg_by_hash(uint32_t svrtype, uint32_t key, const _themsg & themsg)
	{
		m_proxymsg.Clear();
		m_proxymsg.m_ProxyMsgHead.m_SrcSvrID = m_svrid;
		m_proxymsg.m_ProxyMsgHead.m_CmdMsgPara.m_Type = Fproto::CMD_TRANS_BY_KEY_HASH;
		m_proxymsg.m_ProxyMsgHead.m_CmdMsgPara.m_CmdTransByKeyHash.m_Hash = 0;
		m_proxymsg.m_ProxyMsgHead.m_CmdMsgPara.m_CmdTransByKeyHash.m_Type = svrtype;
		m_proxymsg.m_ProxyMsgHead.m_CmdMsgPara.m_CmdTransByKeyHash.m_Key = key;
		
		int32_t themsgsize = themsg.Marshal((char*)m_proxymsg.m_ProxyMsgPara.m_Buff, sizeof(m_proxymsg.m_ProxyMsgPara.m_Buff));
		if (themsgsize < 0)
		{
			return false;
		}

		m_proxymsg.m_ProxyMsgPara.m_Num = themsgsize;

		int32_t msgsize = m_proxymsg.Marshal((char*)m_msg_buffer, BUFFER_SIZE);
		if (msgsize < 0)
		{
			return false;
		}

		_msg sendm;
		sendm.write_buffer((const int8_t*)m_msg_buffer, msgsize);
		
		return send_msg(sendm);
	}
	
private:
    force_inline int32_t choose_link()
    {
		for (int32_t i = 0; i < m_num; i++)
	    {
	        if (m_link[i].isconnect())
	        {
	            return i;
	        }
	        m_lastlink++;
	        if (m_lastlink >= (int32_t)N)
	        {
	            m_lastlink = N - 1;
	        }
	    }
        return INVALID_IDX;
	}
	force_inline bool send_msg(const _msg & msg)
	{
		int32_t i = choose_link();
		SAFE_TEST_RET_VAL(i, INVALID_IDX, false);
		m_link[i].send_msg(msg);
		return false;
	}
private:
	typedef socket_link<_msg, _socket, _real_select, client_processor> Link;
    Link m_link[N];
	int32_t m_lastlink;
	int32_t m_num;
	uint32_t m_svrtype;
	uint32_t m_svrid;
	Fproto::ProxyMsg m_proxymsg;
	int8_t m_msg_buffer[BUFFER_SIZE];
};

