#pragma once

#include <stdint.h>

#define PROTO_MIN(a,b) Min<int32_t>((int32_t)a, (int32_t)b)

namespace Fproto
{

    
// 命令id  
enum CmdID
{
	 
	// 注册  
	CMD_REGISTR = 1,
	 
	// 通过keyhash  
	CMD_TRANS_BY_KEY_HASH = 2,
	 
	// 直接通过id  
	CMD_TRANS_BY_ID = 3,
	 
	// 广播  
	CMD_TRANS_BROADCAST = 4,
	  
};
    
// 常量  
enum ConstDefine
{
	 
	// 传输消息体最大尺寸  
	PROXY_MSG_LEN = 1024*1024,
	  
};
  



		
		
	


	
		
    
// 注册  
struct CmdRegister  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	  
	// 类型  
	uint8_t m_Type;  
	  
	// ID  
	uint32_t m_ID;  
	  
	
	template <typename T>
	bool Marshal(T & buffer)
	{			
		buffer.begin("CmdRegister");
			
		 
		// 类型	
		SAFE_TEST_RET_VAL(buffer.add("Type", m_Type), false, false);
		 
		 
		// ID	
		SAFE_TEST_RET_VAL(buffer.add("ID", m_ID), false, false);
		 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// 类型	
		buffer.get("Type", m_Type);
		 
		 
		// ID	
		buffer.get("ID", m_ID);
		 
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 类型	
		if (ret + (int32_t)sizeof(m_Type) > size)
		{
			return -ret;
		}
		*(uint8_t*)destbuffer = m_Type;
		ret += sizeof(m_Type);
		destbuffer += sizeof(m_Type);
		 
		 
		// ID	
		if (ret + (int32_t)sizeof(m_ID) > size)
		{
			return -ret;
		}
		*(uint32_t*)destbuffer = m_ID;
		ret += sizeof(m_ID);
		destbuffer += sizeof(m_ID);
		 
		 
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 类型	
		if (ret + (int32_t)sizeof(m_Type) > size)
		{
			return -ret;
		}
		m_Type = *(uint8_t*)srcbuffer;
		ret += sizeof(m_Type);
		srcbuffer += sizeof(m_Type);
		 
		 
		// ID	
		if (ret + (int32_t)sizeof(m_ID) > size)
		{
			return -ret;
		}
		m_ID = *(uint32_t*)srcbuffer;
		ret += sizeof(m_ID);
		srcbuffer += sizeof(m_ID);
		 
		 
		return ret;
	}
	
};  
    
// 通过keyhash  
struct CmdTransByKeyHash  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	  
	// hash方法  
	uint8_t m_Hash;  
	  
	// Key  
	uint32_t m_Key;  
	  
	
	template <typename T>
	bool Marshal(T & buffer)
	{			
		buffer.begin("CmdTransByKeyHash");
			
		 
		// hash方法	
		SAFE_TEST_RET_VAL(buffer.add("Hash", m_Hash), false, false);
		 
		 
		// Key	
		SAFE_TEST_RET_VAL(buffer.add("Key", m_Key), false, false);
		 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// hash方法	
		buffer.get("Hash", m_Hash);
		 
		 
		// Key	
		buffer.get("Key", m_Key);
		 
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// hash方法	
		if (ret + (int32_t)sizeof(m_Hash) > size)
		{
			return -ret;
		}
		*(uint8_t*)destbuffer = m_Hash;
		ret += sizeof(m_Hash);
		destbuffer += sizeof(m_Hash);
		 
		 
		// Key	
		if (ret + (int32_t)sizeof(m_Key) > size)
		{
			return -ret;
		}
		*(uint32_t*)destbuffer = m_Key;
		ret += sizeof(m_Key);
		destbuffer += sizeof(m_Key);
		 
		 
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// hash方法	
		if (ret + (int32_t)sizeof(m_Hash) > size)
		{
			return -ret;
		}
		m_Hash = *(uint8_t*)srcbuffer;
		ret += sizeof(m_Hash);
		srcbuffer += sizeof(m_Hash);
		 
		 
		// Key	
		if (ret + (int32_t)sizeof(m_Key) > size)
		{
			return -ret;
		}
		m_Key = *(uint32_t*)srcbuffer;
		ret += sizeof(m_Key);
		srcbuffer += sizeof(m_Key);
		 
		 
		return ret;
	}
	
};  
    
