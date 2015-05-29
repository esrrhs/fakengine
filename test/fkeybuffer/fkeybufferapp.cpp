#include "fakengine.h"
#include "fkeybufferapp.h"
#include "../tools/genmsg/Struct.h"

bool fkeybufferapp::ini( int argc, char *argv[] )
{
	return true;
}

bool fkeybufferapp::heartbeat()
{
    fkeybuffer<10240> testbuff;

    testbuff.clear();
    Fproto::TestW1 w1;
    w1.m_a = 1;
    w1.m_b = 2;
    w1.Marshal(testbuff);
    
    Fproto::TestR1 r1;
    r1.Unmarshal(testbuff);

    testbuff.clear();
    Fproto::TestW2 w2;
    w2.m_Num = 10;
    for (int32_t i = 0; i < (int32_t)w2.m_Num; i++)
    {
        w2.m_test[i] = i;
    }
    w2.Marshal(testbuff);
    
    Fproto::TestR2 r2;
    r2.Unmarshal(testbuff);

    testbuff.clear();
    Fproto::TestW3 w3;
    w3.m_test.m_a = 1;
    w3.m_test.m_b = 10;
    w3.Marshal(testbuff);
    
    Fproto::TestR3 r3;
    r3.Unmarshal(testbuff);
        
    testbuff.clear();
    Fproto::TestW4 w4;
    w4.m_Num = 10;
    for (int32_t i = 0; i < (int32_t)w4.m_Num; i++)
    {
        w4.m_test[i].m_a = i;
        w4.m_test[i].m_b = i + 1;
    }
    w4.Marshal(testbuff);
    
    Fproto::TestR4 r4;
    r4.Unmarshal(testbuff);
    
	//////////////////////////////////////////////////////////////////////////

	fkeybuffer<10240> msgbuff;
	Fproto::NetMsg msg;
	msg.Clear();
	msg.m_NetMsgPara.m_Type = Fproto::SC_RES_LOGIN;
	Fproto::SCResLogin & para = msg.m_NetMsgPara.m_SCResLogin;
	para.m_Ret = 1;
	para.m_RoleInfoNum = 5;
	for (int32_t i = 0; i < (int32_t)para.m_RoleInfoNum; i++)
	{
		para.m_RoleInfo[i].m_Guid = i;
		para.m_RoleInfo[i].m_FriendInfo.m_Num = 50;
	}
	bool ret = msg.Marshal(msgbuff);

	Fproto::NetMsg msg1;
	ret = msg1.Unmarshal(msgbuff);

	//////////////////////////////////////////////////////////////////////////

	int8_t param1 = 123;
	int16_t param2 = 1234;
	int32_t param3 = 123456;
	int64_t param4 = 1234567890;
	float param5 = 1.34f;
	stringc param6 = "aagagag";
	stringc param7;
	fkeybuffer<1024> buff;
	buff.begin("test");
	buff.add("param1", param1);
	buff.add("param2", param2);
	buff.add("param3", param3);
	buff.add("param4", param4);
	buff.add("param5", param5);
	buff.add("param6", param6);
	buff.add("param7", param7);

	fkeybuffer<300> subbuff;
	subbuff.begin("subtest");
	subbuff.add("subparam1", param1);
	subbuff.add("subparam2", param2);
	subbuff.end();

	buff.add("subbuff", subbuff);

	buff.end();

	//////////////////////////////////////////////////////////////////////////
	int8_t tmpparam1;
	int16_t tmpparam2;
	int32_t tmpparam3;
	int64_t tmpparam4;
	float tmpparam5;
	stringc tmpparam6;
	stringc tmpparam7;
	buff.get("param3", tmpparam3);
	buff.get("param6", tmpparam6);
	buff.get("param2", tmpparam2);
	buff.get("param1", tmpparam1);
	buff.get("param4", tmpparam4);
	buff.get("param7", tmpparam7);
	buff.get("param5", tmpparam5);

	fkeybuffer<500> tmpsubbuff;

	buff.get("subbuff", tmpsubbuff);
	int8_t tmpsubparam1;
	int16_t tmpsubparam2;
	tmpsubbuff.get("subparam1", tmpsubparam1);
	tmpsubbuff.get("subparam2", tmpsubparam2);

	return true;
}

bool fkeybufferapp::exit()
{
	return true;
}

fkeybufferapp::fkeybufferapp() : mainapp("fkeybufferapp")
{

}
