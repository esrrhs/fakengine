#pragma once

#include <stdint.h>

#define PROTO_MIN(a,b) Min<int32_t>((int32_t)a, (int32_t)b)

namespace Fproto
{

    
// 消息id  
enum MsgID
{
	 
	// 请求登录  
	CS_REQ_LOGIN = 1,
	 
	// 返回登录  
	SC_RES_LOGIN = 2,
	  
};
    
// 常量  
enum ConstDefine
{
	 
	// 用户名  
	ACC_LEN = 32,
	 
	// 角色名  
	ROLE_NAME_LEN = 32,
	 
	// 密码  
	PWD_LEN = 32,
	 
	// 密码  
	ROLE_INFO_NUM = 5,
	 
	// 好友  
	FRIEND_LEN = 50,
	  
};
  



		
		
	


	
		
    
// 账号信息  
struct AccInfo  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	  
	// 用户名  
	char m_Acc[ACC_LEN];  
	  
	// 钻石  
	uint32_t m_diamond;  
	  
	
	template <typename T>
	bool Marshal(T & buffer)
	{			
		buffer.begin("AccInfo");
			
		 
		// 用户名	
		SAFE_TEST_RET_VAL(buffer.add("Acc", m_Acc, ACC_LEN), false, false);
		 
		 
		// 钻石	
		SAFE_TEST_RET_VAL(buffer.add("diamond", m_diamond), false, false);
		 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// 用户名	
		buffer.get("Acc", m_Acc, ACC_LEN);
		 
		 
		// 钻石	
		buffer.get("diamond", m_diamond);
		 
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 用户名	
		if (ret + (int32_t)sizeof(m_Acc) > size)
		{
			return -ret;
		}
		fstrcopy(destbuffer, m_Acc, sizeof(m_Acc));
		ret += sizeof(m_Acc);
		destbuffer += sizeof(m_Acc);
		 
		 
		// 钻石	
		if (ret + (int32_t)sizeof(m_diamond) > size)
		{
			return -ret;
		}
		*(uint32_t*)destbuffer = m_diamond;
		ret += sizeof(m_diamond);
		destbuffer += sizeof(m_diamond);
		 
		 
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 用户名	
		if (ret + (int32_t)sizeof(m_Acc) > size)
		{
			return -ret;
		}
		fstrcopy(m_Acc, srcbuffer);
		ret += sizeof(m_Acc);
		srcbuffer += sizeof(m_Acc);
		 
		 
		// 钻石	
		if (ret + (int32_t)sizeof(m_diamond) > size)
		{
			return -ret;
		}
		m_diamond = *(uint32_t*)srcbuffer;
		ret += sizeof(m_diamond);
		srcbuffer += sizeof(m_diamond);
		 
		 
		return ret;
	}
	
};  
    
// 好友信息  
struct FriendInfo  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	  
	// 数目  
	uint8_t m_Num;  
	  
	// 唯一id  
	uint64_t m_Guid[FRIEND_LEN];  
	  
	
	template <typename T>
	bool Marshal(T & buffer)
	{			
		buffer.begin("FriendInfo");
			
		 
		// 数目	
		SAFE_TEST_RET_VAL(buffer.add("Num", m_Num), false, false);
		 
		 
		// 唯一id	
		int32_t copyGuidSize = sizeof(uint64_t) * PROTO_MIN(FRIEND_LEN, m_Num);
		SAFE_TEST_RET_VAL(buffer.add("Guid", m_Guid, copyGuidSize), false, false);
		 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// 数目	
		buffer.get("Num", m_Num);
		 
		 
		// 唯一id	
		int32_t copyGuidSize = sizeof(uint64_t) * PROTO_MIN(FRIEND_LEN, m_Num);
		buffer.get("Guid", m_Guid, copyGuidSize);
		 
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 数目	
		if (ret + (int32_t)sizeof(m_Num) > size)
		{
			return -ret;
		}
		*(uint8_t*)destbuffer = m_Num;
		ret += sizeof(m_Num);
		destbuffer += sizeof(m_Num);
		 
		 
		// 唯一id	
		int32_t copyGuidSize = sizeof(uint64_t) * PROTO_MIN(FRIEND_LEN, m_Num);
		if (ret + copyGuidSize > size)
		{
			return -ret;
		}
		if (copyGuidSize > 0)
		{
			memcpy(destbuffer, m_Guid, copyGuidSize);
		}
		ret += copyGuidSize;
		destbuffer += copyGuidSize;
		 
		 
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 数目	
		if (ret + (int32_t)sizeof(m_Num) > size)
		{
			return -ret;
		}
		m_Num = *(uint8_t*)srcbuffer;
		ret += sizeof(m_Num);
		srcbuffer += sizeof(m_Num);
		 
		 
		// 唯一id	
		int32_t copyGuidSize = sizeof(uint64_t) * PROTO_MIN(FRIEND_LEN, m_Num);
		if (ret + copyGuidSize > size)
		{
			return -ret;
		}
		if (copyGuidSize > 0)
		{
			memcpy(m_Guid, srcbuffer, copyGuidSize);
		}
		ret += copyGuidSize;
		srcbuffer += copyGuidSize;
		m_Num = PROTO_MIN(m_Num, FRIEND_LEN);
		 
		 
		return ret;
	}
	
};  
    