// 直接通过id  
struct CmdTransByID  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	  
	// SVRID  
	uint32_t m_DestSvrID;  
	  
	
	template <typename T>
	bool Marshal(T & buffer)
	{			
		buffer.begin("CmdTransByID");
			
		 
		// SVRID	
		SAFE_TEST_RET_VAL(buffer.add("DestSvrID", m_DestSvrID), false, false);
		 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// SVRID	
		buffer.get("DestSvrID", m_DestSvrID);
		 
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// SVRID	
		if (ret + (int32_t)sizeof(m_DestSvrID) > size)
		{
			return -ret;
		}
		*(uint32_t*)destbuffer = m_DestSvrID;
		ret += sizeof(m_DestSvrID);
		destbuffer += sizeof(m_DestSvrID);
		 
		 
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// SVRID	
		if (ret + (int32_t)sizeof(m_DestSvrID) > size)
		{
			return -ret;
		}
		m_DestSvrID = *(uint32_t*)srcbuffer;
		ret += sizeof(m_DestSvrID);
		srcbuffer += sizeof(m_DestSvrID);
		 
		 
		return ret;
	}
	
};  
    
// 广播  
struct CmdTransBroadcast  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	  
	// 类型  
	uint8_t m_Type;  
	  
	
	template <typename T>
	bool Marshal(T & buffer)
	{			
		buffer.begin("CmdTransBroadcast");
			
		 
		// 类型	
		SAFE_TEST_RET_VAL(buffer.add("Type", m_Type), false, false);
		 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// 类型	
		buffer.get("Type", m_Type);
		 
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 类型	
		if (ret + (int32_t)sizeof(m_Type) > size)
		{
			return -ret;
		}
		*(uint8_t*)destbuffer = m_Type;
		ret += sizeof(m_Type);
		destbuffer += sizeof(m_Type);
		 
		 
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 类型	
		if (ret + (int32_t)sizeof(m_Type) > size)
		{
			return -ret;
		}
		m_Type = *(uint8_t*)srcbuffer;
		ret += sizeof(m_Type);
		srcbuffer += sizeof(m_Type);
		 
		 
		return ret;
	}
	
};  
    
