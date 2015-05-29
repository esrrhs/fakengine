#pragma once

#include <string>
#include "common/Framework/ICfgLoader.h"
#include "common/TSF4GHelper.h"

namespace PROJ_NS
{

// {{.Name}}
class C{{.Name}}CfgLoader : public CDoubleCfgLoader<C{{.Name}}CfgLoader>
{
public:
    {{range .Tables}}
	// table {{.Name}}
    struct ST{{.Name}}Cfg
    {
        ST{{.Name}}Cfg()
        {
			{{range .Members}}
			// column {{.Name}}
			{{if eq .Type "integer"}}m_i{{.Name}} = 0;{{else}}m_str{{.Name}}.clear();{{end}}
			{{end}}
        }
        {{range .Members}}
		// column {{.Name}}
		{{if eq .Type "integer"}}int m_i{{.Name}};{{else}}std::string m_str{{.Name}};{{end}}
        {{end}}
    };
	
	// table {{.Name}} map
	typedef std::map<int, ST{{.Name}}Cfg> {{.Name}}CfgMap;

	// table {{.Name}}
    const ST{{.Name}}Cfg * Get{{.Name}}CfgBy{{.Key}}(int i{{.Key}})
    {
		{{.Name}}CfgMap::iterator it = m_{{.Name}}CfgMap.find(i{{.Key}});
		if (it != m_{{.Name}}CfgMap.end())
		{
			return &it->second;
		}
        return NULL;
    }
	
	// table {{.Name}}
    const {{.Name}}CfgMap & Get{{.Name}}CfgMap()
    {
        return m_{{.Name}}CfgMap;
    }
    {{end}}
        
    virtual bool LoadCfg(bool bStart) /*= 0*/;
    
protected:
    {{range .Tables}}
	// table {{.Name}}
    {{.Name}}CfgMap m_{{.Name}}CfgMap;
    {{end}}
};

}
