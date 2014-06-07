#pragma once

#include <stdint.h>

#define PROTO_MIN(a,b) Min<int32_t>((int32_t)a, (int32_t)b)

namespace Protocol
{

{{range .Enums}}    
// {{.Comment}}  
enum {{.Name}}
{
	{{range .Members}} 
	// {{.Comment}}  
	{{.Name}} = {{.Type}},
	{{end}}  
};
{{end}}  

{{define "MarshalCheck"}}if (ret + (int)sizeof(m_{{.Name}}) > size)
		{
			return -ret;
		}{{end}}
{{define "MarshalAdd"}}ret += sizeof(m_{{.Name}});
		destbuffer += sizeof(m_{{.Name}});{{end}}
		
{{define "MarshalNormal"}}{{template "MarshalCheck" .}}
		*({{.Type}}*)destbuffer = m_{{.Name}};
		{{template "MarshalAdd" .}}{{end}}		
	
{{define "UnmarshalCheck"}}if (ret + (int)sizeof(m_{{.Name}}) > size)
		{
			return -ret;
		}{{end}}
{{define "UnmarshalAdd"}}ret += sizeof(m_{{.Name}});
		srcbuffer += sizeof(m_{{.Name}});{{end}}
{{define "UnmarshalNormal"}}{{template "UnmarshalCheck" .}}
		m_{{.Name}} = *({{.Type}}*)srcbuffer;
		{{template "UnmarshalAdd" .}}{{end}}	
		
{{range .Structs}}    
// {{.Comment}}  
struct {{.Name}}  
{  
	{{if eq .Type "union"}}
	// Type 
	int32_t m_Type;
	
	union
	{
		{{range .Members}}  
		// {{.Comment}}  
		{{.Type}} m_{{.Name}}{{if .Length}}[{{.Length}}]{{end}};  
		{{end}}  
	};
	
	int Marshal(char * destbuffer, int size)
	{
		int ret = 0;
		
		// type
		if (ret + (int)sizeof(m_Type) > size)
		{
			return -ret;
		}
		*(int32_t*)destbuffer = m_Type;
		ret += sizeof(m_Type);
		destbuffer += sizeof(m_Type);
		
		switch (m_Type)
		{
		{{range .Members}}  
		// {{.Comment}}  
		case {{.Ref}}:
			{
				int ret{{.Name}} = m_{{.Name}}.Marshal(destbuffer, size - ret);
				if (ret{{.Name}} <= 0)
				{
					return -ret + ret{{.Name}};
				}
				ret += ret{{.Name}};
				destbuffer += ret{{.Name}};
			}
			break;
		{{end}}  
		default:
			return -ret;
		}
		return ret;
	}
	
	int Unmarshal(char * srcbuffer, int size)
	{
		int ret = 0;
		
		// type
		if (ret + (int)sizeof(m_Type) > size)
		{
			return -ret;
		}
		m_Type = *(int32_t*)srcbuffer;
		ret += sizeof(m_Type);
		srcbuffer += sizeof(m_Type);
		
		switch (m_Type)
		{
		{{range .Members}}  
		// {{.Comment}}  
		case {{.Ref}}:
			{
				int ret{{.Name}} = m_{{.Name}}.Unmarshal(srcbuffer, size - ret);
				if (ret{{.Name}} <= 0)
				{
					return -ret + ret{{.Name}};
				}
				ret += ret{{.Name}};
				srcbuffer += ret{{.Name}};
			}
			break;
		{{end}}  
		default:
			return -ret;
		}
		return ret;
	}
	
	{{else}}
	{{range .Members}}  
	// {{.Comment}}  
	{{.Type}} m_{{.Name}}{{if .Length}}[{{.Length}}]{{end}};  
	{{end}}  
	