// 玩家信息  
struct RoleInfo  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	  
	// 唯一id  
	uint64_t m_Guid;  
	  
	// 角色名  
	char m_Name[ROLE_NAME_LEN];  
	  
	// 血量  
	uint32_t m_Hp;  
	  
	// 魔量  
	uint32_t m_Mp;  
	  
	// x  
	float m_X;  
	  
	// y  
	float m_Y;  
	  
	// 好友信息  
	FriendInfo m_FriendInfo;  
	  
	
	template <typename T>
	bool Marshal(T & buffer)
	{			
		buffer.begin("RoleInfo");
			
		 
		// 唯一id	
		SAFE_TEST_RET_VAL(buffer.add("Guid", m_Guid), false, false);
		 
		 
		// 角色名	
		SAFE_TEST_RET_VAL(buffer.add("Name", m_Name, ROLE_NAME_LEN), false, false);
		 
		 
		// 血量	
		SAFE_TEST_RET_VAL(buffer.add("Hp", m_Hp), false, false);
		 
		 
		// 魔量	
		SAFE_TEST_RET_VAL(buffer.add("Mp", m_Mp), false, false);
		 
		 
		// x	
		SAFE_TEST_RET_VAL(buffer.add("X", m_X), false, false);
		 
		 
		// y	
		SAFE_TEST_RET_VAL(buffer.add("Y", m_Y), false, false);
		 
		 
		// 好友信息	
		{
			T tmpFriendInfoBuff;
			SAFE_TEST_RET_VAL(m_FriendInfo.Marshal(tmpFriendInfoBuff), false, false);
			SAFE_TEST_RET_VAL(buffer.add("FriendInfo", tmpFriendInfoBuff), false, false);
		} 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// 唯一id	
		buffer.get("Guid", m_Guid);
		 
		 
		// 角色名	
		buffer.get("Name", m_Name, ROLE_NAME_LEN);
		 
		 
		// 血量	
		buffer.get("Hp", m_Hp);
		 
		 
		// 魔量	
		buffer.get("Mp", m_Mp);
		 
		 
		// x	
		buffer.get("X", m_X);
		 
		 
		// y	
		buffer.get("Y", m_Y);
		 
		 
		// 好友信息	
		
		do
		{
			T tmpFriendInfoBuff;
			SAFE_TEST_BREAK(buffer.get("FriendInfo", tmpFriendInfoBuff), false);
			SAFE_TEST_BREAK(m_FriendInfo.Unmarshal(tmpFriendInfoBuff), false);
		}
		while(0); 
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 唯一id	
		if (ret + (int32_t)sizeof(m_Guid) > size)
		{
			return -ret;
		}
		*(uint64_t*)destbuffer = m_Guid;
		ret += sizeof(m_Guid);
		destbuffer += sizeof(m_Guid);
		 
		 
		// 角色名	
		if (ret + (int32_t)sizeof(m_Name) > size)
		{
			return -ret;
		}
		fstrcopy(destbuffer, m_Name, sizeof(m_Name));
		ret += sizeof(m_Name);
		destbuffer += sizeof(m_Name);
		 
		 
		// 血量	
		if (ret + (int32_t)sizeof(m_Hp) > size)
		{
			return -ret;
		}
		*(uint32_t*)destbuffer = m_Hp;
		ret += sizeof(m_Hp);
		destbuffer += sizeof(m_Hp);
		 
		 
		// 魔量	
		if (ret + (int32_t)sizeof(m_Mp) > size)
		{
			return -ret;
		}
		*(uint32_t*)destbuffer = m_Mp;
		ret += sizeof(m_Mp);
		destbuffer += sizeof(m_Mp);
		 
		 
		// x	
		if (ret + (int32_t)sizeof(m_X) > size)
		{
			return -ret;
		}
		*(float*)destbuffer = m_X;
		ret += sizeof(m_X);
		destbuffer += sizeof(m_X);
		 
		 
		// y	
		if (ret + (int32_t)sizeof(m_Y) > size)
		{
			return -ret;
		}
		*(float*)destbuffer = m_Y;
		ret += sizeof(m_Y);
		destbuffer += sizeof(m_Y);
		 
		 
		// 好友信息	
		int32_t retFriendInfo = m_FriendInfo.Marshal(destbuffer, size - ret);
		if (retFriendInfo <= 0)
		{
			return -ret + retFriendInfo;
		}
		ret += retFriendInfo;
		destbuffer += retFriendInfo;
		 
		 
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 唯一id	
		if (ret + (int32_t)sizeof(m_Guid) > size)
		{
			return -ret;
		}
		m_Guid = *(uint64_t*)srcbuffer;
		ret += sizeof(m_Guid);
		srcbuffer += sizeof(m_Guid);
		 
		 
		// 角色名	
		if (ret + (int32_t)sizeof(m_Name) > size)
		{
			return -ret;
		}
		fstrcopy(m_Name, srcbuffer);
		ret += sizeof(m_Name);
		srcbuffer += sizeof(m_Name);
		 
		 
		// 血量	
		if (ret + (int32_t)sizeof(m_Hp) > size)
		{
			return -ret;
		}
		m_Hp = *(uint32_t*)srcbuffer;
		ret += sizeof(m_Hp);
		srcbuffer += sizeof(m_Hp);
		 
		 
		// 魔量	
		if (ret + (int32_t)sizeof(m_Mp) > size)
		{
			return -ret;
		}
		m_Mp = *(uint32_t*)srcbuffer;
		ret += sizeof(m_Mp);
		srcbuffer += sizeof(m_Mp);
		 
		 
		// x	
		if (ret + (int32_t)sizeof(m_X) > size)
		{
			return -ret;
		}
		m_X = *(float*)srcbuffer;
		ret += sizeof(m_X);
		srcbuffer += sizeof(m_X);
		 
		 
		// y	
		if (ret + (int32_t)sizeof(m_Y) > size)
		{
			return -ret;
		}
		m_Y = *(float*)srcbuffer;
		ret += sizeof(m_Y);
		srcbuffer += sizeof(m_Y);
		 
		 
		// 好友信息	
		int32_t retFriendInfo = m_FriendInfo.Unmarshal(srcbuffer, size - ret);
		if (retFriendInfo <= 0)
		{
			return -ret + retFriendInfo;
		}
		ret += retFriendInfo;
		srcbuffer += retFriendInfo;
		 
		 
		return ret;
	}
	
};  
    
