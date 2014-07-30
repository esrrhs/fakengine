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
		memset(m_lasterr, 0, sizeof(m_lasterr));
	}

	force_inline bool parse(stringc file)
	{
		clear();
		SAFE_TEST_RET_VAL(parse_xml(file), false, false);
		SAFE_TEST_RET_VAL(check_proto(), false, false);
		SAFE_TEST_RET_VAL(parse_proto(), false, false);
		return true;
	}

	force_inline stringc err() const
	{
		return m_lasterr;
	}
private:
	struct xml_member
	{
		stringc m_name;
		stringc m_type;
		stringc m_ref;
		stringc m_length;
		stringc m_comment;
	};
	typedef std::vector<xml_member> xml_member_vec;

	struct xml_enum
	{
		stringc m_name;
		stringc m_comment;

		xml_member_vec m_xml_member_vec;
	};
	typedef std::vector<xml_enum> xml_enum_vec;

	struct xml_struct
	{
		stringc m_name;
		stringc m_comment;

		stringc m_type;
		xml_member_vec m_xml_member_vec;
	};
	typedef std::vector<xml_struct> xml_struct_vec;

	struct xml_proto
	{
		xml_enum_vec m_xml_enum_vec;
		xml_struct_vec m_xml_struct_vec;
	};

private:
	force_inline bool parse_xml(stringc file)
	{
		TiXmlDocument doc((const char *)file.c_str());
		bool loadOkay = doc.LoadFile();
		if (!loadOkay)
		{
			fsnprintf(m_lasterr, sizeof(m_lasterr),
				"Load File %s Error", file.c_str());
			return false;
		}

		TiXmlElement* pResult = doc.RootElement();
		if (!pResult)
		{
			fsnprintf(m_lasterr, sizeof(m_lasterr),
				"No RootElement Error", file.c_str());
			return false;
		}

		parse_xml_enum(pResult, m_xml_proto);
		parse_xml_struct(pResult, m_xml_proto);

		return true;
	}
	force_inline bool check_proto()
	{
		SAFE_TEST_RET_VAL(check_proto_enum(), false, false);
		SAFE_TEST_RET_VAL(check_proto_struct(), false, false);
		
		return true;
	}
	force_inline bool parse_proto()
	{

		return true;
	}
