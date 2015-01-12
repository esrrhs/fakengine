#include "{{.Name}}CfgLoader.h"
#include "TSF4GHelper.h"
#include "CfgHelper.h"

namespace PROJ_NS
{

bool C{{.Name}}CfgLoader::LoadCfg(bool bStart) /*= 0*/
{
	MDEBUG("C{{.Name}}CfgLoader::LoadCfg\n");

    // 先清空{{range .Tables}}
	// table {{.Name}}
    m_{{.Name}}CfgMap.clear();{{end}}
	
	//打开数据库
	sqlite3 *pCfgDB = NULL;
	std::string tableName = XXX_DB_CFG;
    int iRet = sqlite3_open_v2(tableName.c_str(), &pCfgDB, SQLITE_OPEN_READONLY, NULL);
    if (SQLITE_OK != iRet)
    {
	    MERROR("sqlite3_open fail. %s\n", tableName.c_str());
        sqlite3_close(pCfgDB);
        return false;
    }
	
	MDEBUG("sqlite3_open ok. %s\n", tableName.c_str());

	{{range .Tables}}
	// 读取{{.Name}}
    {
        sqlite3_stmt *pStmt = NULL;
        const char *szTail = 0;
        
        //读取表信息
        CSqlStrPackage stSqlstr;
        stSqlstr.Snprintf("SELECT {{range .Members}}{{.Name}}, {{end}} rowid from {{.Name}};");
        MDEBUG("SQL:%s\n", stSqlstr.GetSql());
        
        iRet = sqlite3_prepare(pCfgDB, stSqlstr.GetSql(), stSqlstr.GetSqlLen(), &pStmt, &szTail);
        if (iRet != SQLITE_OK)
        {
            MERROR("Error in table %s : sqlite3_prepare failed: %d\n", tableName.c_str(), iRet);
            sqlite3_close(pCfgDB);
            return false;
        }

        while (1)
        {
            int32_t i32ReturnCode = sqlite3_step(pStmt);
            if (i32ReturnCode != SQLITE_ROW) 
            {
                if (SQLITE_DONE != i32ReturnCode) 
                {
                    MERROR("Error SQLITE_DONE != i32ReturnCode %d\n", i32ReturnCode);
                    return false;
                }
                break;
            }
            
            int iPos = 0;

            ST{{.Name}}Cfg tmp;
            
			{{range .Members}}
			// column {{.Name}}
            {{if eq .Type "integer"}}tmp.m_i{{.Name}} = sqlite3_column_int(pStmt, iPos);{{else}}tmp.m_str{{.Name}} = (const char *)sqlite3_column_text(pStmt, iPos);{{end}}
            iPos++;
			{{end}}
            
            MDEBUG("ST{{.Name}}Cfg: \
				{{range .Members}}{{if eq .Type "integer"}}{{.Name}}[%d]{{else}}{{.Name}}[%s]{{end}} \
				{{end}}\n" 
				{{range .Members}}, {{if eq .Type "integer"}}tmp.m_i{{.Name}}{{else}}tmp.m_str{{.Name}}.c_str(){{end}}
				{{end}});

            m_{{.Name}}CfgMap[tmp.m_i{{.Key}}] = tmp;
        }
    }
	{{end}}
	
	return true;
}

}