// 请求登录  
struct CSReqLogin  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	  
	// 用户名  
	char m_Acc[ACC_LEN];  
	  
	// 密码  
	char m_Pwd[PWD_LEN];  
	  
	
	template <typename T>
	bool Marshal(T & buffer)
	{			
		buffer.begin("CSReqLogin");
			
		 
		// 用户名	
		SAFE_TEST_RET_VAL(buffer.add("Acc", m_Acc, ACC_LEN), false, false);
		 
		 
		// 密码	
		SAFE_TEST_RET_VAL(buffer.add("Pwd", m_Pwd, PWD_LEN), false, false);
		 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// 用户名	
		buffer.get("Acc", m_Acc, ACC_LEN);
		 
		 
		// 密码	
		buffer.get("Pwd", m_Pwd, PWD_LEN);
		 
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 用户名	
		if (ret + (int32_t)sizeof(m_Acc) > size)
		{
			return -ret;
		}
		fstrcopy(destbuffer, m_Acc, sizeof(m_Acc));
		ret += sizeof(m_Acc);
		destbuffer += sizeof(m_Acc);
		 
		 
		// 密码	
		if (ret + (int32_t)sizeof(m_Pwd) > size)
		{
			return -ret;
		}
		fstrcopy(destbuffer, m_Pwd, sizeof(m_Pwd));
		ret += sizeof(m_Pwd);
		destbuffer += sizeof(m_Pwd);
		 
		 
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 用户名	
		if (ret + (int32_t)sizeof(m_Acc) > size)
		{
			return -ret;
		}
		fstrcopy(m_Acc, srcbuffer);
		ret += sizeof(m_Acc);
		srcbuffer += sizeof(m_Acc);
		 
		 
		// 密码	
		if (ret + (int32_t)sizeof(m_Pwd) > size)
		{
			return -ret;
		}
		fstrcopy(m_Pwd, srcbuffer);
		ret += sizeof(m_Pwd);
		srcbuffer += sizeof(m_Pwd);
		 
		 
		return ret;
	}
	
};  
    
