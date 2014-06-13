#include "{{.Name}}CfgLoader.h"
#include "TSF4GHelper.h"
#include "CfgHelper.h"

namespace PROJ_NS
{

bool C{{.Name}}CfgLoader::LoadCfg(bool bStart) /*= 0*/
{
    CConfigFile stConfigFile;
    if (stConfigFile.OpenFile("{{..SrcName}}"))
    {
        MERROR("C{{.Name}}CfgLoader::LoadCfg(%s) failed\n", "{{..SrcName}}");
        return false;
    }

    {{range .Structs}}
    // -------------Begin Load m_{{.Name}}Cfg {{.Comment}}---------------
    {{$name := .Name}}
    {{range .Members}}
    // {{.Comment}}
    stConfigFile.GetItemValue("{{$name}}",
                              "{{.Name}}",
                              m_{{$name}}Cfg.m_i{{.Name}},
                              {{.Default}});
    {{end}}
    // -------------End Load m_{{.Name}}Cfg {{.Comment}}---------------
    {{end}}
    
    MCFG("after load {{.Name}} cfg get ARGS:");
    
    MCFG({{range .Structs}}{{range .Members}}"{{.Name}}[%d], "
        {{end}}{{end}}{{range .Structs}}{{$name := .Name}}{{range .Members}}, m_{{$name}}Cfg.m_i{{.Name}}
        {{end}}{{end}});

    return true;
}

}
