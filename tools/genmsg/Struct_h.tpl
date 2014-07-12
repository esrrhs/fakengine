#pragma once

#include <stdint.h>

#define PROTO_MIN(a,b) Min<int32_t>((int32_t)a, (int32_t)b)

namespace Fproto
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

{{define "MarshalCheck"}}if (ret + (int32_t)sizeof(m_{{.Name}}) > size)
		{
			return -ret;
		}{{end}}
{{define "MarshalAdd"}}ret += sizeof(m_{{.Name}});
		destbuffer += sizeof(m_{{.Name}});{{end}}
		
{{define "MarshalNormal"}}{{template "MarshalCheck" .}}
		*({{.Type}}*)destbuffer = m_{{.Name}};
		{{template "MarshalAdd" .}}{{end}}		
	
{{define "UnmarshalCheck"}}if (ret + (int32_t)sizeof(m_{{.Name}}) > size)
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
	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

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
	
	template <typename T>
	bool Marshal(T & buffer) const
	{		
		buffer.begin("{{.Name}}");
		
		SAFE_TEST_RET_VAL(buffer.add("Type", m_Type), false, false);
		
		switch (m_Type)
		{
		{{range .Members}}  
		// {{.Comment}}  
		case {{.Ref}}:
			{
				T tmp{{.Name}}Buff;
				SAFE_TEST_RET_VAL(m_{{.Name}}.Marshal(tmp{{.Name}}Buff), false, false);
				SAFE_TEST_RET_VAL(buffer.add("{{.Name}}", tmp{{.Name}}Buff), false, false);
			}
			break;
		{{end}}  
		default:
			return false;
		}
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		SAFE_TEST_RET_VAL(buffer.get("Type", m_Type), false, false);
		
		switch (m_Type)
		{
		{{range .Members}}  
		// {{.Comment}}  
		case {{.Ref}}:
			{
				T tmp{{.Name}}Buff;
				SAFE_TEST_RET_VAL(buffer.get("{{.Name}}", tmp{{.Name}}Buff), false, false);
				SAFE_TEST_RET_VAL(m_{{.Name}}.Unmarshal(tmp{{.Name}}Buff), false, false);
			}
			break;
		{{end}}  
		default:
			return false;
		}
		
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size) const
	{
		int32_t ret = 0;
		
		// type
		if (ret + (int32_t)sizeof(m_Type) > size)
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
				int32_t ret{{.Name}} = m_{{.Name}}.Marshal(destbuffer, size - ret);
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
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		
		// type
		if (ret + (int32_t)sizeof(m_Type) > size)
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
				int32_t ret{{.Name}} = m_{{.Name}}.Unmarshal(srcbuffer, size - ret);
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
	
	template <typename T>
	bool Marshal(T & buffer) const
	{			
		buffer.begin("{{.Name}}");
			
		{{range .Members}} 
		// {{.Comment}}	
		{{if eq .Type "char"}}SAFE_TEST_RET_VAL(buffer.add("{{.Name}}", m_{{.Name}}, {{.Length}}), false, false);
		{{else if .Length}}{{if is_normal_type .Type}}int32_t copy{{.Name}}Size = {{if .Ref}}sizeof({{.Type}}) * PROTO_MIN({{.Length}}, m_{{.Ref}}){{else}}sizeof(m_{{.Name}}){{end}};
		SAFE_TEST_RET_VAL(buffer.add("{{.Name}}", m_{{.Name}}, copy{{.Name}}Size), false, false);
		{{else}}T tmp{{.Name}}Buff;
		stringc tmp{{.Name}}Name;
		for (int32_t i = 0; i < {{.Length}}{{if .Ref}} && i < m_{{.Ref}}{{end}}; i++)
		{
			tmp{{.Name}}Buff.reset();
			SAFE_TEST_RET_VAL(m_{{.Name}}[i].Marshal(tmp{{.Name}}Buff), false, false);
			tmp{{.Name}}Name = "{{.Name}}";
			tmp{{.Name}}Name += i;
			SAFE_TEST_RET_VAL(buffer.add(tmp{{.Name}}Name, tmp{{.Name}}Buff), false, false);
		}{{end}}{{else if is_normal_type .Type}}SAFE_TEST_RET_VAL(buffer.add("{{.Name}}", m_{{.Name}}), false, false);
		{{else}}{
			T tmp{{.Name}}Buff;
			SAFE_TEST_RET_VAL(m_{{.Name}}.Marshal(tmp{{.Name}}Buff), false, false);
			SAFE_TEST_RET_VAL(buffer.add("{{.Name}}", tmp{{.Name}}Buff), false, false);
		}{{end}} 
		{{end}}
		
		buffer.end();
		
		return true;
	}
	
	template <typename T>
	bool Unmarshal(T & buffer)
	{			
		{{range .Members}} 
		// {{.Comment}}	
		{{if eq .Type "char"}}buffer.get("{{.Name}}", m_{{.Name}}, {{.Length}});
		{{else if .Length}}{{if is_normal_type .Type}}int32_t copy{{.Name}}Size = {{if .Ref}}sizeof({{.Type}}) * PROTO_MIN({{.Length}}, m_{{.Ref}}){{else}}sizeof(m_{{.Name}}){{end}};
		buffer.get("{{.Name}}", m_{{.Name}}, copy{{.Name}}Size);
		{{else}}T tmp{{.Name}}Buff;
		stringc tmp{{.Name}}Name;
		for (int32_t i = 0; i < {{.Length}}{{if .Ref}} && i < m_{{.Ref}}{{end}}; i++)
		{
			tmp{{.Name}}Buff.reset();
			tmp{{.Name}}Name = "{{.Name}}";
			tmp{{.Name}}Name += i;
			SAFE_TEST_CONTINUE(buffer.get(tmp{{.Name}}Name, tmp{{.Name}}Buff), false);
			SAFE_TEST_CONTINUE(m_{{.Name}}[i].Unmarshal(tmp{{.Name}}Buff), false);
		}{{end}}{{else if is_normal_type .Type}}buffer.get("{{.Name}}", m_{{.Name}});
		{{else}}
		do
		{
			T tmp{{.Name}}Buff;
			SAFE_TEST_BREAK(buffer.get("{{.Name}}", tmp{{.Name}}Buff), false);
			SAFE_TEST_BREAK(m_{{.Name}}.Unmarshal(tmp{{.Name}}Buff), false);
		}
		while(0);{{end}} 
		{{end}}
		return true;
	}
	
	int32_t Marshal(char * destbuffer, int32_t size) const
	{
		int32_t ret = 0;
		{{range .Members}} 
		// {{.Comment}}	
		{{if eq .Type "char"}}{{template "MarshalCheck" .}}
		fstrcopy(destbuffer, m_{{.Name}}, sizeof(m_{{.Name}}));
		{{template "MarshalAdd" .}}
		{{else if .Length}}{{if is_normal_type .Type}}int32_t copy{{.Name}}Size = {{if .Ref}}sizeof({{.Type}}) * PROTO_MIN({{.Length}}, m_{{.Ref}}){{else}}sizeof(m_{{.Name}}){{end}};
		if (ret + copy{{.Name}}Size > size)
		{
			return -ret;
		}
		if (copy{{.Name}}Size > 0)
		{
			memcpy(destbuffer, m_{{.Name}}, copy{{.Name}}Size);
		}
		ret += copy{{.Name}}Size;
		destbuffer += copy{{.Name}}Size;{{else}}for (int32_t i = 0; i < {{.Length}}{{if .Ref}} && i < m_{{.Ref}}{{end}}; i++)
		{
			int32_t ret{{.Name}} = m_{{.Name}}[i].Marshal(destbuffer, size - ret);
			if (ret{{.Name}} <= 0)
			{
				return -ret + ret{{.Name}};
			}
			ret += ret{{.Name}};
			destbuffer += ret{{.Name}};
		}{{end}}
		{{else if is_normal_type .Type}}{{template "MarshalNormal" .}}
		{{else}}int32_t ret{{.Name}} = m_{{.Name}}.Marshal(destbuffer, size - ret);
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
	
	int32_t Unmarshal(char * srcbuffer, int32_t size)
	{
		int32_t ret = 0;
		{{range .Members}} 
		// {{.Comment}}	
		{{if eq .Type "char"}}{{template "UnmarshalCheck" .}}
		fstrcopy(m_{{.Name}}, srcbuffer);
		{{template "UnmarshalAdd" .}}
		{{else if .Length}}{{if is_normal_type .Type}}int32_t copy{{.Name}}Size = {{if .Ref}}sizeof({{.Type}}) * PROTO_MIN({{.Length}}, m_{{.Ref}}){{else}}sizeof(m_{{.Name}}){{end}};
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
		{{if .Ref}}m_{{.Ref}} = PROTO_MIN(m_{{.Ref}}, {{.Length}});{{end}}{{else}}for (int32_t i = 0; i < {{.Length}}{{if .Ref}} && i < m_{{.Ref}}{{end}}; i++)
		{
			int32_t ret{{.Name}} = m_{{.Name}}[i].Unmarshal(srcbuffer, size - ret);
			if (ret{{.Name}} <= 0)
			{
				return -ret + ret{{.Name}};
			}
			ret += ret{{.Name}};
			srcbuffer += ret{{.Name}};
		}
		{{if .Ref}}m_{{.Ref}} = PROTO_MIN(m_{{.Ref}}, {{.Length}});{{end}}{{end}}
		{{else if is_normal_type .Type}}{{template "UnmarshalNormal" .}}
		{{else}}int32_t ret{{.Name}} = m_{{.Name}}.Unmarshal(srcbuffer, size - ret);
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