// 返回登录  
struct SCResLogin  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	  
	// 结果  
	uint8_t m_Ret;  
	  
	// 账号信息  
	AccInfo m_AccInfo;  
	  
	// 玩家数目  
	uint8_t m_RoleInfoNum;  
	  
	// 玩家信息  
	RoleInfo m_RoleInfo[ROLE_INFO_NUM];  
	  
	
	template <typename T>
	bool Marshal(T & buffer)
	{			
		buffer.begin("SCResLogin");
			
		 
		// 结果	
		SAFE_TEST_RET_VAL(buffer.add("Ret", m_Ret), false, false);
		 
		 
		// 账号信息	
		{
			T tmpAccInfoBuff;
			SAFE_TEST_RET_VAL(m_AccInfo.Marshal(tmpAccInfoBuff), false, false);
			SAFE_TEST_RET_VAL(buffer.add("AccInfo", tmpAccInfoBuff), false, false);
		} 
		 
		// 玩家数目	
		SAFE_TEST_RET_VAL(buffer.add("RoleInfoNum", m_RoleInfoNum), false, false);
		 
		 
		// 玩家信息	
		T tmpRoleInfoBuff;
		stringc tmpRoleInfoName;
		for (int32_t i = 0; i < ROLE_INFO_NUM && i < m_RoleInfoNum; i++)
		{
			tmpRoleInfoBuff.reset();
			SAFE_TEST_RET_VAL(m_RoleInfo[i].Marshal(tmpRoleInfoBuff), false, false);
			tmpRoleInfoName = "RoleInfo";
			tmpRoleInfoName += i;
			SAFE_TEST_RET_VAL(buffer.add(tmpRoleInfoName, tmpRoleInfoBuff), false, false);
		} 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// 结果	
		buffer.get("Ret", m_Ret);
		 
		 
		// 账号信息	
		
		do
		{
			T tmpAccInfoBuff;
			SAFE_TEST_BREAK(buffer.get("AccInfo", tmpAccInfoBuff), false);
			SAFE_TEST_BREAK(m_AccInfo.Unmarshal(tmpAccInfoBuff), false);
		}
		while(0); 
		 
		// 玩家数目	
		buffer.get("RoleInfoNum", m_RoleInfoNum);
		 
		 
		// 玩家信息	
		T tmpRoleInfoBuff;
		stringc tmpRoleInfoName;
		for (int32_t i = 0; i < ROLE_INFO_NUM && i < m_RoleInfoNum; i++)
		{
			tmpRoleInfoBuff.reset();
			tmpRoleInfoName = "RoleInfo";
			tmpRoleInfoName += i;
			SAFE_TEST_CONTINUE(buffer.get(tmpRoleInfoName, tmpRoleInfoBuff), false);
			SAFE_TEST_CONTINUE(m_RoleInfo[i].Unmarshal(tmpRoleInfoBuff), false);
		} 
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 结果	
		if (ret + (int32_t)sizeof(m_Ret) > size)
		{
			return -ret;
		}
		*(uint8_t*)destbuffer = m_Ret;
		ret += sizeof(m_Ret);
		destbuffer += sizeof(m_Ret);
		 
		 
		// 账号信息	
		int32_t retAccInfo = m_AccInfo.Marshal(destbuffer, size - ret);
		if (retAccInfo <= 0)
		{
			return -ret + retAccInfo;
		}
		ret += retAccInfo;
		destbuffer += retAccInfo;
		 
		 
		// 玩家数目	
		if (ret + (int32_t)sizeof(m_RoleInfoNum) > size)
		{
			return -ret;
		}
		*(uint8_t*)destbuffer = m_RoleInfoNum;
		ret += sizeof(m_RoleInfoNum);
		destbuffer += sizeof(m_RoleInfoNum);
		 
		 
		// 玩家信息	
		for (int32_t i = 0; i < ROLE_INFO_NUM && i < m_RoleInfoNum; i++)
		{
			int32_t retRoleInfo = m_RoleInfo[i].Marshal(destbuffer, size - ret);
			if (retRoleInfo <= 0)
			{
				return -ret + retRoleInfo;
			}
			ret += retRoleInfo;
			destbuffer += retRoleInfo;
		}
		 
		 
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 结果	
		if (ret + (int32_t)sizeof(m_Ret) > size)
		{
			return -ret;
		}
		m_Ret = *(uint8_t*)srcbuffer;
		ret += sizeof(m_Ret);
		srcbuffer += sizeof(m_Ret);
		 
		 
		// 账号信息	
		int32_t retAccInfo = m_AccInfo.Unmarshal(srcbuffer, size - ret);
		if (retAccInfo <= 0)
		{
			return -ret + retAccInfo;
		}
		ret += retAccInfo;
		srcbuffer += retAccInfo;
		 
		 
		// 玩家数目	
		if (ret + (int32_t)sizeof(m_RoleInfoNum) > size)
		{
			return -ret;
		}
		m_RoleInfoNum = *(uint8_t*)srcbuffer;
		ret += sizeof(m_RoleInfoNum);
		srcbuffer += sizeof(m_RoleInfoNum);
		 
		 
		// 玩家信息	
		for (int32_t i = 0; i < ROLE_INFO_NUM && i < m_RoleInfoNum; i++)
		{
			int32_t retRoleInfo = m_RoleInfo[i].Unmarshal(srcbuffer, size - ret);
			if (retRoleInfo <= 0)
			{
				return -ret + retRoleInfo;
			}
			ret += retRoleInfo;
			srcbuffer += retRoleInfo;
		}
		m_RoleInfoNum = PROTO_MIN(m_RoleInfoNum, ROLE_INFO_NUM);
		 
		 
		return ret;
	}
	
};  
    
// 网络头消息  
struct NetMsgHead  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	  
	// 时间戳  
	uint32_t m_Time;  
	  
	// 序列号  
	uint32_t m_Index;  
	  
	
	template <typename T>
	bool Marshal(T & buffer)
	{			
		buffer.begin("NetMsgHead");
			
		 
		// 时间戳	
		SAFE_TEST_RET_VAL(buffer.add("Time", m_Time), false, false);
		 
		 
		// 序列号	
		SAFE_TEST_RET_VAL(buffer.add("Index", m_Index), false, false);
		 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// 时间戳	
		buffer.get("Time", m_Time);
		 
		 
		// 序列号	
		buffer.get("Index", m_Index);
		 
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 时间戳	
		if (ret + (int32_t)sizeof(m_Time) > size)
		{
			return -ret;
		}
		*(uint32_t*)destbuffer = m_Time;
		ret += sizeof(m_Time);
		destbuffer += sizeof(m_Time);
		 
		 
		// 序列号	
		if (ret + (int32_t)sizeof(m_Index) > size)
		{
			return -ret;
		}
		*(uint32_t*)destbuffer = m_Index;
		ret += sizeof(m_Index);
		destbuffer += sizeof(m_Index);
		 
		 
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 时间戳	
		if (ret + (int32_t)sizeof(m_Time) > size)
		{
			return -ret;
		}
		m_Time = *(uint32_t*)srcbuffer;
		ret += sizeof(m_Time);
		srcbuffer += sizeof(m_Time);
		 
		 
		// 序列号	
		if (ret + (int32_t)sizeof(m_Index) > size)
		{
			return -ret;
		}
		m_Index = *(uint32_t*)srcbuffer;
		ret += sizeof(m_Index);
		srcbuffer += sizeof(m_Index);
		 
		 
		return ret;
	}
	
};  
    
