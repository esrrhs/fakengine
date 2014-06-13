#pragma once

#include <string>
#include "common/Framework/ICfgLoader.h"
#include "common/TSF4GHelper.h"

namespace PROJ_NS
{

class C{{.Name}}CfgLoader : public CDoubleCfgLoader<C{{.Name}}CfgLoader>
{
public:
    {{range .Structs}}
    // {{.Comment}}
    struct ST{{.Name}}Cfg
    {
        ST{{.Name}}Cfg()
        {
            memset(this, 0, sizeof(*this));
        }
        {{range .Members}}
        // {{.Comment}}
        int m_i{{.Name}};
        {{end}}
    };

    const ST{{.Name}}Cfg & Get{{.Name}}Cfg()
    {
        return m_{{.Name}}Cfg;
    }
    {{end}}
        
    virtual bool LoadCfg(bool bStart) /*= 0*/;
    
protected:
    {{range .Structs}}
    // {{.Comment}}
    ST{{.Name}}Cfg m_{{.Name}}Cfg;
    {{end}}
};

}
