#include "StatMgr.h"

namespace PROJ_NS
{

void SortArray(uint32_t * src, int srcSize, uint32_t * desId, uint32_t * desVal, int desSize, bool isMax)
{
    if (isMax)
    {
        for (int i = 0; i < desSize; i++)
        {
            desId[i] = 0;
            desVal[i] = 0;
            uint32_t max = 0;
            for (int j = 0; j < srcSize; j++)
            {
                if (src[j] > max)
                {
                    desId[i] = j;
                    desVal[i] = src[j];
                    max = src[j];
                }
            }
            src[desId[i]] = 0;
        }
    }
    else
    {
        for (int i = 0; i < desSize; i++)
        {
            desId[i] = 0;
            desVal[i] = 0;
            uint32_t min = 0xFFFFFFFF;
            for (int j = 0; j < srcSize; j++)
            {
                if (src[j] < min && src[j] > 0)
                {
                    desId[i] = j;
                    desVal[i] = src[j];
                    min = src[j];
                }
            }
            src[desId[i]] = 0xFFFFFFFF;
        }
    }
}

template <typename Map>
void SortHashMap(Map & map, uint32_t * desId, uint32_t * desVal, int desSize, bool isMax)
{
    if (isMax)
    {
        for (int i = 0; i < desSize; i++)
        {
            desId[i] = 0;
            desVal[i] = 0;
            uint32_t max = 0;
            for (typename Map::iterator it = map.begin(); it != map.end(); it++)
            {
                if (it->second > max)
                {
                    desId[i] = it->first;
                    desVal[i] = it->second;
                    max = it->second;
                }
            }
            typename Map::iterator it = map.find(desId[i]);
            if (it != map.end())
            {
                it->second = 0;
            }
        }
    }
    else
    {
        for (int i = 0; i < desSize; i++)
        {
            desId[i] = 0;
            desVal[i] = 0;
            uint32_t min = 0xFFFFFFFF;
            for (typename Map::iterator it = map.begin(); it != map.end(); it++)
            {
                if (it->second < min && it->second > 0)
                {
                    desId[i] = it->first;
                    desVal[i] = it->second;
                    min = it->second;
                }
            }
            typename Map::iterator it = map.find(desId[i]);
            if (it != map.end())
            {
                it->second = 0xFFFFFFFF;
            }
        }
    }
}
    
{{range .}}  
// {{iconv .Comment}}
C{{.Name}}Mgr::C{{.Name}}Mgr()
{
    Clear();
}

// {{iconv .Comment}}
C{{.Name}}Mgr::~C{{.Name}}Mgr()
{
    Clear();
}

// {{iconv .Comment}}
void C{{.Name}}Mgr::Clear()
{
    {{range .Members}}
    // {{iconv .Comment}}
    m_{{.Name}} = 0;
    {{end}}
    {{range .Arrays}}
    // {{iconv .Comment}}
    memset(m_{{.Name}}, 0, sizeof(m_{{.Name}}));
    {{end}}
    {{range .TopArrays}}
    // {{iconv .Comment}}
    memset(m_{{.Name}}, 0, sizeof(m_{{.Name}}));
    {{end}}
    {{range .HashMaps}}
    // {{iconv .Comment}}
    m_{{.Name}}.clear();
    {{end}}
}

// {{iconv .Comment}}
int C{{.Name}}Mgr::PrintLog(const char * strStatDir)
{
    if(NULL == strStatDir)
    {
        MERROR("NULL strStatDir encountered");
        return -1;
    }

    CLinuxHelper::MakeDirectory(strStatDir);

    char strDir[1024];
    snprintf(strDir
        , sizeof(strDir) - 1
        , "%s/%04d-%02d"
        , strStatDir
        , CClock::instance()->Year()
        , CClock::instance()->Month()
        );
    
    CLinuxHelper::MakeDirectory(strDir);

    char strFile[1024];
    snprintf(strFile, sizeof(strFile) - 1, "%s/%s.txt"
        , strDir
        , CClock::instance()->GetDateStr()
        );
        
    FILE *pLog = NULL;
    if ((pLog = fopen(strFile, "a+")) == NULL)
    {
        MERROR("open %s fail", strFile);
        return -2;
    }
    
    {{range .TopArrays}}
    // {{iconv .Comment}}
    uint32_t u{{.Name}}Id[{{.Top}}] = {0};
    uint32_t u{{.Name}}Val[{{.Top}}] = {0};
    SortArray(m_{{.Name}}, {{.Length}}, u{{.Name}}Id, u{{.Name}}Val, {{.Top}}, {{if .Cmp}}false{{else}}true{{end}});
    {{end}}
    {{range .HashMaps}}
    // {{iconv .Comment}}
    uint32_t u{{.Name}}Id[{{.Top}}] = {0};
    uint32_t u{{.Name}}Val[{{.Top}}] = {0};
    SortHashMap< CHashMap<{{.Length}}, uint32_t, uint32_t> >(m_{{.Name}}, u{{.Name}}Id, u{{.Name}}Val, {{.Top}}, {{if .Cmp}}false{{else}}true{{end}});
    {{end}}
    
    fprintf(pLog, "=============================%s %s=============================\n"
        {{range .Members}}
        // {{iconv .Comment}}
        "{{.Name}}\t%u\t{{iconv .Comment}}\n"
        {{end}}
		{{range .Arrays}}{{$comment := iconv .Comment}}{{$name := iconv .Name}}
        {{range $i,$e := genlist .Length}}
        // {{$comment}} {{$i}}
        "{{$name}}{{$i}}\t%u\t{{$comment}}{{$i}}\n"
        {{end}}
        {{end}}
        {{range .TopArrays}}{{$comment := iconv .Comment}}{{$name := iconv .Name}}
        {{range $i,$e := genlist .Top}}
        // {{$comment}} {{$i}}
        "{{$name}}Id{{$i}}\t%u\t{{$comment}}Id{{$i}}\n{{$name}}Val{{$i}}\t%u\t{{$comment}}Val{{$i}}\n"
        {{end}}
        {{end}}
        {{range .HashMaps}}{{$comment := iconv .Comment}}{{$name := iconv .Name}}
        {{range $i,$e := genlist .Top}}
        // {{$comment}} {{$i}}
        "{{$name}}Id{{$i}}\t%u\t{{$comment}}Id{{$i}}\n{{$name}}Val{{$i}}\t%u\t{{$comment}}Val{{$i}}\n"
        {{end}}
        {{end}}
        "\n"
        
        , CClock::instance()->GetDateStr()
        , CClock::instance()->GetTimeOfDayStr()
        {{range .Members}}
        // {{iconv .Comment}}
        , m_{{.Name}}
        {{end}}
		{{range .Arrays}}{{$comment := iconv .Comment}}{{$name := .Name}}
        {{range $i,$e := genlist .Length}}
        // {{$comment}} {{$i}}
        , m_{{$name}}[{{$i}}]
        {{end}}
        {{end}}
        {{range .TopArrays}}{{$comment := iconv .Comment}}{{$name := .Name}}
        {{range $i,$e := genlist .Top}}
        // {{$comment}} {{$i}}
        , u{{$name}}Id[{{$i}}], u{{$name}}Val[{{$i}}]
        {{end}}
        {{end}}
        {{range .HashMaps}}{{$comment := iconv .Comment}}{{$name := .Name}}
        {{range $i,$e := genlist .Top}}
        // {{$comment}} {{$i}}
        , u{{$name}}Id[{{$i}}], u{{$name}}Val[{{$i}}]
        {{end}}
        {{end}}
        );
        
    fclose(pLog);
    
    Clear();

    return 0;
}

{{end}}  

}