// 传输命令内容  
struct CmdMsgPara  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	// Type 
	int32_t m_Type;
	
	union
	{
		  
		// 注册  
		CmdRegister m_CmdRegister;  
		  
		// 通过keyhash  
		CmdTransByKeyHash m_CmdTransByKeyHash;  
		  
		// 直接通过id  
		CmdTransByID m_CmdTransByID;  
		  
		// 广播  
		CmdTransBroadcast m_CmdTransBroadcast;  
		  
	};
	
	template <typename T>
	bool Marshal(T & buffer)
	{		
		buffer.begin("CmdMsgPara");
		
		SAFE_TEST_RET_VAL(buffer.add("Type", m_Type), false, false);
		
		switch (m_Type)
		{
		  
		// 注册  
		case CMD_REGISTR:
			{
				T tmpCmdRegisterBuff;
				SAFE_TEST_RET_VAL(m_CmdRegister.Marshal(tmpCmdRegisterBuff), false, false);
				SAFE_TEST_RET_VAL(buffer.add("CmdRegister", tmpCmdRegisterBuff), false, false);
			}
			break;
		  
		// 通过keyhash  
		case CMD_TRANS_BY_KEY_HASH:
			{
				T tmpCmdTransByKeyHashBuff;
				SAFE_TEST_RET_VAL(m_CmdTransByKeyHash.Marshal(tmpCmdTransByKeyHashBuff), false, false);
				SAFE_TEST_RET_VAL(buffer.add("CmdTransByKeyHash", tmpCmdTransByKeyHashBuff), false, false);
			}
			break;
		  
		// 直接通过id  
		case CMD_TRANS_BY_ID:
			{
				T tmpCmdTransByIDBuff;
				SAFE_TEST_RET_VAL(m_CmdTransByID.Marshal(tmpCmdTransByIDBuff), false, false);
				SAFE_TEST_RET_VAL(buffer.add("CmdTransByID", tmpCmdTransByIDBuff), false, false);
			}
			break;
		  
		// 广播  
		case CMD_TRANS_BROADCAST:
			{
				T tmpCmdTransBroadcastBuff;
				SAFE_TEST_RET_VAL(m_CmdTransBroadcast.Marshal(tmpCmdTransBroadcastBuff), false, false);
				SAFE_TEST_RET_VAL(buffer.add("CmdTransBroadcast", tmpCmdTransBroadcastBuff), false, false);
			}
			break;
		  
		default:
			return false;
		}
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		SAFE_TEST_RET_VAL(buffer.get("Type", m_Type), false, false);
		
		switch (m_Type)
		{
		  
		// 注册  
		case CMD_REGISTR:
			{
				T tmpCmdRegisterBuff;
				SAFE_TEST_RET_VAL(buffer.get("CmdRegister", tmpCmdRegisterBuff), false, false);
				SAFE_TEST_RET_VAL(m_CmdRegister.Unmarshal(tmpCmdRegisterBuff), false, false);
			}
			break;
		  
		// 通过keyhash  
		case CMD_TRANS_BY_KEY_HASH:
			{
				T tmpCmdTransByKeyHashBuff;
				SAFE_TEST_RET_VAL(buffer.get("CmdTransByKeyHash", tmpCmdTransByKeyHashBuff), false, false);
				SAFE_TEST_RET_VAL(m_CmdTransByKeyHash.Unmarshal(tmpCmdTransByKeyHashBuff), false, false);
			}
			break;
		  
		// 直接通过id  
		case CMD_TRANS_BY_ID:
			{
				T tmpCmdTransByIDBuff;
				SAFE_TEST_RET_VAL(buffer.get("CmdTransByID", tmpCmdTransByIDBuff), false, false);
				SAFE_TEST_RET_VAL(m_CmdTransByID.Unmarshal(tmpCmdTransByIDBuff), false, false);
			}
			break;
		  
		// 广播  
		case CMD_TRANS_BROADCAST:
			{
				T tmpCmdTransBroadcastBuff;
				SAFE_TEST_RET_VAL(buffer.get("CmdTransBroadcast", tmpCmdTransBroadcastBuff), false, false);
				SAFE_TEST_RET_VAL(m_CmdTransBroadcast.Unmarshal(tmpCmdTransBroadcastBuff), false, false);
			}
			break;
		  
		default:
			return false;
		}
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		
		// type
		if (ret + (int32_t)sizeof(m_Type) > size)
		{
			return -ret;
		}
		*(int32_t*)destbuffer = m_Type;
		ret += sizeof(m_Type);
		destbuffer += sizeof(m_Type);
		
		switch (m_Type)
		{
		  
		// 注册  
		case CMD_REGISTR:
			{
				int32_t retCmdRegister = m_CmdRegister.Marshal(destbuffer, size - ret);
				if (retCmdRegister <= 0)
				{
					return -ret + retCmdRegister;
				}
				ret += retCmdRegister;
				destbuffer += retCmdRegister;
			}
			break;
		  
		// 通过keyhash  
		case CMD_TRANS_BY_KEY_HASH:
			{
				int32_t retCmdTransByKeyHash = m_CmdTransByKeyHash.Marshal(destbuffer, size - ret);
				if (retCmdTransByKeyHash <= 0)
				{
					return -ret + retCmdTransByKeyHash;
				}
				ret += retCmdTransByKeyHash;
				destbuffer += retCmdTransByKeyHash;
			}
			break;
		  
		// 直接通过id  
		case CMD_TRANS_BY_ID:
			{
				int32_t retCmdTransByID = m_CmdTransByID.Marshal(destbuffer, size - ret);
				if (retCmdTransByID <= 0)
				{
					return -ret + retCmdTransByID;
				}
				ret += retCmdTransByID;
				destbuffer += retCmdTransByID;
			}
			break;
		  
		// 广播  
		case CMD_TRANS_BROADCAST:
			{
				int32_t retCmdTransBroadcast = m_CmdTransBroadcast.Marshal(destbuffer, size - ret);
				if (retCmdTransBroadcast <= 0)
				{
					return -ret + retCmdTransBroadcast;
				}
				ret += retCmdTransBroadcast;
				destbuffer += retCmdTransBroadcast;
			}
			break;
		  
		default:
			return -ret;
		}
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		
		// type
		if (ret + (int32_t)sizeof(m_Type) > size)
		{
			return -ret;
		}
		m_Type = *(int32_t*)srcbuffer;
		ret += sizeof(m_Type);
		srcbuffer += sizeof(m_Type);
		
		switch (m_Type)
		{
		  
		// 注册  
		case CMD_REGISTR:
			{
				int32_t retCmdRegister = m_CmdRegister.Unmarshal(srcbuffer, size - ret);
				if (retCmdRegister <= 0)
				{
					return -ret + retCmdRegister;
				}
				ret += retCmdRegister;
				srcbuffer += retCmdRegister;
			}
			break;
		  
		// 通过keyhash  
		case CMD_TRANS_BY_KEY_HASH:
			{
				int32_t retCmdTransByKeyHash = m_CmdTransByKeyHash.Unmarshal(srcbuffer, size - ret);
				if (retCmdTransByKeyHash <= 0)
				{
					return -ret + retCmdTransByKeyHash;
				}
				ret += retCmdTransByKeyHash;
				srcbuffer += retCmdTransByKeyHash;
			}
			break;
		  
		// 直接通过id  
		case CMD_TRANS_BY_ID:
			{
				int32_t retCmdTransByID = m_CmdTransByID.Unmarshal(srcbuffer, size - ret);
				if (retCmdTransByID <= 0)
				{
					return -ret + retCmdTransByID;
				}
				ret += retCmdTransByID;
				srcbuffer += retCmdTransByID;
			}
			break;
		  
		// 广播  
		case CMD_TRANS_BROADCAST:
			{
				int32_t retCmdTransBroadcast = m_CmdTransBroadcast.Unmarshal(srcbuffer, size - ret);
				if (retCmdTransBroadcast <= 0)
				{
					return -ret + retCmdTransBroadcast;
				}
				ret += retCmdTransBroadcast;
				srcbuffer += retCmdTransBroadcast;
			}
			break;
		  
		default:
			return -ret;
		}
		return ret;
	}
	
	
};  
    