private:
	force_inline bool is_base_type(const stringc & type)
	{
		if (type == "int8_t" ||
			type == "uint8_t" ||
			type == "int16_t" ||
			type == "uint16_t" ||
			type == "int32_t" ||
			type == "uint32_t" ||
			type == "int64_t" ||
			type == "uint64_t" ||
			type == "float" ||
			type == "char")
		{
			return true;
		}
		return false;
	}
	force_inline bool check_proto_enum()
	{
		// 检查语法
		{
			for (int32_t i = 0; i < (int32_t)m_xml_proto.m_xml_enum_vec.size(); i++)
			{
				xml_enum & xmlenum = m_xml_proto.m_xml_enum_vec[i];
				if (xmlenum.m_name.empty())
				{
					fsnprintf(m_lasterr, sizeof(m_lasterr),
						"Enum %s Empty Name Comment %s Error", xmlenum.m_name.c_str(), xmlenum.m_comment.c_str());
					return false;
				}

				for (int32_t j = 0; j < (int32_t)xmlenum.m_xml_member_vec.size(); j++)
				{
					xml_member & xmlmember = xmlenum.m_xml_member_vec[j];
					if (xmlmember.m_name.empty() || xmlmember.m_type.empty())
					{
						fsnprintf(m_lasterr, sizeof(m_lasterr),
							"Enum %s Member Empty Name %s Type %s Error", xmlenum.m_name.c_str(),
							xmlmember.m_name.c_str(), xmlmember.m_type.c_str());
						return false;
					}
				}
			}
		}

		// 检查重名
		{
			std::set<stringc> nameset;
			for (int32_t i = 0; i < (int32_t)m_xml_proto.m_xml_enum_vec.size(); i++)
			{
				xml_enum & xmlenum = m_xml_proto.m_xml_enum_vec[i];
				if (nameset.find(xmlenum.m_name) != nameset.end())
				{
					fsnprintf(m_lasterr, sizeof(m_lasterr),
						"Enum Double Name %s Error", xmlenum.m_name.c_str());
					return false;
				}
				nameset.insert(xmlenum.m_name);
				for (int32_t j = 0; j < (int32_t)xmlenum.m_xml_member_vec.size(); j++)
				{
					xml_member & xmlmember = xmlenum.m_xml_member_vec[j];
					if (nameset.find(xmlmember.m_name) != nameset.end())
					{
						fsnprintf(m_lasterr, sizeof(m_lasterr),
							"Enum %s Member Double Name %s Error", xmlenum.m_name.c_str(), xmlmember.m_name.c_str());
						return false;
					}
					nameset.insert(xmlmember.m_name);
				}
			}
		}

		return true;
	}
	force_inline bool check_proto_struct()
	{
		// 检查语法
		{
			for (int32_t i = 0; i < (int32_t)m_xml_proto.m_xml_struct_vec.size(); i++)
			{
				xml_struct & xmlstruct = m_xml_proto.m_xml_struct_vec[i];
				if (xmlstruct.m_name.empty())
				{
					fsnprintf(m_lasterr, sizeof(m_lasterr),
						"Struct %s Empty Name Comment %s Error", xmlstruct.m_name.c_str(), xmlstruct.m_comment.c_str());
					return false;
				}

				for (int32_t j = 0; j < (int32_t)xmlstruct.m_xml_member_vec.size(); j++)
				{
					xml_member & xmlmember = xmlstruct.m_xml_member_vec[j];
					if (xmlmember.m_name.empty() || xmlmember.m_type.empty())
					{
						fsnprintf(m_lasterr, sizeof(m_lasterr),
							"Struct %s Member Empty Name %s Type %s Error", xmlstruct.m_name.c_str(),
							xmlmember.m_name.c_str(), xmlmember.m_type.c_str());
						return false;
					}

					if (xmlstruct.m_type == "union")
					{
						if (xmlmember.m_ref.empty())
						{
							fsnprintf(m_lasterr, sizeof(m_lasterr),
								"Struct %s is Union, Member %s Empty Ref Error", xmlstruct.m_name.c_str(),
								xmlmember.m_name.c_str());
							return false;
						}
					}
					else
					{
						if (!xmlmember.m_ref.empty() && xmlmember.m_length.empty())
						{
							fsnprintf(m_lasterr, sizeof(m_lasterr),
								"Struct %s Member %s Empty Lenth But Have Ref %s Error", xmlstruct.m_name.c_str(),
								xmlmember.m_name.c_str(), xmlmember.m_ref.c_str());
							return false;
						}
					}
				}
			}
		}

		// 检查重名
		{
			std::set<stringc> nameset;
			for (int32_t i = 0; i < (int32_t)m_xml_proto.m_xml_struct_vec.size(); i++)
			{
				xml_struct & xmlstruct = m_xml_proto.m_xml_struct_vec[i];
				if (nameset.find(xmlstruct.m_name) != nameset.end())
				{
					fsnprintf(m_lasterr, sizeof(m_lasterr),
						"Struct Double Name %s Error", xmlstruct.m_name.c_str());
					return false;
				}
				nameset.insert(xmlstruct.m_name);

				std::set<stringc> structnameset;
				for (int32_t j = 0; j < (int32_t)xmlstruct.m_xml_member_vec.size(); j++)
				{
					xml_member & xmlmember = xmlstruct.m_xml_member_vec[j];
					if (structnameset.find(xmlmember.m_name) != structnameset.end())
					{
						fsnprintf(m_lasterr, sizeof(m_lasterr),
							"Struct %s Member Double Name %s Error", xmlstruct.m_name.c_str(), xmlmember.m_name.c_str());
						return false;
					}
					structnameset.insert(xmlmember.m_name);
				}
			}
	}

		// 检查type是否有定义
		{
			std::set<stringc> nameset;
			for (int32_t i = 0; i < (int32_t)m_xml_proto.m_xml_struct_vec.size(); i++)
			{
				xml_struct & xmlstruct = m_xml_proto.m_xml_struct_vec[i];
				nameset.insert(xmlstruct.m_name);
			}

			for (int32_t i = 0; i < (int32_t)m_xml_proto.m_xml_struct_vec.size(); i++)
			{
				xml_struct & xmlstruct = m_xml_proto.m_xml_struct_vec[i];

				for (int32_t j = 0; j < (int32_t)xmlstruct.m_xml_member_vec.size(); j++)
				{
					xml_member & xmlmember = xmlstruct.m_xml_member_vec[j];
					if (!is_base_type(xmlmember.m_type))
					{
						if (nameset.find(xmlmember.m_type) == nameset.end())
						{
							fsnprintf(m_lasterr, sizeof(m_lasterr),
								"Struct %s Member %s Type %s not define Error", xmlstruct.m_name.c_str(),
								xmlmember.m_name.c_str(), xmlmember.m_type.c_str());
							return false;
						}
					}
				}
			}
		}

		// 检查枚举是否有定义
		{
			std::set<stringc> enumnameset;
			for (int32_t i = 0; i < (int32_t)m_xml_proto.m_xml_enum_vec.size(); i++)
			{
				xml_enum & xmlenum = m_xml_proto.m_xml_enum_vec[i];
				for (int32_t j = 0; j < (int32_t)xmlenum.m_xml_member_vec.size(); j++)
				{
					xml_member & xmlmember = xmlenum.m_xml_member_vec[j];
					enumnameset.insert(xmlmember.m_name);
				}
			}

			for (int32_t i = 0; i < (int32_t)m_xml_proto.m_xml_struct_vec.size(); i++)
			{
				xml_struct & xmlstruct = m_xml_proto.m_xml_struct_vec[i];

				for (int32_t j = 0; j < (int32_t)xmlstruct.m_xml_member_vec.size(); j++)
				{
					xml_member & xmlmember = xmlstruct.m_xml_member_vec[j];
					if (!xmlmember.m_length.empty())
					{
						if (enumnameset.find(xmlmember.m_length) == enumnameset.end())
						{
							fsnprintf(m_lasterr, sizeof(m_lasterr),
								"Struct %s Member %s Length %s not define Error", xmlstruct.m_name.c_str(),
								xmlmember.m_name.c_str(), xmlmember.m_length.c_str());
							return false;
						}
					}

					if (xmlstruct.m_type == "union")
					{
						if (enumnameset.find(xmlmember.m_ref) == enumnameset.end())
						{
							fsnprintf(m_lasterr, sizeof(m_lasterr),
								"Struct %s is Union, Member %s Ref %s not define Error", xmlstruct.m_name.c_str(),
								xmlmember.m_name.c_str(), xmlmember.m_ref.c_str());
							return false;
						}
					}
				}
			}
		}

		// 检查结构体内refer
		{
			for (int32_t i = 0; i < (int32_t)m_xml_proto.m_xml_struct_vec.size(); i++)
			{
				xml_struct & xmlstruct = m_xml_proto.m_xml_struct_vec[i];
				for (int32_t j = 0; j < (int32_t)xmlstruct.m_xml_member_vec.size(); j++)
				{
					xml_member & xmlmember = xmlstruct.m_xml_member_vec[j];
					if (xmlstruct.m_type != "union" && !xmlmember.m_ref.empty())
					{
						bool find = false;
						for (int32_t z = 0; z < j; z++)
						{
							xml_member & prexmlmember = xmlstruct.m_xml_member_vec[z];
							if (prexmlmember.m_name == xmlmember.m_ref && 
								is_base_type(prexmlmember.m_type) &&
								prexmlmember.m_length.empty() &&
								prexmlmember.m_ref.empty())
							{
								find = true;
							}
						}

						if (!find)
						{
							fsnprintf(m_lasterr, sizeof(m_lasterr),
								"Struct %s Member %s Ref %s not find Error", xmlstruct.m_name.c_str(),
								xmlmember.m_name.c_str(), xmlmember.m_ref.c_str());
							return false;
						}
					}
				}
			}
		}

		return true;
	}
