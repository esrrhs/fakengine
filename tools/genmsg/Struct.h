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
		SAFE_TEST_RET_VAL(buffer.add("m_Acc", m_Acc, ACC_LEN), false, false);
		 
		 
		// 钻石	
		SAFE_TEST_RET_VAL(buffer.add("m_diamond", m_diamond), false, false);
		 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// 用户名	
		SAFE_TEST_RET_VAL(buffer.get("m_Acc", m_Acc, ACC_LEN), false, false);
		 
		 
		// 钻石	
		SAFE_TEST_RET_VAL(buffer.get("m_diamond", m_diamond), false, false);
		 
		
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
		SAFE_TEST_RET_VAL(buffer.add("m_Num", m_Num), false, false);
		 
		 
		// 唯一id	
		int32_t copyGuidSize = sizeof(uint64_t) * PROTO_MIN(FRIEND_LEN, m_Num);
		SAFE_TEST_RET_VAL(buffer.add("m_Guid", m_Guid, copyGuidSize), false, false);
		 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// 数目	
		SAFE_TEST_RET_VAL(buffer.get("m_Num", m_Num), false, false);
		 
		 
		// 唯一id	
		int32_t copyGuidSize = sizeof(uint64_t) * PROTO_MIN(FRIEND_LEN, m_Num);
		SAFE_TEST_RET_VAL(buffer.get("m_Guid", m_Guid, copyGuidSize), false, false);
		 
		
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
		SAFE_TEST_RET_VAL(buffer.add("m_Guid", m_Guid), false, false);
		 
		 
		// 角色名	
		SAFE_TEST_RET_VAL(buffer.add("m_Name", m_Name, ROLE_NAME_LEN), false, false);
		 
		 
		// 血量	
		SAFE_TEST_RET_VAL(buffer.add("m_Hp", m_Hp), false, false);
		 
		 
		// 魔量	
		SAFE_TEST_RET_VAL(buffer.add("m_Mp", m_Mp), false, false);
		 
		 
		// x	
		SAFE_TEST_RET_VAL(buffer.add("m_X", m_X), false, false);
		 
		 
		// y	
		SAFE_TEST_RET_VAL(buffer.add("m_Y", m_Y), false, false);
		 
		 
		// 好友信息	
		{
			T tmpFriendInfoBuff;
			SAFE_TEST_RET_VAL(m_FriendInfo.Marshal(tmpFriendInfoBuff), false, false);
			SAFE_TEST_RET_VAL(buffer.add("m_FriendInfo", tmpFriendInfoBuff), false, false);
		} 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// 唯一id	
		SAFE_TEST_RET_VAL(buffer.get("m_Guid", m_Guid), false, false);
		 
		 
		// 角色名	
		SAFE_TEST_RET_VAL(buffer.get("m_Name", m_Name, ROLE_NAME_LEN), false, false);
		 
		 
		// 血量	
		SAFE_TEST_RET_VAL(buffer.get("m_Hp", m_Hp), false, false);
		 
		 
		// 魔量	
		SAFE_TEST_RET_VAL(buffer.get("m_Mp", m_Mp), false, false);
		 
		 
		// x	
		SAFE_TEST_RET_VAL(buffer.get("m_X", m_X), false, false);
		 
		 
		// y	
		SAFE_TEST_RET_VAL(buffer.get("m_Y", m_Y), false, false);
		 
		 
		// 好友信息	
		{
			T tmpFriendInfoBuff;
			SAFE_TEST_RET_VAL(buffer.get("m_FriendInfo", tmpFriendInfoBuff), false, false);
			SAFE_TEST_RET_VAL(m_FriendInfo.Unmarshal(tmpFriendInfoBuff), false, false);
		} 
		
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
		SAFE_TEST_RET_VAL(buffer.add("m_Acc", m_Acc, ACC_LEN), false, false);
		 
		 
		// 密码	
		SAFE_TEST_RET_VAL(buffer.add("m_Pwd", m_Pwd, PWD_LEN), false, false);
		 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// 用户名	
		SAFE_TEST_RET_VAL(buffer.get("m_Acc", m_Acc, ACC_LEN), false, false);
		 
		 
		// 密码	
		SAFE_TEST_RET_VAL(buffer.get("m_Pwd", m_Pwd, PWD_LEN), false, false);
		 
		
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
		SAFE_TEST_RET_VAL(buffer.add("m_Ret", m_Ret), false, false);
		 
		 
		// 账号信息	
		{
			T tmpAccInfoBuff;
			SAFE_TEST_RET_VAL(m_AccInfo.Marshal(tmpAccInfoBuff), false, false);
			SAFE_TEST_RET_VAL(buffer.add("m_AccInfo", tmpAccInfoBuff), false, false);
		} 
		 
		// 玩家数目	
		SAFE_TEST_RET_VAL(buffer.add("m_RoleInfoNum", m_RoleInfoNum), false, false);
		 
		 
		// 玩家信息	
		T tmpRoleInfoBuff;
		stringc tmpRoleInfoName;
		for (int32_t i = 0; i < ROLE_INFO_NUM && i < m_RoleInfoNum; i++)
		{
			tmpRoleInfoBuff.reset();
			SAFE_TEST_RET_VAL(m_RoleInfo[i].Marshal(tmpRoleInfoBuff), false, false);
			tmpRoleInfoName = "m_RoleInfo";
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
		SAFE_TEST_RET_VAL(buffer.get("m_Ret", m_Ret), false, false);
		 
		 
		// 账号信息	
		{
			T tmpAccInfoBuff;
			SAFE_TEST_RET_VAL(buffer.get("m_AccInfo", tmpAccInfoBuff), false, false);
			SAFE_TEST_RET_VAL(m_AccInfo.Unmarshal(tmpAccInfoBuff), false, false);
		} 
		 
		// 玩家数目	
		SAFE_TEST_RET_VAL(buffer.get("m_RoleInfoNum", m_RoleInfoNum), false, false);
		 
		 
		// 玩家信息	
		T tmpRoleInfoBuff;
		stringc tmpRoleInfoName;
		for (int32_t i = 0; i < ROLE_INFO_NUM && i < m_RoleInfoNum; i++)
		{
			tmpRoleInfoBuff.reset();
			tmpRoleInfoName = "m_RoleInfo";
			tmpRoleInfoName += i;
			SAFE_TEST_RET_VAL(buffer.get(tmpRoleInfoName, tmpRoleInfoBuff), false, false);
			SAFE_TEST_RET_VAL(m_RoleInfo[i].Unmarshal(tmpRoleInfoBuff), false, false);
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
		SAFE_TEST_RET_VAL(buffer.add("m_Time", m_Time), false, false);
		 
		 
		// 序列号	
		SAFE_TEST_RET_VAL(buffer.add("m_Index", m_Index), false, false);
		 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// 时间戳	
		SAFE_TEST_RET_VAL(buffer.get("m_Time", m_Time), false, false);
		 
		 
		// 序列号	
		SAFE_TEST_RET_VAL(buffer.get("m_Index", m_Index), false, false);
		 
		
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
		
		SAFE_TEST_RET_VAL(buffer.add("m_Type", m_Type), false, false);
		
		switch (m_Type)
		{
		  
		// 请求登录  
		case CS_REQ_LOGIN:
			{
				T tmpCSReqLoginBuff;
				SAFE_TEST_RET_VAL(m_CSReqLogin.Marshal(tmpCSReqLoginBuff), false, false);
				SAFE_TEST_RET_VAL(buffer.add("m_CSReqLogin", tmpCSReqLoginBuff), false, false);
			}
			break;
		  
		// 返回登录  
		case SC_RES_LOGIN:
			{
				T tmpSCResLoginBuff;
				SAFE_TEST_RET_VAL(m_SCResLogin.Marshal(tmpSCResLoginBuff), false, false);
				SAFE_TEST_RET_VAL(buffer.add("m_SCResLogin", tmpSCResLoginBuff), false, false);
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
		SAFE_TEST_RET_VAL(buffer.get("m_Type", m_Type), false, false);
		
		switch (m_Type)
		{
		  
		// 请求登录  
		case CS_REQ_LOGIN:
			{
				T tmpCSReqLoginBuff;
				SAFE_TEST_RET_VAL(buffer.get("m_CSReqLogin", tmpCSReqLoginBuff), false, false);
				SAFE_TEST_RET_VAL(m_CSReqLogin.Unmarshal(tmpCSReqLoginBuff), false, false);
			}
			break;
		  
		// 返回登录  
		case SC_RES_LOGIN:
			{
				T tmpSCResLoginBuff;
				SAFE_TEST_RET_VAL(buffer.get("m_SCResLogin", tmpSCResLoginBuff), false, false);
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
			SAFE_TEST_RET_VAL(buffer.add("m_NetMsgHead", tmpNetMsgHeadBuff), false, false);
		} 
		 
		// 网络消息内容	
		{
			T tmpNetMsgParaBuff;
			SAFE_TEST_RET_VAL(m_NetMsgPara.Marshal(tmpNetMsgParaBuff), false, false);
			SAFE_TEST_RET_VAL(buffer.add("m_NetMsgPara", tmpNetMsgParaBuff), false, false);
		} 
		
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		 
		// 网络头消息	
		{
			T tmpNetMsgHeadBuff;
			SAFE_TEST_RET_VAL(buffer.get("m_NetMsgHead", tmpNetMsgHeadBuff), false, false);
			SAFE_TEST_RET_VAL(m_NetMsgHead.Unmarshal(tmpNetMsgHeadBuff), false, false);
		} 
		 
		// 网络消息内容	
		{
			T tmpNetMsgParaBuff;
			SAFE_TEST_RET_VAL(buffer.get("m_NetMsgPara", tmpNetMsgParaBuff), false, false);
			SAFE_TEST_RET_VAL(m_NetMsgPara.Unmarshal(tmpNetMsgParaBuff), false, false);
		} 
		
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
  

}