// 网络消息内容  
struct NetMsgPara  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	// Type 
	int32_t m_Type;
	
	union
	{
		  
		// 请求登录  
		CSReqLogin m_CSReqLogin;  
		  
		// 返回登录  
		SCResLogin m_SCResLogin;  
		  
	};
	
	template <typename T>
	bool Marshal(T & buffer)
	{		
		buffer.begin("NetMsgPara");
		
		SAFE_TEST_RET_VAL(buffer.add("Type", m_Type), false, false);
		
		switch (m_Type)
		{
		  
		// 请求登录  
		case CS_REQ_LOGIN:
			{
				T tmpCSReqLoginBuff;
				SAFE_TEST_RET_VAL(m_CSReqLogin.Marshal(tmpCSReqLoginBuff), false, false);
				SAFE_TEST_RET_VAL(buffer.add("CSReqLogin", tmpCSReqLoginBuff), false, false);
			}
			break;
		  
		// 返回登录  
		case SC_RES_LOGIN:
			{
				T tmpSCResLoginBuff;
				SAFE_TEST_RET_VAL(m_SCResLogin.Marshal(tmpSCResLoginBuff), false, false);
				SAFE_TEST_RET_VAL(buffer.add("SCResLogin", tmpSCResLoginBuff), false, false);
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
		  
		// 请求登录  
		case CS_REQ_LOGIN:
			{
				T tmpCSReqLoginBuff;
				SAFE_TEST_RET_VAL(buffer.get("CSReqLogin", tmpCSReqLoginBuff), false, false);
				SAFE_TEST_RET_VAL(m_CSReqLogin.Unmarshal(tmpCSReqLoginBuff), false, false);
			}
			break;
		  
		// 返回登录  
		case SC_RES_LOGIN:
			{
				T tmpSCResLoginBuff;
				SAFE_TEST_RET_VAL(buffer.get("SCResLogin", tmpSCResLoginBuff), false, false);
				SAFE_TEST_RET_VAL(m_SCResLogin.Unmarshal(tmpSCResLoginBuff), false, false);
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
		  
		// 请求登录  
		case CS_REQ_LOGIN:
			{
				int32_t retCSReqLogin = m_CSReqLogin.Marshal(destbuffer, size - ret);
				if (retCSReqLogin <= 0)
				{
					return -ret + retCSReqLogin;
				}
				ret += retCSReqLogin;
				destbuffer += retCSReqLogin;
			}
			break;
		  
		// 返回登录  
		case SC_RES_LOGIN:
			{
				int32_t retSCResLogin = m_SCResLogin.Marshal(destbuffer, size - ret);
				if (retSCResLogin <= 0)
				{
					return -ret + retSCResLogin;
				}
				ret += retSCResLogin;
				destbuffer += retSCResLogin;
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
		  
		// 请求登录  
		case CS_REQ_LOGIN:
			{
				int32_t retCSReqLogin = m_CSReqLogin.Unmarshal(srcbuffer, size - ret);
				if (retCSReqLogin <= 0)
				{
					return -ret + retCSReqLogin;
				}
				ret += retCSReqLogin;
				srcbuffer += retCSReqLogin;
			}
			break;
		  
		// 返回登录  
		case SC_RES_LOGIN:
			{
				int32_t retSCResLogin = m_SCResLogin.Unmarshal(srcbuffer, size - ret);
				if (retSCResLogin <= 0)
				{
					return -ret + retSCResLogin;
				}
				ret += retSCResLogin;
				srcbuffer += retSCResLogin;
			}
			break;
		  
		default:
			return -ret;
		}
		return ret;
	}
	
	
};  
    
// 网络消息  
struct NetMsg  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	  
	// 网络头消息  
	NetMsgHead m_NetMsgHead;  
	  
	// 网络消息内容  
	NetMsgPara m_NetMsgPara;  
	  
	
	template <typename T>
	bool Marshal(T & buffer)
	{			
		buffer.begin("NetMsg");
			
		 
		// 网络头消息	
		{
			T tmpNetMsgHeadBuff;
			SAFE_TEST_RET_VAL(m_NetMsgHead.Marshal(tmpNetMsgHeadBuff), false, false);
			SAFE_TEST_RET_VAL(buffer.add("NetMsgHead", tmpNetMsgHeadBuff), false, false);
		} 
		 
		// 网络消息内容	
		{
			T tmpNetMsgParaBuff;
			SAFE_TEST_RET_VAL(m_NetMsgPara.Marshal(tmpNetMsgParaBuff), false, false);
			SAFE_TEST_RET_VAL(buffer.add("NetMsgPara", tmpNetMsgParaBuff), false, false);
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
			T tmpNetMsgHeadBuff;
			SAFE_TEST_BREAK(buffer.get("NetMsgHead", tmpNetMsgHeadBuff), false);
			SAFE_TEST_BREAK(m_NetMsgHead.Unmarshal(tmpNetMsgHeadBuff), false);
		}
		while(0); 
		 
		// 网络消息内容	
		
		do
		{
			T tmpNetMsgParaBuff;
			SAFE_TEST_BREAK(buffer.get("NetMsgPara", tmpNetMsgParaBuff), false);
			SAFE_TEST_BREAK(m_NetMsgPara.Unmarshal(tmpNetMsgParaBuff), false);
		}
		while(0); 
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 网络头消息	
		int32_t retNetMsgHead = m_NetMsgHead.Marshal(destbuffer, size - ret);
		if (retNetMsgHead <= 0)
		{
			return -ret + retNetMsgHead;
		}
		ret += retNetMsgHead;
		destbuffer += retNetMsgHead;
		 
		 
		// 网络消息内容	
		int32_t retNetMsgPara = m_NetMsgPara.Marshal(destbuffer, size - ret);
		if (retNetMsgPara <= 0)
		{
			return -ret + retNetMsgPara;
		}
		ret += retNetMsgPara;
		destbuffer += retNetMsgPara;
		 
		 
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 网络头消息	
		int32_t retNetMsgHead = m_NetMsgHead.Unmarshal(srcbuffer, size - ret);
		if (retNetMsgHead <= 0)
		{
			return -ret + retNetMsgHead;
		}
		ret += retNetMsgHead;
		srcbuffer += retNetMsgHead;
		 
		 
		// 网络消息内容	
		int32_t retNetMsgPara = m_NetMsgPara.Unmarshal(srcbuffer, size - ret);
		if (retNetMsgPara <= 0)
		{
			return -ret + retNetMsgPara;
		}
		ret += retNetMsgPara;
		srcbuffer += retNetMsgPara;
		 
		 
		return ret;
	}
	
};  
    
// test  
struct TestW1  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	  
	// a  
	uint32_t m_a;  
	  
	// b  
	uint32_t m_b;  
	  
	
	template <typename T>
	bool Marshal(T & buffer)
	{			
		buffer.begin("TestW1");
			
		 
		// a	
		SAFE_TEST_RET_VAL(buffer.add("a", m_a), false, false);
		 
		 
		// b	
		SAFE_TEST_RET_VAL(buffer.add("b", m_b), false, false);
		 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// a	
		buffer.get("a", m_a);
		 
		 
		// b	
		buffer.get("b", m_b);
		 
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// a	
		if (ret + (int32_t)sizeof(m_a) > size)
		{
			return -ret;
		}
		*(uint32_t*)destbuffer = m_a;
		ret += sizeof(m_a);
		destbuffer += sizeof(m_a);
		 
		 
		// b	
		if (ret + (int32_t)sizeof(m_b) > size)
		{
			return -ret;
		}
		*(uint32_t*)destbuffer = m_b;
		ret += sizeof(m_b);
		destbuffer += sizeof(m_b);
		 
		 
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// a	
		if (ret + (int32_t)sizeof(m_a) > size)
		{
			return -ret;
		}
		m_a = *(uint32_t*)srcbuffer;
		ret += sizeof(m_a);
		srcbuffer += sizeof(m_a);
		 
		 
		// b	
		if (ret + (int32_t)sizeof(m_b) > size)
		{
			return -ret;
		}
		m_b = *(uint32_t*)srcbuffer;
		ret += sizeof(m_b);
		srcbuffer += sizeof(m_b);
		 
		 
		return ret;
	}
	
};  
    