private:
	force_inline bool parse_xml_enum(TiXmlElement* pRoot, xml_proto & xmlproto)
	{
		for (TiXmlNode * pEnum = pRoot->FirstChild("enum"); NULL != pEnum; pEnum = pEnum->NextSibling("enum"))
		{
			xml_enum xmlenum;
			parse_enum((TiXmlElement*)pEnum, xmlenum);
			xmlproto.m_xml_enum_vec.push_back(xmlenum);
		}
		return true;
	}
	force_inline bool parse_enum(TiXmlElement* pEnum, xml_enum & xmlenum)
	{
		xmlenum.m_name = ((TiXmlElement *)pEnum)->Attribute("name");
		xmlenum.m_comment = ((TiXmlElement *)pEnum)->Attribute("comment");
		for (TiXmlNode * pMember = pEnum->FirstChild("member"); NULL != pMember; pMember = pMember->NextSibling("member"))
		{
			xml_member xmlmember;
			parse_member((TiXmlElement*)pMember, xmlmember);
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
		for (TiXmlNode * pMember = pStruct->FirstChild("member"); NULL != pMember; pMember = pMember->NextSibling("member"))
		{
			xml_member xmlmember;
			parse_member((TiXmlElement*)pMember, xmlmember);
			xmlstruct.m_xml_member_vec.push_back(xmlmember);
		}
		return true;
	}
	force_inline bool parse_xml_struct(TiXmlElement* pRoot, xml_proto & xmlproto)
	{
		for (TiXmlNode * pStruct = pRoot->FirstChild("struct"); NULL != pStruct; pStruct = pStruct->NextSibling("struct"))
		{
			xml_struct xmlstruct;
			parse_struct((TiXmlElement*)pStruct, xmlstruct);
			xmlproto.m_xml_struct_vec.push_back(xmlstruct);
		}
		return true;
	}
private:
	int8_t m_lasterr[1024];
	xml_proto m_xml_proto;
};

