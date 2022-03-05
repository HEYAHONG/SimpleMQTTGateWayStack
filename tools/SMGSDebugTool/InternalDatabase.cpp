#include "InternalDatabase.h"
#include "wx/log.h"

sqlite3 *memdb=NULL;
void InternalDatabase_Init()
{
    int rc=0;
    if((rc=sqlite3_open(":memory:",&memdb))!=SQLITE_OK)
    {
        wxLogMessage(_T("初始化内部数据库出错(%d:%s)!"),rc,sqlite3_errstr(rc));
    }

    {
        //创建表
        wxArrayString Header;
        Header.Add(_T("KEY"));
        Header.Add(_T("VALUE"));
        InternalDatabase_Create_Table(_T("ProgramInfo"),Header);
    }
}


bool InternalDatebase_ProgramInfo_Set(wxString key,wxString value)
{
    if(memdb==NULL)
    {
        return false;
    }

    int rc=0;

    std::map<wxString,wxString> Data;

    Data["KEY"]=key;


    {
        //删除之前的数据
        if(!InternalDatabase_Table_Delete_Data(_T("ProgramInfo"),Data))
        {
            return false;
        }
    }

    Data["VALUE"]=value;

    {
        //插入信息
        if(!InternalDatabase_Table_Insert_Data(_T("ProgramInfo"),Data))
        {
            return false;
        }
    }


    return true;
}

static int InternalDatebase_ProgramInfo_Get_cb(void *para,int ncolumn,char ** columnvalue,char *columnname[])
{
    if(para==NULL)
    {
        return 0;
    }

    wxString &ret=(*(wxString *)para);

    int i;
    for(i = 0; i < ncolumn; i++)
    {
        if(wxString(_T("VALUE"))==columnname[i])
        {
            ret=columnvalue[i];
        }
    }
    return 0;
}

wxString InternalDatebase_ProgramInfo_Get(wxString key)
{
    wxString ret;

    if(memdb==NULL)
    {
        return ret;
    }

    int rc=0;

    {
        //查询信息
        wxString sql=((wxString)"SELECT * FROM ProgramInfo WHERE KEY=\'")+key+"\';";
        //操作表
        if((rc= sqlite3_exec(memdb,sql.ToAscii(),InternalDatebase_ProgramInfo_Get_cb,&ret,NULL))!=SQLITE_OK)
        {
            wxLogMessage(_T("操作内部数据库出错(%d:%s)!"),rc,sqlite3_errstr(rc));
            return ret;
        }
    }

    return ret;

}

bool InternalDatabase_Backup(wxString destpath)
{
    if(memdb==NULL)
    {
        return false;
    }

    sqlite3 *filedb=NULL;

    if(sqlite3_open(destpath.ToAscii(),&filedb)!=SQLITE_OK)
    {
        return false;
    }

    sqlite3_backup * filedb_bak=sqlite3_backup_init(filedb,"main",memdb,"main");
    {
        sqlite3_backup_step(filedb_bak,-1);
    }
    sqlite3_backup_finish(filedb_bak);

    if(filedb!=NULL)
    {
        sqlite3_close(filedb);
    }


    return true;
}

static int InternalDatabase_Is_Table_Valied_cb(void *para,int ncolumn,char ** columnvalue,char *columnname[])
{
    if(para==NULL)
    {
        return 0;
    }

    wxString &ret=(*(wxString *)para);

    int i;
    for(i = 0; i < ncolumn; i++)
    {
        ret=columnvalue[i];
    }
    return 0;
}

//检查表名是否有效
bool InternalDatabase_Is_Table_Valied(wxString Table_Name)
{
    wxString ret;

    if(memdb==NULL || Table_Name.empty())
    {
        return false;
    }

    int rc=0;

    {
        //查询信息
        wxString sql=((wxString)"SELECT NAME FROM SQLITE_MASTER WHERE TYPE=\'table\' AND NAME=\'")+Table_Name+"\';";
        //操作表
        if((rc= sqlite3_exec(memdb,sql.ToAscii(),InternalDatabase_Is_Table_Valied_cb,&ret,NULL))!=SQLITE_OK)
        {
            wxLogMessage(_T("操作内部数据库出错(%d:%s)!"),rc,sqlite3_errstr(rc));
        }
    }

    return !ret.empty();
}




bool InternalDatabase_Create_Table(wxString Table_Name,wxArrayString Header)
{

    if(memdb==NULL || Header.size()==0)
    {
        return false;
    }

    if(InternalDatabase_Is_Table_Valied(Table_Name))
    {
        return false;
    }

    int rc=0;

    wxString sql;
    {
        sql+=_T("CREATE TABLE ");
        sql+=Table_Name;
        sql+=_T("(");
        sql+=Header[0];

        for(size_t i=1; i<Header.size(); i++)
        {
            sql+=_T(",");
            sql+=Header[i];
        }

        sql+=_T(");");
    }
    if((rc= sqlite3_exec(memdb,sql.ToAscii(),NULL,NULL,NULL))!=SQLITE_OK)
    {
        wxLogMessage(_T("操作内部数据库出错(%d:%s)!"),rc,sqlite3_errstr(rc));
        return false;
    }


    return true;
}