// test  
struct TestR1  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	  
	// a  
	uint32_t m_a;  
	  
	// b  
	uint32_t m_b;  
	  
	// c  
	uint32_t m_c;  
	  
	
	template <typename T>
	bool Marshal(T & buffer)
	{			
		buffer.begin("TestR1");
			
		 
		// a	
		SAFE_TEST_RET_VAL(buffer.add("a", m_a), false, false);
		 
		 
		// b	
		SAFE_TEST_RET_VAL(buffer.add("b", m_b), false, false);
		 
		 
		// c	
		SAFE_TEST_RET_VAL(buffer.add("c", m_c), false, false);
		 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// a	
		buffer.get("a", m_a);
		 
		 
		// b	
		buffer.get("b", m_b);
		 
		 
		// c	
		buffer.get("c", m_c);
		 
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// a	
		if (ret + (int32_t)sizeof(m_a) > size)
		{
			return -ret;
		}
		*(uint32_t*)destbuffer = m_a;
		ret += sizeof(m_a);
		destbuffer += sizeof(m_a);
		 
		 
		// b	
		if (ret + (int32_t)sizeof(m_b) > size)
		{
			return -ret;
		}
		*(uint32_t*)destbuffer = m_b;
		ret += sizeof(m_b);
		destbuffer += sizeof(m_b);
		 
		 
		// c	
		if (ret + (int32_t)sizeof(m_c) > size)
		{
			return -ret;
		}
		*(uint32_t*)destbuffer = m_c;
		ret += sizeof(m_c);
		destbuffer += sizeof(m_c);
		 
		 
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// a	
		if (ret + (int32_t)sizeof(m_a) > size)
		{
			return -ret;
		}
		m_a = *(uint32_t*)srcbuffer;
		ret += sizeof(m_a);
		srcbuffer += sizeof(m_a);
		 
		 
		// b	
		if (ret + (int32_t)sizeof(m_b) > size)
		{
			return -ret;
		}
		m_b = *(uint32_t*)srcbuffer;
		ret += sizeof(m_b);
		srcbuffer += sizeof(m_b);
		 
		 
		// c	
		if (ret + (int32_t)sizeof(m_c) > size)
		{
			return -ret;
		}
		m_c = *(uint32_t*)srcbuffer;
		ret += sizeof(m_c);
		srcbuffer += sizeof(m_c);
		 
		 
		return ret;
	}
	
};  
    
// test  
struct TestW2  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	  
	// 数目  
	uint8_t m_Num;  
	  
	// test  
	uint64_t m_test[100];  
	  
	
	template <typename T>
	bool Marshal(T & buffer)
	{			
		buffer.begin("TestW2");
			
		 
		// 数目	
		SAFE_TEST_RET_VAL(buffer.add("Num", m_Num), false, false);
		 
		 
		// test	
		int32_t copytestSize = sizeof(uint64_t) * PROTO_MIN(100, m_Num);
		SAFE_TEST_RET_VAL(buffer.add("test", m_test, copytestSize), false, false);
		 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// 数目	
		buffer.get("Num", m_Num);
		 
		 
		// test	
		int32_t copytestSize = sizeof(uint64_t) * PROTO_MIN(100, m_Num);
		buffer.get("test", m_test, copytestSize);
		 
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 数目	
		if (ret + (int32_t)sizeof(m_Num) > size)
		{
			return -ret;
		}
		*(uint8_t*)destbuffer = m_Num;
		ret += sizeof(m_Num);
		destbuffer += sizeof(m_Num);
		 
		 
		// test	
		int32_t copytestSize = sizeof(uint64_t) * PROTO_MIN(100, m_Num);
		if (ret + copytestSize > size)
		{
			return -ret;
		}
		if (copytestSize > 0)
		{
			memcpy(destbuffer, m_test, copytestSize);
		}
		ret += copytestSize;
		destbuffer += copytestSize;
		 
		 
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 数目	
		if (ret + (int32_t)sizeof(m_Num) > size)
		{
			return -ret;
		}
		m_Num = *(uint8_t*)srcbuffer;
		ret += sizeof(m_Num);
		srcbuffer += sizeof(m_Num);
		 
		 
		// test	
		int32_t copytestSize = sizeof(uint64_t) * PROTO_MIN(100, m_Num);
		if (ret + copytestSize > size)
		{
			return -ret;
		}
		if (copytestSize > 0)
		{
			memcpy(m_test, srcbuffer, copytestSize);
		}
		ret += copytestSize;
		srcbuffer += copytestSize;
		m_Num = PROTO_MIN(m_Num, 100);
		 
		 
		return ret;
	}
	
};  
    
