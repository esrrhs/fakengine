#pragma once

class fproto
{
public:
    force_inline fproto()
	{
		clear();
	}

	force_inline ~fproto()
	{

	}

	force_inline void clear()
	{
	    m_lasterr.clear();
	}

	force_inline bool parse(stringc file)
	{
	    clear();
	    SAFE_TEST_RET_VAL(parse_xml(file), false, false);
	    SAFE_TEST_RET_VAL(parse_proto(), false, false);
	    SAFE_TEST_RET_VAL(check_proto(), false, false);
	    return true;
	}

    force_inline stringc err() const
    {   
        return m_lasterr;
    }
private:
    typedef std::vector<xml_member> xml_member_vec;
    typedef std::vector<xml_enum> xml_enum_vec;
    typedef std::vector<xml_struct> xml_struct_vec;
    
    struct xml_member
    {
        stringc m_name;
        stringc m_type;
        stringc m_ref;
        stringc m_length;
        stringc m_comment;
    };

    struct xml_enum
    {
        stringc m_name;
        stringc m_comment;
        xml_member_vec m_xml_member_vec;
    };

    struct xml_struct
    {
        stringc m_name;
        stringc m_comment;
        stringc m_type;
        xml_member_vec m_xml_member_vec;
    };

    struct xml_proto
    {
        xml_enum_vec m_xml_enum_vec;
        xml_struct_vec m_xml_struct_vec;
    };
    
private:
    force_inline bool parse_xml(stringc file)
	{
	    TiXmlDocument doc("../tools/genxml/sample.xml");		
        bool loadOkay = doc.LoadFile();		
        if (!loadOkay)		
        {
            snprintf(m_lasterr.c_str(), m_lasterr.size() - 1, "Load File %s Error", file.c_str());
            return false;
        }

        TiXmlElement* pResult = doc.RootElement();
        if (!pResult)
    	{
            snprintf(m_lasterr.c_str(), m_lasterr.size() - 1, "No RootElement Error", file.c_str());
    		return false;
    	}

    	parse_xml_enum(pResult, m_xml_proto);
    	parse_xml_struct(pResult, m_xml_proto);
    	
	    return true;
	}
    force_inline bool parse_proto()
	{
	    
	    return true;
	}
    force_inline bool check_proto()
	{
	    
	    return true;
	}
	force_inline bool parse_xml_enum(TiXmlElement* pRoot, xml_proto & xml_proto)
	{
	    for (TiXmlNode * pEnum = pRoot->FirstChild("Enum"); NULL != pEnum; pEnum = pRoot->NextSibling("Enum"))
        {
            xml_enum xmlenum;
            parse_enum(pEnum, xmlenum);
            xml_proto.m_xml_enum_vec.push_back(xmlenum);
        }
	    return true;
	}
	force_inline bool parse_enum(TiXmlElement* pEnum, xml_enum & xmlenum)
	{
        xmlenum.m_name = ((TiXmlElement *)pEnum)->Attribute("name");
        xmlenum.m_comment = ((TiXmlElement *)pEnum)->Attribute("comment");
	    for (TiXmlNode * pMember = pEnum->FirstChild("member"); NULL != pMember; pMember = pEnum->NextSibling("member"))
        {
            xml_member xmlmember;
            parse_member(pMember, xmlmember);
            xmlenum.m_xml_member_vec.push_back(xmlmember);
        }
	    return true;
	}
	force_inline bool parse_member(TiXmlElement* pMember, xml_member & xmlmember)
	{
        xmlmember.m_name = ((TiXmlElement *)pMember)->Attribute("name");
        xmlmember.m_type = ((TiXmlElement *)pMember)->Attribute("type");
        xmlmember.m_ref = ((TiXmlElement *)pMember)->Attribute("ref");
        xmlmember.m_length = ((TiXmlElement *)pMember)->Attribute("length");
        xmlmember.m_comment = ((TiXmlElement *)pMember)->Attribute("comment");
	    return true;
	}
	force_inline bool parse_struct(TiXmlElement* pStruct, xml_struct & xmlstruct)
	{
        xmlstruct.m_name = ((TiXmlElement *)pStruct)->Attribute("name");
        xmlstruct.m_type = ((TiXmlElement *)pStruct)->Attribute("type");
        xmlstruct.m_comment = ((TiXmlElement *)pStruct)->Attribute("comment");
	    for (TiXmlNode * pMember = pStruct->FirstChild("member"); NULL != pMember; pMember = pStruct->NextSibling("member"))
        {
            xml_member xmlmember;
            parse_member(pMember, xmlmember);
            xmlstruct.m_xml_member_vec.push_back(xmlmember);
        }
	    return true;
	}
	force_inline bool parse_xml_struct(TiXmlElement* pRoot)
	{
	    for (TiXmlNode * pStruct = pRoot->FirstChild("struct"); NULL != pStruct; pStruct = pRoot->NextSibling("struct"))
        {
            xml_struct xmlstruct;
            parse_struct(pStruct, xmlstruct);
            xml_proto.m_xml_struct_vec.push_back(xmlstruct);
        }
	    return true;
	}
private:
	stringc m_lasterr;
	xml_proto m_xml_proto;
};