static int InternalDatabase_Table_Get_AllData_cb(void *para,int ncolumn,char ** columnvalue,char *columnname[])
{
    if(para==NULL)
    {
        return 0;
    }

    std::map<wxString,wxArrayString> &ret=(*(std::map<wxString,wxArrayString> *)para);

    int i;
    for(i = 0; i < ncolumn; i++)
    {
        std::map<wxString,wxArrayString>::iterator it=ret.find(wxString(columnname[i]));
        if(it!=ret.end())
        {
            it->second.Add(columnvalue[i]);
        }
        else
        {
            wxArrayString col;
            col.Add(columnvalue[i]);
            ret.insert(std::pair<wxString,wxArrayString>(columnname[i],col));
        }

    }
    return 0;
}
std::map<wxString,wxArrayString> InternalDatabase_Table_Get_AllData(wxString Table_Name,std::map<wxString,wxString> Condition)
{
    std::map<wxString,wxArrayString> ret;

    if(!InternalDatabase_Is_Table_Valied(Table_Name))
    {
        return ret;
    }

    int rc=0;

    {
        //查询信息
        wxString sql=((wxString)"SELECT * FROM ")+Table_Name;
        if(!Condition.empty())
        {
            sql+=_T(" WHERE ");
            for(auto it=Condition.begin(); it!=Condition.end();)
            {
                sql+=(it->first+_T("=\'")+it->second+_T("\'"));
                if((++it)!=Condition.end())
                {
                    sql+=_T(" AND ");
                }
            }
        }
        sql+=_T(";");
        //操作表
        if((rc= sqlite3_exec(memdb,sql.ToAscii(),InternalDatabase_Table_Get_AllData_cb,&ret,NULL))!=SQLITE_OK)
        {
            wxLogMessage(_T("操作内部数据库出错(%d:%s)!"),rc,sqlite3_errstr(rc));
        }
    }

    return ret;
}

bool InternalDatabase_Table_Insert_Data(wxString Table_Name,std::map<wxString,wxString> Data)
{
    if(!InternalDatabase_Is_Table_Valied(Table_Name) || Data.empty())
    {
        return false;
    }

    {
        int rc=0;
        //插入信息
        wxString sql;
        {
            sql+=_T("INSERT INTO ");
            sql+=Table_Name;
            sql+=_T(" ( ");
            for(auto it=Data.begin(); it!=Data.end();)
            {
                sql+=it->first;
                if((++it)!=Data.end())
                {
                    sql+=_T(",");
                }
            }

            sql+=_T(")");

            sql+=_T(" VALUES(");

            for(auto it=Data.begin(); it!=Data.end();)
            {
                sql+=_T("\'");
                sql+=it->second;
                sql+=_T("\'");
                if((++it)!=Data.end())
                {
                    sql+=_T(",");
                }
            }

            sql+=_T(");");

        }

        //操作表
        if((rc= sqlite3_exec(memdb,sql.ToAscii(),NULL,NULL,NULL))!=SQLITE_OK)
        {
            wxLogMessage(_T("操作内部数据库出错(%d:%s)!"),rc,sqlite3_errstr(rc));
            return false;
        }
    }

    return true;
}

bool InternalDatabase_Table_Delete_Data(wxString Table_Name,std::map<wxString,wxString> Condition)
{
    if(!InternalDatabase_Is_Table_Valied(Table_Name))
    {
        return false;
    }

    {
        int rc=0;
        //插入信息
        wxString sql;
        {
            sql+=_T("DELETE  FROM ");
            sql+=Table_Name;

            if(!Condition.empty())
            {
                sql+=_T(" WHERE ");
                for(auto it=Condition.begin(); it!=Condition.end();)
                {
                    sql+=(it->first+_T("=\'")+it->second+_T("\'"));
                    if((++it)!=Condition.end())
                    {
                        sql+=_T(" AND ");
                    }
                }
            }

            sql+=_T(";");

        }

        //操作表
        if((rc= sqlite3_exec(memdb,sql.ToAscii(),NULL,NULL,NULL))!=SQLITE_OK)
        {
            wxLogMessage(_T("操作内部数据库出错(%d:%s)!"),rc,sqlite3_errstr(rc));
            return false;
        }
    }

    return true;
}


bool InternalDatabase_Table_Update_Data(wxString Table_Name,std::map<wxString,wxString> Data,std::map<wxString,wxString> Condition)
{
    if(!InternalDatabase_Is_Table_Valied(Table_Name) || Data.empty())
    {
        return false;
    }

    {
        int rc=0;
        //更新信息
        wxString sql;
        {
            sql+=_T("UPDATE ");
            sql+=Table_Name;

            sql+=_T(" SET ");

            for(auto it=Data.begin(); it!=Data.end();)
            {
                sql+=it->first;
                sql+=_T("=");
                sql+=_T("\'");
                sql+=it->second;
                sql+=_T("\'");
                if((++it)!=Data.end())
                {
                    sql+=_T(",");
                }
            }

            if(!Condition.empty())
            {
                sql+=_T(" WHERE ");
                for(auto it=Condition.begin(); it!=Condition.end();)
                {
                    sql+=(it->first+_T("=\'")+it->second+_T("\'"));
                    if((++it)!=Condition.end())
                    {
                        sql+=_T(" AND ");
                    }
                }
            }

            sql+=_T(";");

        }

        //操作表
        if((rc= sqlite3_exec(memdb,sql.ToAscii(),NULL,NULL,NULL))!=SQLITE_OK)
        {
            wxLogMessage(_T("操作内部数据库出错(%d:%s)!"),rc,sqlite3_errstr(rc));
            return false;
        }
    }

    return true;
}

void InternalDatabase_Deinit()
{
    if(memdb!=NULL)
    {
        sqlite3_close(memdb);
    }
}