// test  
struct TestR2  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	  
	// 数目  
	uint8_t m_Num;  
	  
	// test  
	uint64_t m_test[200];  
	  
	
	template <typename T>
	bool Marshal(T & buffer)
	{			
		buffer.begin("TestR2");
			
		 
		// 数目	
		SAFE_TEST_RET_VAL(buffer.add("Num", m_Num), false, false);
		 
		 
		// test	
		int32_t copytestSize = sizeof(uint64_t) * PROTO_MIN(200, m_Num);
		SAFE_TEST_RET_VAL(buffer.add("test", m_test, copytestSize), false, false);
		 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// 数目	
		buffer.get("Num", m_Num);
		 
		 
		// test	
		int32_t copytestSize = sizeof(uint64_t) * PROTO_MIN(200, m_Num);
		buffer.get("test", m_test, copytestSize);
		 
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 数目	
		if (ret + (int32_t)sizeof(m_Num) > size)
		{
			return -ret;
		}
		*(uint8_t*)destbuffer = m_Num;
		ret += sizeof(m_Num);
		destbuffer += sizeof(m_Num);
		 
		 
		// test	
		int32_t copytestSize = sizeof(uint64_t) * PROTO_MIN(200, m_Num);
		if (ret + copytestSize > size)
		{
			return -ret;
		}
		if (copytestSize > 0)
		{
			memcpy(destbuffer, m_test, copytestSize);
		}
		ret += copytestSize;
		destbuffer += copytestSize;
		 
		 
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 数目	
		if (ret + (int32_t)sizeof(m_Num) > size)
		{
			return -ret;
		}
		m_Num = *(uint8_t*)srcbuffer;
		ret += sizeof(m_Num);
		srcbuffer += sizeof(m_Num);
		 
		 
		// test	
		int32_t copytestSize = sizeof(uint64_t) * PROTO_MIN(200, m_Num);
		if (ret + copytestSize > size)
		{
			return -ret;
		}
		if (copytestSize > 0)
		{
			memcpy(m_test, srcbuffer, copytestSize);
		}
		ret += copytestSize;
		srcbuffer += copytestSize;
		m_Num = PROTO_MIN(m_Num, 200);
		 
		 
		return ret;
	}
	
};  
    
// test  
struct TestW3  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	  
	// test  
	TestW1 m_test;  
	  
	
	template <typename T>
	bool Marshal(T & buffer)
	{			
		buffer.begin("TestW3");
			
		 
		// test	
		{
			T tmptestBuff;
			SAFE_TEST_RET_VAL(m_test.Marshal(tmptestBuff), false, false);
			SAFE_TEST_RET_VAL(buffer.add("test", tmptestBuff), false, false);
		} 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// test	
		
		do
		{
			T tmptestBuff;
			SAFE_TEST_BREAK(buffer.get("test", tmptestBuff), false);
			SAFE_TEST_BREAK(m_test.Unmarshal(tmptestBuff), false);
		}
		while(0); 
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// test	
		int32_t rettest = m_test.Marshal(destbuffer, size - ret);
		if (rettest <= 0)
		{
			return -ret + rettest;
		}
		ret += rettest;
		destbuffer += rettest;
		 
		 
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// test	
		int32_t rettest = m_test.Unmarshal(srcbuffer, size - ret);
		if (rettest <= 0)
		{
			return -ret + rettest;
		}
		ret += rettest;
		srcbuffer += rettest;
		 
		 
		return ret;
	}
	
};  
    
// test  
struct TestR3  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	  
	// test  
	TestR1 m_test;  
	  
	
	template <typename T>
	bool Marshal(T & buffer)
	{			
		buffer.begin("TestR3");
			
		 
		// test	
		{
			T tmptestBuff;
			SAFE_TEST_RET_VAL(m_test.Marshal(tmptestBuff), false, false);
			SAFE_TEST_RET_VAL(buffer.add("test", tmptestBuff), false, false);
		} 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// test	
		
		do
		{
			T tmptestBuff;
			SAFE_TEST_BREAK(buffer.get("test", tmptestBuff), false);
			SAFE_TEST_BREAK(m_test.Unmarshal(tmptestBuff), false);
		}
		while(0); 
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// test	
		int32_t rettest = m_test.Marshal(destbuffer, size - ret);
		if (rettest <= 0)
		{
			return -ret + rettest;
		}
		ret += rettest;
		destbuffer += rettest;
		 
		 
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// test	
		int32_t rettest = m_test.Unmarshal(srcbuffer, size - ret);
		if (rettest <= 0)
		{
			return -ret + rettest;
		}
		ret += rettest;
		srcbuffer += rettest;
		 
		 
		return ret;
	}
	
};  
    