// 网络头消息  
struct ProxyMsgHead  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	  
	// SrcSvrID  
	uint32_t m_SrcSvrID;  
	  
	// 传输命令内容  
	CmdMsgPara m_CmdMsgPara;  
	  
	
	template <typename T>
	bool Marshal(T & buffer)
	{			
		buffer.begin("ProxyMsgHead");
			
		 
		// SrcSvrID	
		SAFE_TEST_RET_VAL(buffer.add("SrcSvrID", m_SrcSvrID), false, false);
		 
		 
		// 传输命令内容	
		{
			T tmpCmdMsgParaBuff;
			SAFE_TEST_RET_VAL(m_CmdMsgPara.Marshal(tmpCmdMsgParaBuff), false, false);
			SAFE_TEST_RET_VAL(buffer.add("CmdMsgPara", tmpCmdMsgParaBuff), false, false);
		} 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// SrcSvrID	
		buffer.get("SrcSvrID", m_SrcSvrID);
		 
		 
		// 传输命令内容	
		
		do
		{
			T tmpCmdMsgParaBuff;
			SAFE_TEST_BREAK(buffer.get("CmdMsgPara", tmpCmdMsgParaBuff), false);
			SAFE_TEST_BREAK(m_CmdMsgPara.Unmarshal(tmpCmdMsgParaBuff), false);
		}
		while(0); 
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// SrcSvrID	
		if (ret + (int32_t)sizeof(m_SrcSvrID) > size)
		{
			return -ret;
		}
		*(uint32_t*)destbuffer = m_SrcSvrID;
		ret += sizeof(m_SrcSvrID);
		destbuffer += sizeof(m_SrcSvrID);
		 
		 
		// 传输命令内容	
		int32_t retCmdMsgPara = m_CmdMsgPara.Marshal(destbuffer, size - ret);
		if (retCmdMsgPara <= 0)
		{
			return -ret + retCmdMsgPara;
		}
		ret += retCmdMsgPara;
		destbuffer += retCmdMsgPara;
		 
		 
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// SrcSvrID	
		if (ret + (int32_t)sizeof(m_SrcSvrID) > size)
		{
			return -ret;
		}
		m_SrcSvrID = *(uint32_t*)srcbuffer;
		ret += sizeof(m_SrcSvrID);
		srcbuffer += sizeof(m_SrcSvrID);
		 
		 
		// 传输命令内容	
		int32_t retCmdMsgPara = m_CmdMsgPara.Unmarshal(srcbuffer, size - ret);
		if (retCmdMsgPara <= 0)
		{
			return -ret + retCmdMsgPara;
		}
		ret += retCmdMsgPara;
		srcbuffer += retCmdMsgPara;
		 
		 
		return ret;
	}
	
};  
    