	int Marshal(char * destbuffer, int size)
	{
		int ret = 0;
		{{range .Members}} 
		// {{.Comment}}	
		{{if eq .Type "char"}}{{template "MarshalCheck" .}}
		strncpy(destbuffer, m_{{.Name}}, sizeof(m_{{.Name}}));
		{{template "MarshalAdd" .}}
		{{else if .Length}}{{if is_normal_type .Type}}int copy{{.Name}}Size = {{if .Ref}}sizeof({{.Type}}) * PROTO_MIN({{.Length}}, m_{{.Ref}}){{else}}sizeof(m_{{.Name}}){{end}};
		if (ret + copy{{.Name}}Size > size)
		{
			return -ret;
		}
		if (copy{{.Name}}Size > 0)
		{
			memcpy(destbuffer, m_{{.Name}}, copy{{.Name}}Size);
		}
		ret += copy{{.Name}}Size;
		destbuffer += copy{{.Name}}Size;{{else}}for (int i = 0; i < {{.Length}}{{if .Ref}} && i < m_{{.Ref}}{{end}}; i++)
		{
			int ret{{.Name}} = m_{{.Name}}[i].Marshal(destbuffer, size - ret);
			if (ret{{.Name}} <= 0)
			{
				return -ret + ret{{.Name}};
			}
			ret += ret{{.Name}};
			destbuffer += ret{{.Name}};
		}{{end}}
		{{else if is_normal_type .Type}}{{template "MarshalNormal" .}}
		{{else}}int ret{{.Name}} = m_{{.Name}}.Marshal(destbuffer, size - ret);
		if (ret{{.Name}} <= 0)
		{
			return -ret + ret{{.Name}};
		}
		ret += ret{{.Name}};
		destbuffer += ret{{.Name}};
		{{end}} 
		{{end}} 
		return ret;
	}
	
	int Unmarshal(char * srcbuffer, int size)
	{
		int ret = 0;
		{{range .Members}} 
		// {{.Comment}}	
		{{if eq .Type "char"}}{{template "UnmarshalCheck" .}}
		strncpy(m_{{.Name}}, srcbuffer, sizeof(m_{{.Name}}));
		{{template "UnmarshalAdd" .}}
		{{else if .Length}}{{if is_normal_type .Type}}int copy{{.Name}}Size = {{if .Ref}}sizeof({{.Type}}) * PROTO_MIN({{.Length}}, m_{{.Ref}}){{else}}sizeof(m_{{.Name}}){{end}};
		if (ret + copy{{.Name}}Size > size)
		{
			return -ret;
		}
		if (copy{{.Name}}Size > 0)
		{
			memcpy(m_{{.Name}}, srcbuffer, copy{{.Name}}Size);
		}
		ret += copy{{.Name}}Size;
		srcbuffer += copy{{.Name}}Size;
		{{if .Ref}}m_{{.Ref}} = PROTO_MIN(m_{{.Ref}}, {{.Length}});{{end}}{{else}}for (int i = 0; i < {{.Length}}{{if .Ref}} && i < m_{{.Ref}}{{end}}; i++)
		{
			int ret{{.Name}} = m_{{.Name}}[i].Unmarshal(srcbuffer, size - ret);
			if (ret{{.Name}} <= 0)
			{
				return -ret + ret{{.Name}};
			}
			ret += ret{{.Name}};
			srcbuffer += ret{{.Name}};
		}
		{{if .Ref}}m_{{.Ref}} = PROTO_MIN(m_{{.Ref}}, {{.Length}});{{end}}{{end}}
		{{else if is_normal_type .Type}}{{template "UnmarshalNormal" .}}
		{{else}}int ret{{.Name}} = m_{{.Name}}.Unmarshal(srcbuffer, size - ret);
		if (ret{{.Name}} <= 0)
		{
			return -ret + ret{{.Name}};
		}
		ret += ret{{.Name}};
		srcbuffer += ret{{.Name}};
		{{end}} 
		{{end}} 
		return ret;
	}
	{{end}}
};  
{{end}}  

}
