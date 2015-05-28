#include "fakengine.h"
#include "xmlapp.h"

bool xmlapp::ini( int argc, char *argv[] )
{
	return true;
}

bool xmlapp::heartbeat()
{
	XML_Parser pXML_Parser = XML_ParserCreate("../tools/genxml/sample.xml");
	FUSE(pXML_Parser);

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