// 网络消息内容  
struct ProxyMsgPara  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	  
	// 大小  
	uint8_t m_Num;  
	  
	// 消息buffer  
	uint8_t m_Buff[PROXY_MSG_LEN];  
	  
	
	template <typename T>
	bool Marshal(T & buffer)
	{			
		buffer.begin("ProxyMsgPara");
			
		 
		// 大小	
		SAFE_TEST_RET_VAL(buffer.add("Num", m_Num), false, false);
		 
		 
		// 消息buffer	
		int32_t copyBuffSize = sizeof(uint8_t) * PROTO_MIN(PROXY_MSG_LEN, m_Num);
		SAFE_TEST_RET_VAL(buffer.add("Buff", m_Buff, copyBuffSize), false, false);
		 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// 大小	
		buffer.get("Num", m_Num);
		 
		 
		// 消息buffer	
		int32_t copyBuffSize = sizeof(uint8_t) * PROTO_MIN(PROXY_MSG_LEN, m_Num);
		buffer.get("Buff", m_Buff, copyBuffSize);
		 
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 大小	
		if (ret + (int32_t)sizeof(m_Num) > size)
		{
			return -ret;
		}
		*(uint8_t*)destbuffer = m_Num;
		ret += sizeof(m_Num);
		destbuffer += sizeof(m_Num);
		 
		 
		// 消息buffer	
		int32_t copyBuffSize = sizeof(uint8_t) * PROTO_MIN(PROXY_MSG_LEN, m_Num);
		if (ret + copyBuffSize > size)
		{
			return -ret;
		}
		if (copyBuffSize > 0)
		{
			memcpy(destbuffer, m_Buff, copyBuffSize);
		}
		ret += copyBuffSize;
		destbuffer += copyBuffSize;
		 
		 
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 大小	
		if (ret + (int32_t)sizeof(m_Num) > size)
		{
			return -ret;
		}
		m_Num = *(uint8_t*)srcbuffer;
		ret += sizeof(m_Num);
		srcbuffer += sizeof(m_Num);
		 
		 
		// 消息buffer	
		int32_t copyBuffSize = sizeof(uint8_t) * PROTO_MIN(PROXY_MSG_LEN, m_Num);
		if (ret + copyBuffSize > size)
		{
			return -ret;
		}
		if (copyBuffSize > 0)
		{
			memcpy(m_Buff, srcbuffer, copyBuffSize);
		}
		ret += copyBuffSize;
		srcbuffer += copyBuffSize;
		m_Num = PROTO_MIN(m_Num, PROXY_MSG_LEN);
		 
		 
		return ret;
	}
	
};  
    
