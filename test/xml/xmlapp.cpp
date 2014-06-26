#include "fuck_game_server_engine.h"
#include "xmlapp.h"

bool xmlapp::ini( int argc, char *argv[] )
{
	return true;
}

bool xmlapp::heartbeat()
{
    int * pp = new int;
	delete pp;

	pp = new int[10];
	delete[] pp;

    void * p1 = malloc(10);
	p1 = realloc(p1, 123);
    free(p1);
    
    p1 = falloc(2);
    ffree(p1);

    void * p = 0;

    p = frealloc(0, 2);
    frealloc(p, 0);

    p = falloc(22);
    p = frealloc(p, 10);
    p = frealloc(p, 100);
    p = frealloc(p, 100*1024);
    ffree(p);
    
    p = fmemalign(1024, 10240);
    FASSERT(p % 1024 == 0);
	ffree(p);
    
    p = fmemalign(2, 98);
    FASSERT(p % 2 == 0);
	ffree(p);
    
	p = falloc(20);
	size_t size = fmemsize(p);
	FUSE(size);
    FASSERT(size == 20);
	ffree(p);
	
	p = falloc(21);
	ffree(p);
	p = falloc(19);
	ffree(p);

    p = falloc(102349);
    ffree(p);

	for (int i = 1; i < 99*1024;i += 32)
	{
		void * p = falloc(i);
		//memset(p, 1, i);
    	size_t size = fmemsize(p);
    	FUSE(size);
		ffree(p);
	}

	for (int i = 1; i < 99*1024;i += 32)
	{
	    for (int j = 2; j <= 1024 && j < i; j += 32)
	    {
		    void * p = fmemalign(j, i);
    		//memset(p, 1, i);
        	size_t size = fmemsize(p);
        	FUSE(size);
    		ffree(p);
	    }
	}
	
    TiXmlDocument doc("../tools/genxml/sample.xml");		
    bool loadOkay = doc.LoadFile();		
    if (!loadOkay)		
    {
        return false;
    }

    TiXmlElement* pResult = doc.RootElement();
    if (!pResult)
	{
		return false;
	}

	for (TiXmlNode * pEnum = pResult->FirstChild("Enum"); NULL != pEnum; pEnum = pEnum->NextSibling("Enum"))
    {
        FPRINTF("Enum %s", ((TiXmlElement *)pEnum)->Attribute("name"));
    }

	CResultLoader loader;
    bool ret = loader.LoadCfg("../tools/genxml/sample.xml");
    FPRINTF("type %s", (const char *)loader.GetResult().m_vecSTStruct[0].m_vecSTMember[0].m_strtype.c_str());
    FUSE(ret);

	CSampleCfgLoader cfgloader;
	ret = cfgloader.LoadCfg("../tools/gencfg/sample.cfg");

	CGameStatMng::ptr()->AddCPU(1);
	CGameStatMng::ptr()->AddPvPRoomStateNum(1, Rand());
	CGameStatMng::ptr()->AddPvPRoomStateNum(2, Rand());
	CGameStatMng::ptr()->AddPvPRoomStateNum(3, Rand());
	CGameStatMng::ptr()->AddTopSceneNum(1, Rand());
	CGameStatMng::ptr()->AddTopSceneNum(2, Rand());
	CGameStatMng::ptr()->AddTopSceneNum(3, Rand());
	CGameStatMng::ptr()->AddTopCLRecvPacketNum(1, Rand());
	CGameStatMng::ptr()->AddTopCLRecvPacketNum(2, Rand());
	CGameStatMng::ptr()->AddTopCLRecvPacketNum(3, Rand());
	CGameStatMng::ptr()->PrintLog("stat.txt");
	
	return true;
}

bool xmlapp::exit()
{
	return true;
}

xmlapp::xmlapp() : mainapp("xmlapp")
{

}
