#pragma once

class C{{.Name}}CfgLoader
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
        int32_t m_i{{.Name}};
        {{end}}
    };

    const ST{{.Name}}Cfg & Get{{.Name}}Cfg()
    {
        return m_{{.Name}}Cfg;
    }
    {{end}}
        
	bool LoadCfg(const stringc & file)
	{
		inifile<16, 16> stConfigFile;
		if (!stConfigFile.load(file))
		{
			LOG_ERROR("LoadCfg(%s) failed\n", file.c_str());
			return false;
		}

		{{range .Structs}}
		// -------------Begin Load m_{{.Name}}Cfg {{.Comment}}---------------
		{{$name := .Name}}
		{{range .Members}}
		// {{.Comment}}
		stConfigFile.get("{{$name}}",
								  "{{.Name}}",
								  m_{{$name}}Cfg.m_i{{.Name}},
								  {{.Default}});
		{{end}}
		// -------------End Load m_{{.Name}}Cfg {{.Comment}}---------------
		{{end}}
		
		LOG_DEBUG("after load {{.Name}} cfg get ARGS:");
		
		LOG_DEBUG({{range .Structs}}{{range .Members}}"{{.Name}}[%d], "
			{{end}}{{end}}{{range .Structs}}{{$name := .Name}}{{range .Members}}, m_{{$name}}Cfg.m_i{{.Name}}
			{{end}}{{end}});

		return true;
	}
    
protected:
    {{range .Structs}}
    // {{.Comment}}
    ST{{.Name}}Cfg m_{{.Name}}Cfg;
    {{end}}
};