// 网络消息  
struct ProxyMsg  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	  
	// 网络头消息  
	ProxyMsgHead m_ProxyMsgHead;  
	  
	// 网络消息内容  
	ProxyMsgPara m_ProxyMsgPara;  
	  
	
	template <typename T>
	bool Marshal(T & buffer)
	{			
		buffer.begin("ProxyMsg");
			
		 
		// 网络头消息	
		{
			T tmpProxyMsgHeadBuff;
			SAFE_TEST_RET_VAL(m_ProxyMsgHead.Marshal(tmpProxyMsgHeadBuff), false, false);
			SAFE_TEST_RET_VAL(buffer.add("ProxyMsgHead", tmpProxyMsgHeadBuff), false, false);
		} 
		 
		// 网络消息内容	
		{
			T tmpProxyMsgParaBuff;
			SAFE_TEST_RET_VAL(m_ProxyMsgPara.Marshal(tmpProxyMsgParaBuff), false, false);
			SAFE_TEST_RET_VAL(buffer.add("ProxyMsgPara", tmpProxyMsgParaBuff), false, false);
		} 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// 网络头消息	
		
		do
		{
			T tmpProxyMsgHeadBuff;
			SAFE_TEST_BREAK(buffer.get("ProxyMsgHead", tmpProxyMsgHeadBuff), false);
			SAFE_TEST_BREAK(m_ProxyMsgHead.Unmarshal(tmpProxyMsgHeadBuff), false);
		}
		while(0); 
		 
		// 网络消息内容	
		
		do
		{
			T tmpProxyMsgParaBuff;
			SAFE_TEST_BREAK(buffer.get("ProxyMsgPara", tmpProxyMsgParaBuff), false);
			SAFE_TEST_BREAK(m_ProxyMsgPara.Unmarshal(tmpProxyMsgParaBuff), false);
		}
		while(0); 
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 网络头消息	
		int32_t retProxyMsgHead = m_ProxyMsgHead.Marshal(destbuffer, size - ret);
		if (retProxyMsgHead <= 0)
		{
			return -ret + retProxyMsgHead;
		}
		ret += retProxyMsgHead;
		destbuffer += retProxyMsgHead;
		 
		 
		// 网络消息内容	
		int32_t retProxyMsgPara = m_ProxyMsgPara.Marshal(destbuffer, size - ret);
		if (retProxyMsgPara <= 0)
		{
			return -ret + retProxyMsgPara;
		}
		ret += retProxyMsgPara;
		destbuffer += retProxyMsgPara;
		 
		 
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 网络头消息	
		int32_t retProxyMsgHead = m_ProxyMsgHead.Unmarshal(srcbuffer, size - ret);
		if (retProxyMsgHead <= 0)
		{
			return -ret + retProxyMsgHead;
		}
		ret += retProxyMsgHead;
		srcbuffer += retProxyMsgHead;
		 
		 
		// 网络消息内容	
		int32_t retProxyMsgPara = m_ProxyMsgPara.Unmarshal(srcbuffer, size - ret);
		if (retProxyMsgPara <= 0)
		{
			return -ret + retProxyMsgPara;
		}
		ret += retProxyMsgPara;
		srcbuffer += retProxyMsgPara;
		 
		 
		return ret;
	}
	
};  
  

}