// test  
struct TestW4  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	  
	// 数目  
	uint8_t m_Num;  
	  
	// test  
	TestW1 m_test[100];  
	  
	
	template <typename T>
	bool Marshal(T & buffer)
	{			
		buffer.begin("TestW4");
			
		 
		// 数目	
		SAFE_TEST_RET_VAL(buffer.add("Num", m_Num), false, false);
		 
		 
		// test	
		T tmptestBuff;
		stringc tmptestName;
		for (int32_t i = 0; i < 100 && i < m_Num; i++)
		{
			tmptestBuff.reset();
			SAFE_TEST_RET_VAL(m_test[i].Marshal(tmptestBuff), false, false);
			tmptestName = "test";
			tmptestName += i;
			SAFE_TEST_RET_VAL(buffer.add(tmptestName, tmptestBuff), false, false);
		} 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// 数目	
		buffer.get("Num", m_Num);
		 
		 
		// test	
		T tmptestBuff;
		stringc tmptestName;
		for (int32_t i = 0; i < 100 && i < m_Num; i++)
		{
			tmptestBuff.reset();
			tmptestName = "test";
			tmptestName += i;
			SAFE_TEST_CONTINUE(buffer.get(tmptestName, tmptestBuff), false);
			SAFE_TEST_CONTINUE(m_test[i].Unmarshal(tmptestBuff), false);
		} 
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 数目	
		if (ret + (int32_t)sizeof(m_Num) > size)
		{
			return -ret;
		}
		*(uint8_t*)destbuffer = m_Num;
		ret += sizeof(m_Num);
		destbuffer += sizeof(m_Num);
		 
		 
		// test	
		for (int32_t i = 0; i < 100 && i < m_Num; i++)
		{
			int32_t rettest = m_test[i].Marshal(destbuffer, size - ret);
			if (rettest <= 0)
			{
				return -ret + rettest;
			}
			ret += rettest;
			destbuffer += rettest;
		}
		 
		 
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 数目	
		if (ret + (int32_t)sizeof(m_Num) > size)
		{
			return -ret;
		}
		m_Num = *(uint8_t*)srcbuffer;
		ret += sizeof(m_Num);
		srcbuffer += sizeof(m_Num);
		 
		 
		// test	
		for (int32_t i = 0; i < 100 && i < m_Num; i++)
		{
			int32_t rettest = m_test[i].Unmarshal(srcbuffer, size - ret);
			if (rettest <= 0)
			{
				return -ret + rettest;
			}
			ret += rettest;
			srcbuffer += rettest;
		}
		m_Num = PROTO_MIN(m_Num, 100);
		 
		 
		return ret;
	}
	
};  
    
// test  
struct TestR4  
{  
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	
	  
	// 数目  
	uint8_t m_Num;  
	  
	// test  
	TestR1 m_test[100];  
	  
	
	template <typename T>
	bool Marshal(T & buffer)
	{			
		buffer.begin("TestR4");
			
		 
		// 数目	
		SAFE_TEST_RET_VAL(buffer.add("Num", m_Num), false, false);
		 
		 
		// test	
		T tmptestBuff;
		stringc tmptestName;
		for (int32_t i = 0; i < 100 && i < m_Num; i++)
		{
			tmptestBuff.reset();
			SAFE_TEST_RET_VAL(m_test[i].Marshal(tmptestBuff), false, false);
			tmptestName = "test";
			tmptestName += i;
			SAFE_TEST_RET_VAL(buffer.add(tmptestName, tmptestBuff), false, false);
		} 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// 数目	
		buffer.get("Num", m_Num);
		 
		 
		// test	
		T tmptestBuff;
		stringc tmptestName;
		for (int32_t i = 0; i < 100 && i < m_Num; i++)
		{
			tmptestBuff.reset();
			tmptestName = "test";
			tmptestName += i;
			SAFE_TEST_CONTINUE(buffer.get(tmptestName, tmptestBuff), false);
			SAFE_TEST_CONTINUE(m_test[i].Unmarshal(tmptestBuff), false);
		} 
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 数目	
		if (ret + (int32_t)sizeof(m_Num) > size)
		{
			return -ret;
		}
		*(uint8_t*)destbuffer = m_Num;
		ret += sizeof(m_Num);
		destbuffer += sizeof(m_Num);
		 
		 
		// test	
		for (int32_t i = 0; i < 100 && i < m_Num; i++)
		{
			int32_t rettest = m_test[i].Marshal(destbuffer, size - ret);
			if (rettest <= 0)
			{
				return -ret + rettest;
			}
			ret += rettest;
			destbuffer += rettest;
		}
		 
		 
		return ret;
	}
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		 
		// 数目	
		if (ret + (int32_t)sizeof(m_Num) > size)
		{
			return -ret;
		}
		m_Num = *(uint8_t*)srcbuffer;
		ret += sizeof(m_Num);
		srcbuffer += sizeof(m_Num);
		 
		 
		// test	
		for (int32_t i = 0; i < 100 && i < m_Num; i++)
		{
			int32_t rettest = m_test[i].Unmarshal(srcbuffer, size - ret);
			if (rettest <= 0)
			{
				return -ret + rettest;
			}
			ret += rettest;
			srcbuffer += rettest;
		}
		m_Num = PROTO_MIN(m_Num, 100);
		 
		 
		return ret;
	}
	
};  
  

}
